#include "CardModel.h"

// CardModel类实现
// 功能：卡牌数据模型类，封装卡牌的花色、点数、位置等核心数据，提供卡牌图片路径的生成逻辑
// 职责：初始化卡牌的花色、点数、初始位置，维护卡牌当前位置的状态；根据花色生成对应的花色图片路径；根据花色（颜色）和点数生成对应的点数图片路径；提供只读接口，保证卡牌数据的封装性
// 使用场景：由GameModel创建，作为单张卡牌的数据载体;为CardView提供图片路径;存储卡牌的位置信息，用于卡牌移动、撤销等逻辑的状态管理

// 构造函数
// 功能：初始化CardModel，设置花色、点数、初始位置，初始化当前位置为初始位置
CardModel::CardModel(CardSuit suit, CardRank rank, const cocos2d::Vec2& initPos)
    : _suit(suit), _rank(rank), _initPos(initPos), _currentPos(initPos) {
}

// 功能：根据卡牌花色，返回对应的花色图片资源路径
std::string CardModel::getSuitImgPath() const {
    switch (_suit) {
    case CardSuit::CLUB: return "card/club.png";
    case CardSuit::HEART: return "card/heart.png";
    case CardSuit::DIAMOND: return "card/diamond.png";
    case CardSuit::SPADE: return "card/spade.png";
    default: return "";
    }
}

// 功能：根据卡牌花色和点数，返回对应的点数图片资源路径
std::string CardModel::getRankImgPath() const {
    // 根据花色确定点数图片颜色（红桃/方块为红色，梅花/黑桃为黑色）
    std::string color = (_suit == CardSuit::HEART || _suit == CardSuit::DIAMOND) ? "red" : "black";
    // 将点数枚举转换为对应的字符串
    std::string rankStr;
    switch (_rank) {
    case CardRank::A: rankStr = "A"; break;
    case CardRank::J: rankStr = "J"; break;
    case CardRank::Q: rankStr = "Q"; break;
    case CardRank::K: rankStr = "K"; break;
    default: rankStr = std::to_string(static_cast<int>(_rank));
    }
    return "card/big_" + color + "_" + rankStr + ".png";
}