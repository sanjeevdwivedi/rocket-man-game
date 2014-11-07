//
//  MainLayer.cpp
//  TweeJumpCocos2dx
//
//  Created by Carlos Pinan on 13/10/13.
//
//

#include "MainLayer.h"

using namespace cocos2d;

CCScene* MainLayer::scene()
{
    CCScene* scene = CCScene::create();
    MainLayer* layer = new MainLayer();
    scene->addChild(layer);
    layer->autorelease();
    return scene;
}

MainLayer::MainLayer()
{
    RANDOM_SEED();
    
    CCSpriteBatchNode* batchNode = CCSpriteBatchNode::create("sprites.png", 10);
    batchNode->setAnchorPoint(ccp(0,0));
    addChild(batchNode, -1, kSpriteManager);
    
    CCSprite* background = CCSprite::createWithTexture(batchNode->getTexture(), CCRectMake(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
    batchNode->addChild(background);
    background->setPosition(ccp(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f));
}



void MainLayer::update(float dt)
{
}



