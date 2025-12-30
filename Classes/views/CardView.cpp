#include "CardView.h"
#include "../models/GameModel.h"

USING_NS_CC;

// CardView类
// 功能：渲染卡牌视图，处理卡牌触摸、移动动画，同步模型位置
// 职责：创建卡牌UI、响应交互、同步卡牌位置状态
// 使用场景：展示单张卡牌，支撑卡牌移动、重置位置等视觉交互
CardView* CardView::create(CardModel* model) {
    CardView* view = new CardView();
    if (view && view->init(model)) {
        view->autorelease();
        return view;
    }
    delete view;

    view = new CardView();
    view->initWithColor(Color4B(255, 255, 255, 255), 181, 280);
    view->autorelease();
    return view;
}

// 功能：创建卡牌UI，绑定数据模型，设置初始位置和样式
bool CardView::init(CardModel* model) {
    if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255), 181, 280)) {
        return false;
    }

    this->setTouchEnabled(true);
    this->setAnchorPoint(Vec2(0.5f, 0.5f));
    _model = model;

    // 初始位置
    Vec2 initPos = (model) ? model->getCurrentPos() : Vec2(0, 0);
    this->setPosition(initPos);

    // 底牌 Sprite
    auto cardSprite = Sprite::create("card/card_general.png");
    if (cardSprite) {
        cardSprite->setPosition(this->getContentSize() / 2);
        this->addChild(cardSprite, 0);

        // 花色和数字
        if (model) {
            std::string suitStr;
            switch (model->getSuit()) {
            case CardSuit::HEART: suitStr = "heart"; break;
            case CardSuit::DIAMOND: suitStr = "diamond"; break;
            case CardSuit::CLUB: suitStr = "club"; break;
            case CardSuit::SPADE: suitStr = "spade"; break;
            }

            int rankVal = model->getRankValue();
            std::string rankStr;
            if (rankVal == 1) rankStr = "A";
            else if (rankVal >= 2 && rankVal <= 10) rankStr = std::to_string(rankVal);
            else if (rankVal == 11) rankStr = "J";
            else if (rankVal == 12) rankStr = "Q";
            else if (rankVal == 13) rankStr = "K";

            std::string colorStr = (suitStr == "heart" || suitStr == "diamond") ? "red" : "black";

            // 花色 Sprite
            std::string suitFile = "card/" + suitStr + ".png";
            auto suitSprite = Sprite::create(suitFile);
            if (suitSprite) {
                suitSprite->setPosition(Vec2(60, 320));
                cardSprite->addChild(suitSprite, 1);
            }

            // 数字 Sprite
            std::string rankFile = "card/big_" + colorStr + "_" + rankStr + ".png";
            auto rankSprite = Sprite::create(rankFile);
            if (rankSprite) {
                rankSprite->setPosition(Vec2(125, 190));
                cardSprite->addChild(rankSprite, 1);
            }
        }
    }
    this->setScale(0.7f);

    return true;
}

// 功能：执行卡牌移动动画，同步更新数据模型位置
void CardView::moveTo(const Vec2& targetPos, float duration) {
    this->stopAllActions();
    // 移动动画
    auto move = MoveTo::create(duration, targetPos);
    this->runAction(move);
    CCLOG("CardView: Move the card to the target position：(%f,%f)", targetPos.x, targetPos.y);

    // 同步模型位置
    if (_model) {
        _model->setPosition(targetPos);
    }
}

// 功能：将卡牌移回初始位置，同步更新数据模型
void CardView::resetToInitPos() {
    if (_model) {
        Vec2 initPos = _model->getInitPos();
        this->moveTo(initPos);
        _model->setPosition(initPos);
        CCLOG("CardView: Reset the cards to their original positions：(%f,%f)", initPos.x, initPos.y);
    }
}