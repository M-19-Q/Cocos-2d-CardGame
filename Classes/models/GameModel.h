#ifndef GAME_MODEL_H
#define GAME_MODEL_H

#include "CardModel.h"
#include "UndoModel.h"

// GameModel类
// 功能：管理游戏全量核心数据，包括预设卡牌实例、游戏常量配置、手牌状态、撤销模型
// 职责：定义游戏全局常量;创建并管理所有预设卡牌;管理撤销模型（UndoModel）

class GameModel {
public:
    GameModel();
    ~GameModel();

    // 常量配置
    static const int DESIGN_WIDTH = 1080;
    static const int DESIGN_HEIGHT = 2080;
    static const int CARD_WIDTH = 181;
    static const int CARD_HEIGHT = 280;
    static const int MAIN_AREA_HEIGHT = 1500;
    static const int HAND_AREA_HEIGHT = 580;

    // 获取预设卡牌
    CardModel* getDeskCard_Diamond3() const { return _diamond3; }
    CardModel* getHandCard_Club6() const { return _club6; }
    CardModel* getHandCard_HeartA() const { return _heartA; }
    CardModel* getDeskCard_Heart7() const { return _heart7; }
    CardModel* getHandTopCard_Club4() const { return _club4; }
    CardModel* getDeskCard_Spade2() const { return _spade2; }
    CardModel* getHandCard_Spade10() const { return _spade10; }

    // ========= 手牌顶部牌 =========
    CardModel* getCurrentHandTopCard() const { return _currentHandTopCard; }

    void setCurrentHandTopCard(CardModel* card) {
        _currentHandTopCard = card;
    }

    bool isCurrentHandTopCard(CardModel* card) const {
        return card != nullptr && card == _currentHandTopCard;
    }

    // 回退模型
    UndoModel* getUndoModel() const { return _undoModel; }

private:
    // 预设卡牌
    CardModel* _diamond3;
    CardModel* _club6;
    CardModel* _heartA;
    CardModel* _heart7;
    CardModel* _spade10;
    CardModel* _club4;
    CardModel* _spade2;

    // 当前手牌区顶部牌
    CardModel* _currentHandTopCard;

    UndoModel* _undoModel;
};

#endif // GAME_MODEL_H
