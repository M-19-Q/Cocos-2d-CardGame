#pragma once
#include <map>
#include "cocos2d.h"
#include "../views/GameView.h"
#include "../models/GameModel.h"
#include "../managers/UndoManager.h"
#include "../models/UndoModel.h"
#include "../views/CardView.h"

// GameController类
// 功能：游戏核心控制器
// 职责：绑定GameModel（数据）、GameView（视图）、UndoManager（撤销管理）;初始化触摸监听，响应撤销按钮、卡牌的点击交互； 实现卡牌点击的业务规则（手牌翻牌、桌面牌匹配）；处理撤销操作逻辑
// 使用场景：由GameScene创建并初始化;所有用户触摸事件的响应与处理;依赖GameView完成界面渲染更新

class GameController {
public:
    // 构造函数
    // 功能：初始化GameController
    GameController(GameModel* model, GameView* view, UndoManager* undoManager);

    // 析构函数
    // 功能：销毁GameController
    ~GameController();

    // 初始化触摸监听
    // 功能：创建并注册触摸监听器，绑定到指定层，响应撤销按钮、卡牌的点击事件
    void initTouchListener(cocos2d::Layer* layer);

// 处理卡牌点击事件
    // 功能：根据卡牌类型（手牌/桌面牌）执行对应逻辑，完成卡牌移动与数据更新
    void handleCardClick(CardModel* card);

    // 处理撤销按钮点击事件
    // 执行撤销操作，包含视觉回退、数据清理、顶部牌重置
    void handleUndoClick();

private:
    // 存储游戏核心数据（卡牌状态、顶部牌、撤销记录等）
    GameModel* _model;

    // 提供界面渲染、控件获取（如撤销按钮）、卡牌视图管理等能力
    GameView* _view;

    // 关联卡牌数据模型与视图，便于快速查找和更新卡牌视图

    std::map<CardModel*, CardView*>& _cardViewMap;


    // 管理游戏操作的撤销记录，执行卡牌移动的回退逻辑
    UndoManager* _undoManager;

    // 功能：根据卡牌Y坐标判断是否归属桌面区域
    bool isDeskCard(CardModel* card);

    // 功能：根据卡牌Y坐标判断是否归属手牌区域

    bool isHandCard(CardModel* card);

    // 判断两张卡牌是否相差1
    // 功能：校验卡牌匹配规则，用于桌面牌点击的合法性判断
    //  card1 - 第一张卡牌模型指针；
    //  card2 - 第二张卡牌模型指针。

    bool isRankDiffOne(CardModel* card1, CardModel* card2);
};