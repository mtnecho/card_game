#include "CardController.h"
#include "cocos2d.h"
#include "json/document.h"

USING_NS_CC; 


CardController* CardController::instance = nullptr;

CardController* CardController::getInstance() {
    if (!instance) {
        instance = new CardController();
    }
    return instance;
}

void CardController::init(cocos2d::Node* parentNode) {
    _parentNode = parentNode;
    _cards.clear();
    _topCards.clear();
    while (!_swapStack.empty()) {
        _swapStack.pop();
    }
}

void CardController::loadCardsFromJson(const std::string& jsonData) {
    rapidjson::Document doc;
    doc.Parse(jsonData.c_str());

    if (doc.HasParseError()) {
        CCLOG("JSON parse error");
        return;
    }

    _cards.clear();
    _topCards.clear();

    auto visibleSize = Director::getInstance()->getVisibleSize();
    const float Y_OFFSET = 200.0f;

    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray()) {
        const rapidjson::Value& playfield = doc["Playfield"];
        for (rapidjson::SizeType i = 0; i < playfield.Size(); i++) {
            const rapidjson::Value& cardData = playfield[i];

            CardFaceType face = static_cast<CardFaceType>(cardData["CardFace"].GetInt());
            CardSuitType suit = static_cast<CardSuitType>(cardData["CardSuit"].GetInt());

            Card* card = Card::create();
            if (card) {
                card->initWithValue(face, suit);
                card->setClickCallback([this](Card* card) { handleCardClick(card); });

                float x = cardData["Position"]["x"].GetFloat();
                float y = cardData["Position"]["y"].GetFloat();
                card->setPosition(x, y);

                _parentNode->addChild(card);
                _topCards.push_back(card);
            }
        }
    }

    if (doc.HasMember("Stack") && doc["Stack"].IsArray()) {
        const rapidjson::Value& stack = doc["Stack"];

        const float CARD_WIDTH = 100;
        const float START_X = Director::getInstance()->getVisibleSize().width * 0.15;
        const float BOTTOM_Y = Director::getInstance()->getVisibleSize().height * 0.3;

        for (rapidjson::SizeType i = 0; i < stack.Size(); i++) {
            const rapidjson::Value& cardData = stack[i];

            CardFaceType face = static_cast<CardFaceType>(cardData["CardFace"].GetInt());
            CardSuitType suit = static_cast<CardSuitType>(cardData["CardSuit"].GetInt());

            Card* card = Card::create();
            if (card) {
                card->initWithValue(face, suit);
                card->setClickCallback([this](Card* card) { handleCardClick(card); });

                float x = cardData["Position"]["x"].GetFloat();
                float y = cardData["Position"]["y"].GetFloat();

                if (x == 0 && y == 0) {
                    x = START_X + i * (CARD_WIDTH + 10);
                    y = BOTTOM_Y;
                }

                card->setPosition(x, y);

                _parentNode->addChild(card);
                _cards.push_back(card);
            }
        }
    }
}

void CardController::handleCardClick(Card* clickedCard) {
    bool isTopCard = std::find(_topCards.begin(), _topCards.end(), clickedCard) != _topCards.end();

    if (isTopCard) {
        if (_cards.empty()) return;
        Card* rightmostCard = _cards.back();
        int faceDiff = std::abs(clickedCard->getFace() - rightmostCard->getFace());

        CCLOG("Top card clicked: face=%d, rightmost face=%d, diff=%d",
            clickedCard->getFace(), rightmostCard->getFace(), faceDiff);

        if (faceDiff == 1) {
            // 记录交换前的状态
            SwapInfo info;
            info.card1 = clickedCard;
            info.card2 = rightmostCard;
            info.pos1 = clickedCard->getPosition();
            info.pos2 = rightmostCard->getPosition();
            _swapStack.push(info);

            // 执行交换
            clickedCard->runAction(MoveTo::create(0.3f, info.pos2));
            rightmostCard->runAction(MoveTo::create(0.3f, info.pos1));

            // 更新卡牌列表顺序
            auto topIt = std::find(_topCards.begin(), _topCards.end(), clickedCard);
            if (topIt != _topCards.end()) {
                *topIt = rightmostCard;
            }
            _cards.back() = clickedCard;

            CCLOG("Swapping top card with bottom rightmost card");
        }
        else {
            CCLOG("No swap: face difference is not 1");
        }
    }
    else {
        // 下方卡牌逻辑（与最右侧卡牌交换）
        if (_cards.empty() || _cards.size() <= 1) return;

        auto it = std::find(_cards.begin(), _cards.end(), clickedCard);
        if (it == _cards.end()) return;

        Card* rightmostCard = _cards.back();
        if (clickedCard == rightmostCard) {
            CCLOG("Clicked card is already rightmost in stack");
            return;
        }

        // 记录交换前的状态
        SwapInfo info;
        info.card1 = clickedCard;
        info.card2 = rightmostCard;
        info.pos1 = clickedCard->getPosition();
        info.pos2 = rightmostCard->getPosition();
        _swapStack.push(info);

        // 执行交换
        clickedCard->runAction(MoveTo::create(0.3f, info.pos2));
        rightmostCard->runAction(MoveTo::create(0.3f, info.pos1));

        // 更新卡牌列表顺序
        *it = rightmostCard;
        _cards.back() = clickedCard;

        CCLOG("Swapping bottom card with rightmost card");
    }
}

const std::vector<Card*>& CardController::getCards() const {
    return _cards;
}

const std::vector<Card*>& CardController::getTopCards() const {
    return _topCards;
}

void CardController::undo() {
    if (_swapStack.empty()) return;

    SwapInfo info = _swapStack.top();
    _swapStack.pop();

    auto moveBack1 = cocos2d::MoveTo::create(0.3f, info.pos1);
    auto moveBack2 = cocos2d::MoveTo::create(0.3f, info.pos2);

    info.card1->runAction(moveBack1);
    info.card2->runAction(moveBack2);

    // 如果是下方卡牌交换，还需要交换向量中的位置
    auto it1 = std::find(_cards.begin(), _cards.end(), info.card1);
    auto it2 = std::find(_cards.begin(), _cards.end(), info.card2);
    if (it1 != _cards.end() && it2 != _cards.end()) {
        size_t index1 = std::distance(_cards.begin(), it1);
        size_t index2 = std::distance(_cards.begin(), it2);
        std::swap(_cards[index1], _cards[index2]);
    }
}