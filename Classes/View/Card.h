#ifndef CARD_H
#define CARD_H

#include "cocos2d.h"

enum CardSuitType
{
    CST_NONE = -1,
    CST_CLUBS,      // 梅花
    CST_DIAMONDS,   // 方块
    CST_HEARTS,     // 红桃
    CST_SPADES,     // 黑桃
    CST_NUM_CARD_SUIT_TYPES
};

// 正面类型
enum CardFaceType
{
    CFT_NONE = -1,
    CFT_ACE = 1,    // 显式指定值，与数值对应
    CFT_TWO,
    CFT_THREE,
    CFT_FOUR,
    CFT_FIVE,
    CFT_SIX,
    CFT_SEVEN,
    CFT_EIGHT,
    CFT_NINE,
    CFT_TEN,
    CFT_JACK,
    CFT_QUEEN,
    CFT_KING,
    CFT_NUM_CARD_FACE_TYPES
};


class Card : public cocos2d::Node {
public:
	CREATE_FUNC(Card);
	bool init() override;
	void initWithValue(CardFaceType face, CardSuitType suit);
    // 添加回调函数类型和设置方法
    using CardClickCallback = std::function<void(Card* card)>;
    void setClickCallback(const CardClickCallback& callback);
   
    CardFaceType getFace() const { return _face; }
    CardSuitType getSuit() const { return _suit; }
private:
    CardFaceType _face;
    CardSuitType _suit;
    CardClickCallback _clickCallback;
	std::string getSuitTextureName(CardSuitType suit);
	std::string getFaceTextureName(CardFaceType face, bool isSmall = false);
    bool isRedSuit(CardSuitType suit);
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
};

#endif