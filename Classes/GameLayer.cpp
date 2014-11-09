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

void GameLayer::didAccelerate(CCAcceleration *pAccelerationValue)
{
    if(gameSuspended)
        return;
    
    // RocketMan's acceleration, left and right
    float accel_filter = 0.1f;
    rm_velocity.x = rm_velocity.x * accel_filter + pAccelerationValue->x * (1.0f - accel_filter) * 500.0f;
}

// TODO: Does the game create all the levels in the beginning itself?

GameLayer::GameLayer()
{
    gameSuspended = true;
    isGameOver = false;
    CCSpriteBatchNode* batchNode = dynamic_cast<CCSpriteBatchNode*>(getChildByTag(kSpriteManager));
    
    // We don't need a packed sprite, we can pick individual textures. Here we are getting the sprite for RocketMan
    //CCSprite* bird = CCSprite::createWithTexture(batchNode->getTexture(), CCRectMake(608, 16, 44, 32));
	//EMchange: the size of the sprite, 50*60
	CCSprite* rocketman = CCSprite::createWithTexture(batchNode->getTexture(), CCRectMake(608, 16, 50, 60));
    batchNode->addChild(rocketman, 4, kRocketMan);

    CCSize landscapeSize = CCDirector::sharedDirector()->getVisibleSize();
    CCSize visibleSize = CCSize(landscapeSize.width, landscapeSize.height);
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    _startGame();
    scheduleUpdate();
    
    // We don't want touch interaction
    setTouchEnabled(false);
    
    // enable the accelerometer
    setAccelerometerEnabled(true);
}

GameLayer::~GameLayer()
{
}


void GameLayer::update(float dt)
{
    if(gameSuspended)
        return;
    
    // MainLayer shows the background with clouds that does just scrolls but does not interact
    MainLayer::update(dt);
    
    CCSpriteBatchNode* batchNode = dynamic_cast<CCSpriteBatchNode*>(getChildByTag(kSpriteManager));
    CCSprite* rocketMan = dynamic_cast<CCSprite*>(batchNode->getChildByTag(kRocketMan));
    
    rm_position.x += rm_velocity.x * dt;
    // rm_lookingRight/Left is used to flip RocketMan in the right direction i.e. direction of the velocity
    // so RocketMan does not travel backwards
    if (rm_velocity.x < -30.0f && rm_lookingRight)
    {
        rm_lookingRight = false;

        // what is the point of setting scaleX?
        rocketMan->setScaleX(-1.0f);
    }
    else if (rm_velocity.x > 30.0f && !rm_lookingRight)
    {
        rm_lookingRight = true;
        rocketMan->setScaleX(1.0f);
    }
    
    CCSize rm_size = rocketMan->getContentSize();
    float max_x = SCREEN_WIDTH + rm_size.width * 0.5f;
    float min_x = -rm_size.width * 0.5f;
    
    if(rm_position.x > max_x)
        rm_position.x = min_x;
    
    if(rm_position.x < min_x)
        rm_position.x = max_x;
    
    // draw RocketMan at its new position
    rocketMan->setPosition(rm_position);
}


// RocketMan logic
void GameLayer::_resetRocketMan()
{
    CCSpriteBatchNode* batchNode = dynamic_cast<CCSpriteBatchNode*>(getChildByTag(kSpriteManager));
    CCSprite* rocketMan = dynamic_cast<CCSprite*>(batchNode->getChildByTag(kRocketMan));
    
    rm_position.x = SCREEN_WIDTH * 0.5f;
    rm_position.y = SCREEN_HEIGHT * 0.5f;
    rocketMan->setPosition(rm_position);
    
    rm_velocity.x = 0;
    rm_velocity.y = 0;
    
    rm_acceleration.x = 0;
    rm_acceleration.y = -550.0f;
    
    rm_lookingRight = true;
    rocketMan->setScaleX(1.0f);
}



void GameLayer::_startGame()
{
    _resetRocketMan();
    gameSuspended = false;
}




