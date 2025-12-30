#ifndef UNDO_MODEL_H
#define UNDO_MODEL_H

#include "CardModel.h"
#include <vector>

// 操作类型
enum class UndoType {
    MOVE_FROM_HAND,  // 手牌区翻牌
    MOVE_FROM_DESK   // 桌面牌匹配
};

// 回退记录结构体
struct UndoRecord {
    CardModel* card;          // 移动的卡牌
    UndoType type;            // 操作类型
    cocos2d::Vec2 oldPos;     // 移动前位置
    cocos2d::Vec2 targetPos;  // 移动后位置
    CardModel* replacedCard;  // 被替换的顶部牌

    cocos2d::Vec2 replacedOldPos; // 被替换顶部牌原来的位置（用于正确回退）
};

// UndoModel类
// 功能：管理撤销记录的存储、读取与状态判断
// 职责：保存操作记录、按后进先出规则返回记录、判断记录是否存在
// 使用场景：支撑撤销操作，存储卡牌移动的历史记录

class UndoModel {
public:
    // 记录操作
    void addRecord(const UndoRecord& record);
    // 取出最后一条记录（出栈）
    UndoRecord popRecord();
    // 是否有可回退记录
    bool hasRecords() const { return !_records.empty(); }
    // 清空记录
    void clear() { _records.clear(); }

private:
    std::vector<UndoRecord> _records; // 回退记录栈
};

#endif // UNDO_MODEL_H