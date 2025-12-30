#pragma once
#include "cocos2d.h"
#include "views/CardView.h"
#include "managers/UndoManager.h"
#include "models/GameModel.h"

USING_NS_CC;

// 功能：游戏的核心场景
// 职责：初始化游戏数据模型、撤销管理器、卡牌视图容器;卡牌点击事件回调接口;管理动态资源的内存分配与释放
// 使用场景：游戏的视图渲染、数据管理、交互逻辑的初始化整合;游戏场景管理

class GameScene : public cocos2d::Scene {
public:
    // 创建游戏场景
    // 功能：创建并返回GameScene场景

    static cocos2d::Scene* createScene();

    // 功能：初始化游戏场景
    virtual bool init();

    // 自动生成create方法
    CREATE_FUNC(GameScene);

    // 构造函数
    // 功能：初始化GameScene私有成员，创建GameModel和UndoManager
    GameScene();

    // 析构函数
    // 功能：释放所有动态分配的资源（GameModel、UndoManager、卡牌视图）

    ~GameScene() override;

private:
    // 顶部卡牌视图指针
    // 记录顶部牌，用于卡牌交互逻辑
    CardView* _handTop;


    // 桌面卡牌视图容器
    // 存储所有桌面牌视图
    std::vector<CardView*> _tableCards;

   // 撤销管理器指针
   // 管理游戏操作的撤销逻辑，关联GameModel
    UndoManager* _undo;

    // 游戏数据模型指针
    // 存储卡牌状态、游戏规则
    GameModel* _gameModel;

    // 卡牌点击事件回调方法
    // 功能：预留卡牌点击交互的扩展接口
    void onCardClicked(CardView* card);
};