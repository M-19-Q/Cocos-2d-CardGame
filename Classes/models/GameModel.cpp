#include "GameModel.h"

// GameModel类
// 功能：游戏核心数据模型，管理所有卡牌、游戏状态、撤销模型
// 职责：初始化预设卡牌的位置;区分桌面牌区 / 手牌区；创建所有卡牌，初始化手牌顶部牌为默认♣4；
// 使用场景：由GameScene创建，为GameController提供卡牌数据、游戏状态，为UndoManager提供撤销模型

GameModel::GameModel() {
    // 初始化预设卡牌位置
    // 桌面牌区（主牌区）：y轴 580~2080（HAND_AREA_HEIGHT ~ DESIGN_HEIGHT）
    // 手牌区（堆牌区）：y轴 0~580
    cocos2d::Vec2 pos_diamond3(200, 1500); // 桌面♦3
    cocos2d::Vec2 pos_club6(300, 200); // 手牌♦6
    cocos2d::Vec2 pos_heartA(350, 200); // 手牌♥A
    cocos2d::Vec2 pos_heart7(500, 1100); // 桌面♥7
    cocos2d::Vec2 pos_club4(700, 200); // 手牌顶部♣4
    cocos2d::Vec2 pos_spade2(300,1300); // 桌面♠2
    cocos2d::Vec2 pos_spade10(400, 200); // 手牌♠10


    // 创建卡牌
    _diamond3 = new CardModel(CardSuit::DIAMOND, CardRank::_3, pos_diamond3);
    _club6 = new CardModel(CardSuit::CLUB, CardRank::_6, pos_club6);
    _heartA = new CardModel(CardSuit::HEART, CardRank::A, pos_heartA);
    _heart7 = new CardModel(CardSuit::HEART, CardRank::_7, pos_heart7);
    _club4 = new CardModel(CardSuit::CLUB, CardRank::_4, pos_club4);
    _spade2 = new CardModel(CardSuit::SPADE, CardRank::_2, pos_spade2);
    _spade10 = new CardModel(CardSuit::SPADE, CardRank::_10, pos_spade10);


    // 初始顶部牌为♣4
    _currentHandTopCard = _club4;

    // 初始化回退模型
    _undoModel = new UndoModel();
}

GameModel::~GameModel() {
    delete _diamond3;
    delete _club6;
    delete _heartA;
    delete _heart7;
    delete _club4;
    delete _spade2;
    delete _spade10;
    delete _undoModel;
}