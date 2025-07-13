#include "Card.h"
#include "cocos2d.h"
#include <sstream>

USING_NS_CC;

bool Card::init() {
	if (!Node::init()) return false;
	setContentSize(Size(100, 150));
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](Touch* touch, Event* event) {
		Vec2 location = convertToNodeSpace(touch->getLocation());
		Rect rect = Rect(0, 0, getContentSize().width, getContentSize().height);

		if (rect.containsPoint(location)) {
			// 添加选中效果
			setScale(1.05f);

			// 触发回调
			if (_clickCallback) {
				_clickCallback(this);
			}
			return true;
		}
		return false;
		};

	listener->onTouchEnded = [this](Touch* touch, Event* event) {
		setScale(1.0f); // 恢复原始大小
		};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void Card::initWithValue(CardFaceType face, CardSuitType suit) {
	_face = face;
	_suit = suit;
	//加载牌底
	auto cardBg = Sprite::create("res/card_general.png");
	cardBg->setPosition(getContentSize() / 2);
	addChild(cardBg);
	
	//加载居中显示的数字
	std::string bigValueTexture = getFaceTextureName(face, false);
	auto bigValueSprite = Sprite::create(bigValueTexture);
	bigValueSprite->setPosition(getContentSize() / 2);
	bigValueSprite->setScale(0.8f);
	addChild(bigValueSprite);

	//加载左上角的小数字
	std::string smallValueTexture = getFaceTextureName(face, true);
    auto smallValueSprite = Sprite::create(smallValueTexture);
	smallValueSprite->setPosition(5, 170);
	smallValueSprite->setScale(0.4f);
	addChild(smallValueSprite);

	//加载右上角的花色
	std::string suitTexture = getSuitTextureName(suit);
	auto suitSprite = cocos2d::Sprite::create(suitTexture);
	if (suitSprite == nullptr) {
		CCLOG("Failed to create suitSprite with texture: %s", suitTexture.c_str());
	}
	else {
		suitSprite->setPosition(95, 170);
		suitSprite->setScale(0.4f);
		addChild(suitSprite);
	}
	
}

void Card::setClickCallback(const CardClickCallback& callback) {
	_clickCallback = callback;
}

std::string Card::getSuitTextureName(CardSuitType suit) {
	switch (suit) {
	case CST_CLUBS:    return "res/suits/club.png";
	case CST_DIAMONDS: return "res/suits/suit_diamond.png";
	case CST_HEARTS:   return "res/suits/suit_heart.png";
	case CST_SPADES:   return "res/suits/suit_spade.png";
	default: return "res/suits/suit_club.png";
	}
}

std::string Card::getFaceTextureName(CardFaceType face, bool isSmall) {
	std::string faceStr;
	switch (face) {
	case CFT_ACE:   faceStr = "A"; break;
	case CFT_TWO:   faceStr = "2"; break;
	case CFT_THREE: faceStr = "3"; break;
	case CFT_FOUR:  faceStr = "4"; break;
	case CFT_FIVE:  faceStr = "5"; break;
	case CFT_SIX:   faceStr = "6"; break;
	case CFT_SEVEN: faceStr = "7"; break;
	case CFT_EIGHT: faceStr = "8"; break;
	case CFT_NINE:  faceStr = "9"; break;
	case CFT_TEN:   faceStr = "10"; break;
	case CFT_JACK:  faceStr = "J"; break;
	case CFT_QUEEN: faceStr = "Q"; break;
	case CFT_KING:  faceStr = "K"; break;
	default:        faceStr = "2"; break;
	}

	std::string sizePrefix = isSmall ? "small_" : "big_";
	std::string color = isRedSuit(_suit) ? "red_" : "black_";

	// 使用 stringstream 拼接字符串
	std::stringstream ss;
	ss << "res/number/" << sizePrefix << color << faceStr << ".png";  // 依次拼接各部分
	return ss.str();  // 转换为 string 并返回
}

bool Card::isRedSuit(CardSuitType suit) {
	return suit == CST_HEARTS || suit == CST_DIAMONDS;
}

bool Card::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
	if (getBoundingBox().containsPoint(touch->getLocation())) {
		// 触发点击回调
		if (_clickCallback) {
			_clickCallback(this);
		}
		return true;
	}
	return false;
}