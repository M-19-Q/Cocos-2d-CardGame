#ifndef CARD_VIEW_H
#define CARD_VIEW_H

#include "cocos2d.h"
#include "../models/CardModel.h"

// CardView类
// 功能：渲染卡牌视图，提供卡牌移动、重置位置功能
// 职责：创建卡牌UI、执行移动动画、同步模型位置
// 使用场景：展示单张卡牌，支撑卡牌交互的视觉表现


class CardView : public cocos2d::LayerColor {
public:
    // 静态创建函数（兜底：即使model为空也返回默认视图）
    static CardView* create(CardModel* model);

    virtual bool init(CardModel* model);

    // 移动卡牌
    void moveTo(const cocos2d::Vec2& targetPos, float duration = 0.2f);
    void resetToInitPos();

    // 获取绑定的模型
    CardModel* getModel() const { return _model; }

private:
    CardModel* _model;
    cocos2d::Label* _cardLabel;
};

#endif // CARD_VIEW_H