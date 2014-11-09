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

    // RocketMan's acceleration, up and and down
    rm_velocity.y = rm_velocity.y * accel_filter + pAccelerationValue->y * (1.0f - accel_filter) * 500.0f;
}

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
    
    // MainLayer shows the background 
    MainLayer::update(dt);
    
    CCSpriteBatchNode* batchNode = dynamic_cast<CCSpriteBatchNode*>(getChildByTag(kSpriteManager));
    CCSprite* rocketMan = dynamic_cast<CCSprite*>(batchNode->getChildByTag(kRocketMan));
    
    rm_position.x += rm_velocity.x * dt;
    rm_position.y += rm_velocity.y * dt;

    // rm_lookingRight/Left is used to flip RocketMan in the right direction i.e. direction of the velocity
    // so RocketMan does not travel backwards
    if (rm_velocity.x < -30.0f && rm_lookingRight)
    {
        rm_lookingRight = false;

        // flip RocketMan on the x axis
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
    
    float max_y = SCREEN_HEIGHT + rm_size.height * 0.5f;
    float min_y = -rm_size.height * 0.5f;

    if (rm_position.y > max_y)
        rm_position.y = min_y;

    if (rm_position.y < min_y)
        rm_position.y = max_y;

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
    rm_acceleration.y = 0;
    
    rm_lookingRight = true;
    rocketMan->setScaleX(1.0f);
}



void GameLayer::_startGame()
{
    _resetRocketMan();
    gameSuspended = false;
}




