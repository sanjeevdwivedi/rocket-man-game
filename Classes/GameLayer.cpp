//
//  GameLayer.cpp
//  TweeJumpCocos2dx
//
//  Modified by Sanjeev Dwivedi, Dale Stammen and Eric Mitelette
//  Created by Carlos Pinan on 13/10/13.
//
//

#include "GameLayer.h"
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

GameLayer::GameLayer()
{
    CCSpriteBatchNode* batchNode = dynamic_cast<CCSpriteBatchNode*>(getChildByTag(kSpriteManager));

    // Load the rocketman sprite and call the startGame function which simply draws the rocketman in the middle of the screen
    CCSprite* rocketman = CCSprite::createWithTexture(batchNode->getTexture(), CCRectMake(608, 16, 50, 60));
    batchNode->addChild(rocketman, 4, kRocketMan);

    _startGame();

#if K_PLAY_BACKGROUND_MUSIC
    // play and loop background music during game
    auto soundEngine = CocosDenshion::SimpleAudioEngine::sharedEngine();
    soundEngine->playBackgroundMusic("Sounds/background.wav", true);
    soundEngine->setBackgroundMusicVolume(K_PLAY_BACKGROUND_MUSIC_VOLUME);
#endif
}

void GameLayer::_startGame()
{
    _resetRocketMan();
}

// RocketMan logic
void GameLayer::_resetRocketMan()
{
    CCSpriteBatchNode* batchNode = dynamic_cast<CCSpriteBatchNode*>(getChildByTag(kSpriteManager));
    CCSprite* rocketMan = dynamic_cast<CCSprite*>(batchNode->getChildByTag(kRocketMan));

    rm_position.x = SCREEN_WIDTH * 0.5f;
    rm_position.y = SCREEN_WIDTH * 0.5f;
    rocketMan->setPosition(rm_position);
}


void GameLayer::update(float dt)
{
}

// When we exit the layer, stop all background music and any other audio effects being played
GameLayer::~GameLayer()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
}