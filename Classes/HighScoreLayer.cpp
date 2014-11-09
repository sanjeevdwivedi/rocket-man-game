//
//  HighScoreLayer.cpp
//  TweeJumpCocos2dx
//
//  Created by Carlos Pinan on 13/10/13.
//
//

#include "HighScoreLayer.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace extension;
using namespace std;

CCScene* HighScoreLayer::scene(int lastScore)
{
    CCScene* scene = CCScene::create();
    HighScoreLayer* layer = new HighScoreLayer(lastScore);
    scene->addChild(layer);
    layer->autorelease();
    return scene;
}


HighScoreLayer::HighScoreLayer(int lastScore) : currentScore(lastScore)
{
    CCSpriteBatchNode* batchNode = dynamic_cast<CCSpriteBatchNode*>(getChildByTag(kSpriteManager));
    CCSprite* title = CCSprite::createWithTexture(batchNode->getTexture(), CCRectMake(608, 192, 225, 57));
    batchNode->addChild(title, 5);
    title->setPosition(ccp(160, 420));
    

    //
    
    CCMenuItemImage* button1 = CCMenuItemImage::create("playAgainButton.png", "playAgainButton.png", this, menu_selector(HighScoreLayer::button1Callback));
    
    CCMenu* menu = CCMenu::create(button1, NULL);
    menu->alignItemsVerticallyWithPadding(9.0f);
    menu->setPosition(ccp(160, 58));
    addChild(menu);
}

void HighScoreLayer::draw()
{
    MainLayer::draw();
}



void HighScoreLayer::button1Callback(CCObject *pSender)
{
    CCTransitionFade* scene = CCTransitionFade::create(0.5f, HighScoreLayer::scene(0), ccWHITE);
    CCDirector::sharedDirector()->replaceScene(scene);
}



