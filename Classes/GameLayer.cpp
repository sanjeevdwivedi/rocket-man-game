//
//  GameLayer.cpp
//  TweeJumpCocos2dx
//
//  Created by Carlos Pinan on 13/10/13.
//
//

#include "GameLayer.h"
#include "HighScoreLayer.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;

CCScene* GameLayer::scene()
{
    CCScene* scene = CCScene::create();
    GameLayer* gameLayer = new GameLayer();
    gameLayer->autorelease();
    scene->addChild(gameLayer);
    return scene;
}


// TODO: Does the game create all the levels in the beginning itself?

GameLayer::GameLayer()
{
    CCSpriteBatchNode* batchNode = dynamic_cast<CCSpriteBatchNode*>(getChildByTag(kSpriteManager));
    
    // We don't need a packed sprite, we can pick individual textures. Here we are getting the sprite for RocketMan
    //CCSprite* bird = CCSprite::createWithTexture(batchNode->getTexture(), CCRectMake(608, 16, 44, 32));
	//EMchange: the size of the sprite, 50*60
	CCSprite* rocketman = CCSprite::createWithTexture(batchNode->getTexture(), CCRectMake(608, 16, 50, 60));
    batchNode->addChild(rocketman, 4, kRocketMan);
    
    _startGame();
    scheduleUpdate();
    
    // We don't want touch interaction
    setTouchEnabled(false);

}

GameLayer::~GameLayer()
{
}


void GameLayer::update(float dt)
{
    // MainLayer shows the background with clouds that does just scrolls but does not interact
    MainLayer::update(dt);
}


// RocketMan logic
void GameLayer::_resetRocketMan()
{
    CCSpriteBatchNode* batchNode = dynamic_cast<CCSpriteBatchNode*>(getChildByTag(kSpriteManager));
    CCSprite* rocketMan = dynamic_cast<CCSprite*>(batchNode->getChildByTag(kRocketMan));

    // draw RocketMan at the center of the screen
    CCPoint position(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    rocketMan->setPosition(position);
    rocketMan->setScaleX(1.0f);
}

void GameLayer::_startGame()
{
    _resetRocketMan();
}



