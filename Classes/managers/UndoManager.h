#pragma once

#include "../models/UndoModel.h"
#include "../models/GameModel.h"
#include "../views/CardView.h"
#include <map>

// UndoManager类
// 功能：撤销操作管理器
// 职责：初始化时绑定GameModel，关联UndoModel用于存储撤销记录；封装卡牌移动的核心信息;执行撤销操作，恢复卡牌到移动前的位置，重置手牌顶部牌状态；
// 使用场景：由GameController实例化并调用，记录卡牌点击移动的操作；响应撤销按钮点击事件，完成单步操作回退；依赖UndoModel存储/读取撤销记录

class UndoManager {
public:
    // 构造函数
    // 功能：初始化UndoManager

    UndoManager(GameModel* model);

    // 功能：封装卡牌移动的关键信息，生成撤销记录并存储到UndoModel
    void recordMove(CardModel* card, UndoType type, const cocos2d::Vec2& oldPos,
        const cocos2d::Vec2& targetPos, CardModel* replacedCard);

    // 执行单次撤销操作
    // 功能：取出最新的撤销记录，恢复卡牌到移动前位置，重置顶部牌状态
    bool undoLastMove(std::map<CardModel*, CardView*>& cardViewMap);

private:
    // 重置手牌顶部牌状态，关联UndoModel
    GameModel* _model;

    // 撤销数据模型指针
    UndoModel* _undoModel;
};