#include "GameController.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

// GameController类实现
// 功能：游戏核心控制器
// 职责：初始化触摸监听，响应卡牌点击、撤销按钮点击事件；处理卡牌点击的核心逻辑（手牌翻牌、桌面牌匹配）；实现撤销操作的完整逻辑（视觉回退、数据清理、顶部牌重置）；提供工具函数判断卡牌归属（手牌/桌面）、卡牌点数匹配规则。
// 使用场景：核心玩法逻辑;由GameScene创建并关联GameModel/GameView;处理所有用户触摸交互，驱动游戏数据模型更新和视图渲染

 // 构造函数
 // 功能：初始化GameController

GameController::GameController(GameModel* model, GameView* view, UndoManager* undoManager)
    : _model(model)
    , _view(view)
    , _cardViewMap(view->getCardViewMap())
    , _undoManager(undoManager) {
}

// 析构函数
// 功能：销毁GameController
GameController::~GameController() {
}

// 初始化触摸监听
// 功能：创建并注册触摸监听器，响应撤销按钮、卡牌的点击事件

void GameController::initTouchListener(cocos2d::Layer* layer) {
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = [=](cocos2d::Touch* touch, cocos2d::Event* event) -> bool {
        Vec2 touchPos = touch->getLocation();

        auto undoBtn = _view->getUndoBtn();
        if (undoBtn && undoBtn->isVisible()) {
            Vec2 localPos = undoBtn->convertToNodeSpace(touchPos);
            Rect rect(0, 0, undoBtn->getContentSize().width, undoBtn->getContentSize().height);
            if (rect.containsPoint(localPos)) {
                handleUndoClick();
                return true;
            }
        }

        for (auto& pair : _cardViewMap) {
            CardView* cardView = pair.second;
            if (!cardView) continue;

            Rect box = cardView->getBoundingBox();
            box.origin = cardView->getParent()->convertToWorldSpace(box.origin);
            if (box.containsPoint(touchPos)) {
                handleCardClick(pair.first);
                return true;
            }
        }
        return false;
        };

    Director::getInstance()->getEventDispatcher()
        ->addEventListenerWithSceneGraphPriority(listener, layer);
}

// 处理卡牌点击事件
// 功能：根据卡牌类型（手牌/桌面牌）执行不同逻辑，完成卡牌移动、数据记录、顶部牌更新

void GameController::handleCardClick(CardModel* card) {
    if (!card) return;

    if (_model->isCurrentHandTopCard(card)) {
        CCLOG("GameController: Top hand card is not clickable");
        return;
    }

    CardModel* currentTop = _model->getCurrentHandTopCard();
    if (!currentTop) return;

    Vec2 targetPos = currentTop->getCurrentPos();

    // 手牌区翻牌逻辑
    if (isHandCard(card)) {
        Vec2 oldPos = card->getCurrentPos();

        CardView* view = _cardViewMap[card];
        if (view) {
            view->moveTo(targetPos);
            card->setPosition(targetPos);
            view->setLocalZOrder(10);
        }

        _undoManager->recordMove(card, UndoType::MOVE_FROM_HAND,
            oldPos, targetPos, currentTop);

        _model->setCurrentHandTopCard(card);
        return;
    }

    // 桌面牌匹配逻辑
    if (isDeskCard(card)) {
        if (!isRankDiffOne(card, currentTop)) {
            CCLOG("GameController: Rank not match");
            return;
        }

        Vec2 oldPos = card->getCurrentPos();
        CardView* view = _cardViewMap[card];
        if (view) {
            view->moveTo(targetPos);
            card->setPosition(targetPos);
            view->setLocalZOrder(10);
        }

        _undoManager->recordMove(card, UndoType::MOVE_FROM_DESK,
            oldPos, targetPos, currentTop);

        _model->setCurrentHandTopCard(card);
    }
}


 // 功能：执行撤销操

void GameController::handleUndoClick() {
    // 1. 执行视觉回退，移动卡牌到原位置
    bool success = _undoManager->undoLastMove(_cardViewMap);
    if (!success) {
        CCLOG("GameController: No undo record to revert");
        return;
    }
    CCLOG("GameController: undo = %d", success);

    // 弹出回退记录
    _model->getUndoModel()->popRecord();

    // 定义手牌顶部固定位置（和 GameModel 初始化的♣4位置一致：700, 200）
    const Vec2 HAND_TOP_POS = Vec2(700, 200);

    // 遍历所有卡牌，找到「当前位置在手牌顶部区域」的卡牌
    CardModel* newTopCard = nullptr;
    float posTolerance = 5.0f; // 解决坐标微小偏差问题
    for (auto& pair : _cardViewMap) {
        CardModel* card = pair.first;
        if (!card || !isHandCard(card)) continue; // 仅遍历手牌区卡牌，减少无效判断

        // 扩大坐标容错：x/y分别判断，容错5像素
        bool isXMatch = abs(card->getCurrentPos().x - HAND_TOP_POS.x) < posTolerance;
        bool isYMatch = abs(card->getCurrentPos().y - HAND_TOP_POS.y) < posTolerance;
        if (isXMatch && isYMatch) {
            newTopCard = card;
            break;
        }
    }

    // 若未找到匹配卡牌，遍历手牌区找zOrder最高的牌（视觉最上层）
    if (newTopCard == nullptr) {
        int maxZOrder = -1;
        for (auto& pair : _cardViewMap) {
            CardModel* card = pair.first;
            CardView* view = pair.second;
            if (!card || !view || !isHandCard(card)) continue;

            int zOrder = view->getLocalZOrder();
            if (zOrder > maxZOrder) {
                maxZOrder = zOrder;
                newTopCard = card;
            }
        }
    }

    // 强制更新顶部牌
    if (newTopCard != nullptr) {
        _model->setCurrentHandTopCard(newTopCard);
        CCLOG("GameController: Undo success, new top card is %p (pos: %f, %f)",
            newTopCard, newTopCard->getCurrentPos().x, newTopCard->getCurrentPos().y);
    }
    else {
        // 极端情况：恢复初始顶部牌♣4
        _model->setCurrentHandTopCard(_model->getHandTopCard_Club4());
        CCLOG("GameController: Undo fallback to initial top card (Club4)");
    }
}

// 功能：根据卡牌Y坐标判断是否归属桌面区域
bool GameController::isDeskCard(CardModel* card) {
    return card->getCurrentPos().y > GameModel::HAND_AREA_HEIGHT;
}

// 功能：根据卡牌Y坐标判断是否归属手牌区域
bool GameController::isHandCard(CardModel* card) {
    return card->getCurrentPos().y <= GameModel::HAND_AREA_HEIGHT;
}

// 判断两张卡牌等级是否相差1
// 功能：校验卡牌等级匹配规则，用于桌面牌点击逻辑
// card1 - 第一张卡牌模型指针；
// card2 - 第二张卡牌模型指针。

bool GameController::isRankDiffOne(CardModel* card1, CardModel* card2) {
    if (!card1 || !card2) return false;
    return abs(card1->getRankValue() - card2->getRankValue()) == 1;
}