#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "Card.h"
#include "CardController.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}


// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto topBackground = LayerColor::create(Color4B(0, 128, 255, 255));
    topBackground->setContentSize(Size(1080,1500));
    topBackground->setPosition(origin.x, origin.y + visibleSize.height * 0.4);
    this->addChild(topBackground, -1);

    auto bottomBackground = LayerColor::create(Color4B(0, 200, 0, 255));
    bottomBackground->setContentSize(Size(1080,580));
    bottomBackground->setPosition(origin.x, origin.y);
    this->addChild(bottomBackground, -1);

    auto cardController = CardController::getInstance();
    cardController->init(this);

    std::string jsonData = R"({
        "Playfield": [
            {"CardFace": 12, "CardSuit": 0, "Position": {"x": 250, "y": 1000}},
            {"CardFace": 2, "CardSuit": 0, "Position": {"x": 300, "y": 800}},
            {"CardFace": 2, "CardSuit": 1, "Position": {"x": 350, "y": 600}},
            {"CardFace": 2, "CardSuit": 0, "Position": {"x": 850, "y": 1000}},
            {"CardFace": 2, "CardSuit": 0, "Position": {"x": 800, "y": 800}},
            {"CardFace": 1, "CardSuit": 3, "Position": {"x": 750, "y": 600}}
        ],
        "Stack": [
            {"CardFace": 2, "CardSuit": 0, "Position": {"x": 0, "y": 0}},
            {"CardFace": 0, "CardSuit": 2, "Position": {"x": 0, "y": 0}},
            {"CardFace": 3, "CardSuit": 0, "Position": {"x": 0, "y": 0}}
        ]
    })";
    cardController->loadCardsFromJson(jsonData);


    auto backButton = Label::createWithTTF("BACK", "fonts/arial.ttf", 24);
    backButton->setPosition(origin.x + visibleSize.width * 0.7, origin.y + visibleSize.height * 0.1);
    backButton->setColor(Color3B::WHITE);
    this->addChild(backButton, 1);

    auto backButtonListener = EventListenerTouchOneByOne::create();
    backButtonListener->setSwallowTouches(true);

    backButtonListener->onTouchBegan = [backButton](Touch* touch, Event* event) {
        Vec2 locationInNode = backButton->convertToNodeSpace(touch->getLocation());
        Rect rect = Rect(0, 0, backButton->getContentSize().width, backButton->getContentSize().height);

        if (rect.containsPoint(locationInNode)) {
            backButton->setColor(Color3B::YELLOW);
            return true;
        }
        return false;
        };

    backButtonListener->onTouchEnded = [backButton, cardController](Touch* touch, Event* event) {
        backButton->setColor(Color3B::WHITE);
        cardController->undo();
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(backButtonListener, backButton);
    return true;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
