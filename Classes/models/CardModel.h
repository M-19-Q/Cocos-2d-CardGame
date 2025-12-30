#ifndef CARD_MODEL_H
#define CARD_MODEL_H

#include "cocos2d.h"
#include <string>

// 功能：定义卡牌的四种花色类型，区分花色对应的颜色属性
enum class CardSuit {
    CLUB,    // 梅花（黑）
    HEART,   // 红桃（红）
    DIAMOND, // 方块（红）
    SPADE    // 黑桃（黑）
};


// 功能：定义卡牌的点数类型，实际卡牌的点数值（1=A，11=J，12=Q，13=K）
enum class CardRank {
    A = 1,
    _2 = 2,
    _3 = 3,
    _4 = 4,
    _5 = 5,
    _6 = 6,
    _7 = 7,
    _8 = 8,
    _9 = 9,
    _10 = 10,
    J = 11,
    Q = 12,
    K = 13
};

// CardModel类
// 功能：卡牌数据模型
// 职责：存储卡牌的花色、点数、初始位置、当前位置等核心状态；提供只读接口获取卡牌基础属性（花色、点数、点数数值）；提供位置管理接口，更新/获取卡牌的当前位置、初始位置；生成花色、点数对应的图片资源路径，支撑卡牌视图渲染
// 使用场景：由GameModel创建，作为卡牌数据的载体，存储单张卡牌的全量状态；为CardView提供图片路径和位置信息;支撑撤销操作，通过初始位置恢复卡牌到移动前状态

class CardModel {
public:
    // 构造函数
    // 功能：初始化CardModel，设置花色、点数、初始位置，初始化当前位置为初始位置
    CardModel(CardSuit suit, CardRank rank, const cocos2d::Vec2& initPos);

    // 功能：获取卡牌花色

    CardSuit getSuit() const { return _suit; }

    // 功能：获取卡牌点数
    CardRank getRank() const { return _rank; }

    // 功能：获取卡牌点数的数值
    int getRankValue() const { return static_cast<int>(_rank); }

    // 功能：更新卡牌的当前位置，用于卡牌移动逻辑
    void setPosition(const cocos2d::Vec2& pos) { _currentPos = pos; }

    // 功能：返回卡牌的当前位置
    cocos2d::Vec2 getCurrentPos() const { return _currentPos; }

    // 功能：获取卡牌初始位置（用于撤销操作恢复位置）
    cocos2d::Vec2 getInitPos() const { return _initPos; }

    // 功能：根据卡牌花色，返回对应的花色图片资源路径
    std::string getSuitImgPath() const;

    // 功能：根据卡牌花色（颜色）和点数，返回对应的点数图片资源路径
    std::string getRankImgPath() const;

private:
    // 用途：存储卡牌的花色类型，用于判断颜色、生成花色图片路径
    CardSuit _suit;

    // 用途：存储卡牌的点数类型，用于生成点数图片路径、判断点数匹配规则

    CardRank _rank;

    // 用途：存储卡牌的初始位置，用于撤销操作时恢复卡牌到移动前状态
    cocos2d::Vec2 _initPos;

    // 用途：存储卡牌的实时位置，用于卡牌移动、碰撞检测、视图渲染
    cocos2d::Vec2 _currentPos;
};

#endif // CARD_MODEL_H