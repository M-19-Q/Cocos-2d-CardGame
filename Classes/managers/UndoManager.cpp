#include "UndoManager.h"
#include "../views/CardView.h"

USING_NS_CC;

// UndoManager类
// 功能：游戏撤销操作管理器
// 职责：执行撤销操作，恢复卡牌到移动前的位置，重置手牌顶部牌状态; 记录卡牌移动操作的关键信息，存入UndoModel；初始化时绑定GameModel与UndoModel，建立撤销记录的数据关联
// 使用场景：由GameController调用，记录卡牌点击移动的操作记录；响应撤销按钮点击事件，执行单步撤销逻辑；


 // 构造函数
 // 功能：初始化UndoManager
UndoManager::UndoManager(GameModel* model)
    : _model(model)
    , _undoModel(model->getUndoModel()) {
}

// 功能：封装卡牌移动的关键信息，生成撤销记录并存入UndoModel
void UndoManager::recordMove(
    CardModel* card,
    UndoType type,
    const cocos2d::Vec2& oldPos,
    const cocos2d::Vec2& targetPos,
    CardModel* replacedCard) {

    if (!card || !replacedCard) {
        CCLOG("UndoManager: recordMove failed, invalid card");
        return;
    }

    // 封装被点击并移动的卡牌的撤销记录
    UndoRecord record;
    record.card = card;
    record.type = type;
    record.oldPos = oldPos;
    record.targetPos = targetPos;
    record.replacedCard = replacedCard;

    _undoModel->addRecord(record);

    CCLOG(
        "UndoManager: Record move, card=%p, old=(%f,%f), target=(%f,%f), replaced=%p",
        card, oldPos.x, oldPos.y, targetPos.x, targetPos.y, replacedCard
    );
}

// 执行最后一次移动操作的撤销
// 功能：从UndoModel取出最新撤销记录，恢复卡牌到移动前位置，重置顶部牌状态
bool UndoManager::undoLastMove(std::map<CardModel*, CardView*>& cardViewMap) {
    if (!_undoModel->hasRecords()) {
        CCLOG("UndoManager: No undo records");
        return false;
    }

    // 单次撤销仅处理最新的一条记录
    UndoRecord record = _undoModel->popRecord();

    if (!record.card || !cardViewMap.count(record.card)) {
        CCLOG("UndoManager: Invalid undo record");
        return false;
    }

    // 恢复被点击卡牌到移动前位置（仅处理该张卡牌的回退）
    CardView* cardView = cardViewMap[record.card];
    cardView->moveTo(record.oldPos);      // 执行动画回退到原位置
    record.card->setPosition(record.oldPos);
    cardView->setLocalZOrder(2);

    CCLOG(
        "UndoManager: Undo card %p to (%f,%f)",
        record.card, record.oldPos.x, record.oldPos.y
    );

    if (record.replacedCard) {
        _model->setCurrentHandTopCard(record.replacedCard);
        CCLOG(
            "UndoManager: Restore top hand card to %p",
            record.replacedCard
        );
    }

    return true;
}