#include "GameView.h"
#include "../models/GameModel.h"
#include "../controllers/GameController.h"
#include "ui/CocosGUI.h" 
#include "../models/CardModel.h"
#include "CardView.h"

USING_NS_CC;

// GameView类
// 功能：游戏界面总控视图，负责整合所有UI元素（背景、卡牌、按钮）的创建与管理
// 职责：初始化游戏背景/手牌区视觉样式；创建所有卡牌视图并绑定数据模型；生成撤销按钮并绑定点击事件；清理视图资源
// 使用场景：游戏启动时初始化界面，承接控制器指令，管理界面元素
GameView::GameView(cocos2d::Layer* layer, GameModel* model)
    : _layer(layer), _model(model), _undoBtn(nullptr), _controller(nullptr) {
}

// 析构函数
// 移除并清空所有卡牌视图; 移除撤销按钮
GameView::~GameView() {
    // 遍历卡牌视图映射表，移除所有卡牌视图并清空表
    for (auto& pair : _cardViewMap) {
        if (pair.second) {
            pair.second->removeFromParent();
        }
    }
    _cardViewMap.clear();

    // 移除撤销按钮
    if (_undoBtn) {
        _undoBtn->removeFromParent();
    }
}

// 初始化游戏界面所有UI元素
// 功能：依次创建背景、手牌区背景、所有预设卡牌视图、撤销按钮，并绑定按钮交互事件
void GameView::initView() {
    // 校验核心层对象是否有效，无效则终止初始化
    if (!_layer) {
        CCLOG("GameView: Layer is nullptr!");
        return;
    }

    // 创建游戏主背景 
    // 优先加载背景图片，加载失败则创建绿色纯色背景兜底
    auto bg = Sprite::create("card/background.png");
    if (bg) {
        bg->setPosition(Vec2(TABLE_AREA_WIDTH / 2, (TABLE_AREA_HEIGHT + HAND_AREA_HEIGHT) / 2));
        _layer->addChild(bg, 0); // 层级0：最底层背景
    }
    else {
        auto bgLayer = LayerColor::create(Color4B(0, 150, 0, 255), TABLE_AREA_WIDTH, TABLE_AREA_HEIGHT + HAND_AREA_HEIGHT);
        bgLayer->setPosition(0, 0);
        _layer->addChild(bgLayer, 0);
    }

    // 创建手牌区背景 
    // 优先加载手牌区图片，加载失败则创建红色纯色背景兜底
    auto handBg = Sprite::create("card/HandArea.png");
    if (handBg) {
        handBg->setContentSize(Size(1080, 580)); // 适配手牌区尺寸
        handBg->setPosition(1080 / 2, 580 / 2);   // 居中显示在屏幕下方
        _layer->addChild(handBg, 1); // 层级1：高于主背景
    }
    else {
        auto handBgLayer = LayerColor::create(Color4B(150, 0, 0, 255), HAND_AREA_WIDTH, HAND_AREA_HEIGHT);
        handBgLayer->setPosition(0, 0);
        _layer->addChild(handBgLayer, 1);
    }

    // 创建所有预设卡牌的视图
    // 桌面牌♦3：从模型获取数据，创建视图并添加到界面，存入视图映射表
    auto diamond3Model = _model->getDeskCard_Diamond3();
    if (diamond3Model) {
        CardView* diamond3View = CardView::create(diamond3Model);
        if (diamond3View) {
            _layer->addChild(diamond3View, 2); // 层级2：高于背景
            _cardViewMap[diamond3Model] = diamond3View; // 绑定模型与视图
        }
    }

    // 手牌♣6：创建视图并绑定模型
    auto club6Model = _model->getHandCard_Club6();
    if (club6Model) {
        CardView* club6View = CardView::create(club6Model);
        if (club6View) {
            _layer->addChild(club6View, 2);
            _cardViewMap[club6Model] = club6View;
        }
    }

    // 手牌♥A：创建视图并绑定模型
    auto heartAModel = _model->getHandCard_HeartA();
    if (heartAModel) {
        CardView* heartAView = CardView::create(heartAModel);
        if (heartAView) {
            _layer->addChild(heartAView, 2);
            _cardViewMap[heartAModel] = heartAView;
        }
    }

    // 桌面牌♥7：创建视图并绑定模型
    auto heart7Model = _model->getDeskCard_Heart7();
    if (heart7Model) {
        CardView* heart7View = CardView::create(heart7Model);
        if (heart7View) {
            _layer->addChild(heart7View, 2);
            _cardViewMap[heart7Model] = heart7View;
        }
    }

    // 手牌♠10：创建视图并绑定模型
    auto spade10Model = _model->getHandCard_Spade10();
    if (spade10Model) {
        CardView* spade10View = CardView::create(spade10Model);
        if (spade10View) {
            _layer->addChild(spade10View, 2);
            _cardViewMap[spade10Model] = spade10View;
        }
    }

    // 手牌顶部♣4：创建视图并绑定模型（游戏初始顶部牌）
    auto club4Model = _model->getHandTopCard_Club4();
    if (club4Model) {
        CardView* club4View = CardView::create(club4Model);
        if (club4View) {
            _layer->addChild(club4View, 2);
            _cardViewMap[club4Model] = club4View;
        }
    }

    // 桌面牌♠2：创建视图并绑定模型
    auto spade2Model = _model->getDeskCard_Spade2();
    if (spade2Model) {
        CardView* spade2View = CardView::create(spade2Model);
        if (spade2View) {
            _layer->addChild(spade2View, 2);
            _cardViewMap[spade2Model] = spade2View;
        }
    }

    // 创建撤销按钮 
    // 加载撤销按钮图片，绑定点击事件（触发控制器的撤销逻辑）
    auto undoBtn = cocos2d::ui::Button::create("ui/undo.png");
    if (undoBtn) {
        undoBtn->setPosition(Vec2(980, 290)); // 按钮位置：屏幕右下方
        undoBtn->setScale(0.1f);              // 按钮缩放比例
        _layer->addChild(undoBtn, 3);         // 层级3：高于卡牌
        _undoBtn = undoBtn;

        // 绑定按钮点击事件：调用控制器的撤销处理函数
        undoBtn->addClickEventListener([=](Ref*) {
            CCLOG("Undo Button Clicked");
            if (_controller) {
                _controller->handleUndoClick();
            }
            });
    }
    else {
        CCLOG("Undo button image not found!"); // 图片加载失败日志
    }
}