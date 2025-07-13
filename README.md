文档目的
解释程序架构及未来开发可扩展方向

项目开发环境
开发环境：cocos2dx 3.17+visual studio 2022
项目需求
需求1：手牌区翻牌替换
点击手牌区♥A，♥A会平移（简单MoveTo）到手牌区的顶部牌（♣4）并替换它作为新的顶部牌
需求2：桌面牌和手牌区顶部牌匹配
点击桌面牌的♦️3，卡牌会和手牌区顶部的♣4进行匹配【桌面牌区的牌只要和手牌区顶部牌点数差1就可以匹配，无花色要求】，点击的桌面牌（♦️3）会平移到手牌区的顶部牌（♣4）并替换它作为新的手牌区的顶部牌
需求3：回退功能
场景：点击♦️3 -> 点击♥A -> 点击♠2 后；连续多次点击 回退按钮 ，各卡牌需要反着平移（简单MoveTo）到原位置；直到无回退记录可回退；

程序架构
代码结构：如下图，程序文件均位于src文件夹中，卡牌视图和主场景位于View文件夹下，逻辑控制代码位于Controller中，三者均被Classes包含。资源文件位于resource/res文件夹中 ![[Pasted image 20250714024259.png]]
代码实现功能
CardController
单实例：getInstance
初始化+状态重置：init
JSON数据解析+卡牌加载：loadCardsFromJson
卡牌点击处理+交换：handleCardClick
状态管理+回退：undo
数据访问接口：getCards / getTopCards
Card
初始化：init
卡牌属性初始化+资源加载（卡牌组合）：initWithValue
点击回调：setClickCallback
花色纹理名称获取：getSuitTextureName
牌面纹理名称获取：getFaceTextureName
花色判断：isRedSuit
HelloWorldScene
场景创建：createScene
场景初始化：init
初始化父类
获取屏幕信息
添加背景
卡牌初始化+卡牌加载
回退按钮创建+事件处理
可扩展
如何新加卡牌：在HelloWorld中的json块直接按顺序添加
如何添加新类型的回退功能：记录操作类型，是哪里的卡牌在进行交换，标记操作时的操作类型，扩展回退功能，根据不同操作类型执行不同回退逻辑
