#include "UndoModel.h"

// UndoModel类
// 功能：管理撤销记录的存储与读取
// 职责：保存操作记录、按后进先出规则返回记录
// 使用场景：支撑撤销操作，存储卡牌移动的历史记录
void UndoModel::addRecord(const UndoRecord& record) {
    _records.push_back(record);
}


// 功能：保存单次操作记录，不做逻辑判断
UndoRecord UndoModel::popRecord() {
    if (_records.empty()) {
        UndoRecord empty;
        empty.card = nullptr;
        empty.replacedCard = nullptr;
        empty.type = UndoType::MOVE_FROM_HAND;
        empty.oldPos = cocos2d::Vec2::ZERO;
        empty.targetPos = cocos2d::Vec2::ZERO;
        empty.replacedOldPos = cocos2d::Vec2::ZERO;
        return empty;
    }

    UndoRecord last = _records.back();
    _records.pop_back();
    return last;
}