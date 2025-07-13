#ifndef CARD_CONTROLLER_H
#define CARD_CONTROLLER_H

#include "json/document.h"
#include "cocos2d.h"
#include "Card.h"
#include <vector>
#include <stack>

// 记录交换信息的结构体
struct SwapInfo {
    Card* card1;
    Card* card2;
    cocos2d::Vec2 pos1;
    cocos2d::Vec2 pos2;
};

class CardController {
public:
    static CardController* getInstance();

    // 初始化控制器
    void init(cocos2d::Node* parentNode);

    // 处理卡牌点击事件
    void handleCardClick(Card* clickedCard);

    void loadCardsFromJson(const std::string& jsonData);

    // 获取卡牌列表
    const std::vector<Card*>& getCards() const;
    const std::vector<Card*>& getTopCards() const;

    // 回退功能
    void undo();

private:
    static CardController* instance;
    cocos2d::Node* _parentNode;      // 卡牌的父节点
    std::vector<Card*> _cards;       // 下方卡牌列表
    std::vector<Card*> _topCards;    // 上方卡牌列表
    std::stack<SwapInfo> _swapStack; // 记录交换信息的栈
};

#endif
