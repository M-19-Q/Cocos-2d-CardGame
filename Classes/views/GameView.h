#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "../models/GameModel.h"
#include "../models/CardModel.h"
#include "CardView.h"
#include <map>

class GameController;

USING_NS_CC;

// GameView类
// 功能：游戏界面统一管理类，负责所有UI元素的创建、维护与资源清理
// 职责：初始化游戏背景、手牌区、卡牌视图和撤销按钮，维护卡牌视图与模型的映射关系，承接控制器指令
// 使用场景：游戏启动时初始化界面，运行中为控制器提供视图层数据访问能力，游戏结束时清理界面资源
class GameView {
public:
    // 构造函数
    // 功能：初始化视图核心依赖对象
    GameView(cocos2d::Layer* layer, GameModel* model);

    // 析构函数
    // 功能：清理所有视图资源，释放UI元素内存
    ~GameView();

    // 初始化游戏所有UI元素
    // 功能：依次创建背景、手牌区、卡牌视图和撤销按钮，绑定按钮交互事件
    void initView();

    // 获取卡牌视图映射表
    std::map<CardModel*, CardView*>& getCardViewMap() { return _cardViewMap; }

    // 获取撤销按钮对象
    cocos2d::ui::Button* getUndoBtn() const { return _undoBtn; }

    // 绑定视图与控制器，使视图可响应控制器的交互逻辑
    void setController(GameController* controller) {
        _controller = controller;
    }

private:
    // 游戏桌面区域宽度（适配1080×2080设计分辨率）
    const int TABLE_AREA_WIDTH = 1080;
    // 游戏桌面区域高度（适配1080×2080设计分辨率）
    const int TABLE_AREA_HEIGHT = 1500;
    // 手牌区宽度（适配1080×2080设计分辨率）
    const int HAND_AREA_WIDTH = 1080;
    // 手牌区高度（适配1080×2080设计分辨率）
    const int HAND_AREA_HEIGHT = 580;

    // 承载所有UI元素的核心层对象，视图所有元素均添加至此层
    cocos2d::Layer* _layer;
    // 游戏数据模型指针，为视图提供卡牌数据来源
    GameModel* _model;
    // 卡牌模型与视图的映射表，维护两者的绑定关系
    std::map<CardModel*, CardView*> _cardViewMap;
    // 撤销按钮指针，用于触发撤销操作的交互控件
    cocos2d::ui::Button* _undoBtn;
    // 游戏控制器指针，绑定视图与业务逻辑层的交互通道
    GameController* _controller;
};

#endif // GAME_VIEW_H