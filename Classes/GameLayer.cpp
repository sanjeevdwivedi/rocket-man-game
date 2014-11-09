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
    fuelInTank = 3600; // one minute worth
    CCSpriteBatchNode* batchNode = dynamic_cast<CCSpriteBatchNode*>(getChildByTag(kSpriteManager));
    
    // Initialize all the platforms. Really? They predraw all the platforms??
    _initPlatforms();
    
    // We don't need a packed sprite, we can pick individual textures. Here we are getting the sprite for RocketMan
    //CCSprite* bird = CCSprite::createWithTexture(batchNode->getTexture(), CCRectMake(608, 16, 44, 32));
	//EMchange: the size of the sprite, 50*60
	CCSprite* rocketman = CCSprite::createWithTexture(batchNode->getTexture(), CCRectMake(608, 16, 50, 60));
    batchNode->addChild(rocketman, 4, kRocketMan);

    // add the exit arrow
    CCSprite* exit = CCSprite::create("exit_arrow.png");
    exit->setTag(kExit);
    exit->setVisible(false);
    addChild(exit);
    
    CCSize landscapeSize = CCDirector::sharedDirector()->getVisibleSize();
    CCSize visibleSize = CCSize(landscapeSize.width, landscapeSize.height);
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    pHealthSprite = CCSprite::create("health_bar.png");
    //pHealthSprite->setPosition(ccp(origin.x + 10, visibleSize.height - 20));
    pHealthSprite->setAnchorPoint(ccp(0.0, 0.0));
    //pHealthSprite->setScaleY(0.02);
    
    //pHealthSprite->setScaleX(100);
    
    
    pHealthBar = CCProgressTimer::create(pHealthSprite);
    pHealthBar->setScale(0.5, 0.5);
    pHealthBar->setType(kCCProgressTimerTypeBar);
    //pHealthBar->setMidpoint(ccp(0,0));
    pHealthBar->setBarChangeRate(ccp(1,0));
    pHealthBar->setPercentage(100);
    pHealthBar->setPosition(ccp(origin.x+10, visibleSize.height - 20));
    this->addChild(pHealthBar);
    
    CCSprite* bonus;
    
    // the bonus sprites which say 5, 10 etc can be picked from here
    for(int i = 0; i < kNumBonuses; i++)
    {
        bonus = CCSprite::createWithTexture(batchNode->getTexture(), CCRectMake(608 + i * 32, 256, 25, 25));
        batchNode->addChild(bonus, 4, kBonusStartTag + i);
        bonus->setVisible(false);
    }
    
    // TODO: read the sprite for the Exit door - add the tag "Exit to it"
    
    CCLabelBMFont* scoreLabel = CCLabelBMFont::create("0", "bitmapFont.fnt");
    addChild(scoreLabel, 5, kScoreLabel);
    scoreLabel->setPosition(ccp(160, 430));
    
    _startGame();
    scheduleUpdate();
    
    // We don't want touch interaction
    setTouchEnabled(false);
    
    setAccelerometerEnabled(true);
 
#if K_PLAY_BACKGROUND_MUSIC
    // play and loop background music during game
    auto soundEngine = CocosDenshion::SimpleAudioEngine::sharedEngine();
    soundEngine->playBackgroundMusic("Sounds/background.wav", true);
    soundEngine->setBackgroundMusicVolume(K_PLAY_BACKGROUND_MUSIC_VOLUME);
#endif
}

GameLayer::~GameLayer()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
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
    
    rm_velocity.y += rm_acceleration.y * dt;
    rm_position.y += rm_velocity.y * dt;
    
  
    // TODO: (fix this hack) - just set it so that every 20 frames, we decrease the percentage by 1
    // when the percentage goes below zero, the healthbar is finished and finish the game
    // We should show some animation that the health is over.
    fuelInTank--;
    if (fuelInTank%20 == 0)
    {
        pHealthBar->setPercentage(pHealthBar->getPercentage()-1.0);
        if (pHealthBar->getPercentage() <= 0)
        {
            _showHighScores();
        }
    }
    
    
    CCSprite* bonus = dynamic_cast<CCSprite*>(batchNode->getChildByTag(kBonusStartTag + currentBonusType));
    
    // check if the bonus node is visible
    if(bonus->isVisible())
    {
        // check if RocketMan and the bonus are colliding, if so, give RocketMan the bonus
        CCPoint bonus_position = bonus->getPosition();
        float range = 20.0f;
        if(rm_position.x > bonus_position.x - range &&
           rm_position.x < bonus_position.x + range &&
           rm_position.y > bonus_position.y - range &&
           rm_position.y < bonus_position.y + range)
        {
            // TODO: Our bonuses should be more rocket fuel or additional lives
            switch(currentBonusType)
            {
                case kBonus5:
                    score += 5000;
                    break;
                case kBonus10:
                    score += 10000;
                    break;
                case kBonus50:
                    score += 50000;
                    break;
                case kBonus100:
                    score += 100000;
                    break;
            }
            
            CCString* scoreStr = CCString::createWithFormat("%d", score);
            CCLabelBMFont* scoreLabel = dynamic_cast<CCLabelBMFont*>(getChildByTag(kScoreLabel));
            scoreLabel->setString(scoreStr->getCString());
            
            CCScaleTo* action1 = CCScaleTo::create(0.2f, 1.5f, 0.8f);
            CCScaleTo* action2 = CCScaleTo::create(0.2f, 1.0f, 1.0f);
            
            // What are CCScaleTo and CCSequence.. what do these actions do?
            // Likely, this just makes RocketMan move up very fast without it having to collide with anything
            CCSequence* action3 = CCSequence::create(action1, action2, action1, action2, action1, action2, NULL);
            scoreLabel->runAction(action3);

            // what does resetBonus do?
            _resetBonus();
            
            _superJump();
            
        }
    }
    
    int platformTag;
    
    // RocketMan collisions with platforms are detected only when RocketMan is falling down
    if(rm_velocity.y < 0)
    {
        for(platformTag = kPlatformsStartTag; platformTag < kPlatformsStartTag + K_NUM_PLATFORMS; platformTag++)
        {
            CCSprite* platform = dynamic_cast<CCSprite*>(batchNode->getChildByTag(platformTag));
            CCSize platform_size = platform->getContentSize();
            CCPoint platform_position = platform->getPosition();
            
            max_x = platform_position.x - platform_size.width * 0.5f - 10;
            min_x = platform_position.x + platform_size.width * 0.5f + 10;
            float min_y = platform_position.y + (platform_size.height + rm_size.height) * 0.5f - K_PLATFORM_TOP_PADDING;
            
            // check if RocketMan and the platform is colliding, if so, make the bird jump
            if(rm_position.x > max_x && rm_position.x < min_x &&
               rm_position.y > platform_position.y && rm_position.y < min_y)
                _jump();
        }
        
        // The game is endless in the original version.. when RocketMan falls down and is longer on the screen, call the game done and
        // show the high score screen
        if(rm_position.y < - rm_size.height)
            _showHighScores();
        
    }
    else if(rm_position.y > SCREEN_HEIGHT * 0.5f)
    {
        float delta = rm_position.y - SCREEN_HEIGHT * 0.5f;
        rm_position.y = SCREEN_HEIGHT * 0.5f;
        currentPlatformY -= delta;
        
        //for(cloudTag = kCloudsStartTag; cloudTag < kCloudsStartTag + K_NUM_CLOUDS; cloudTag++)
        //{
        //    CCSprite* cloud = dynamic_cast<CCSprite*>(batchNode->getChildByTag(cloudTag));
        //    CCPoint position = cloud->getPosition();
        //    position.y -= delta * cloud->getScaleY() * 0.8f;
        //    
        //    // assuming that the clouds in the background, when they have scrolled off the screen
        //    // reset them so that they will scroll in from the bottom
        //    if(position.y < -cloud->getContentSize().height * 0.5f)
        //    {
        //        currentCloudTag = cloudTag;
        //        resetCloud();
        //    }
        //    else
        //    {
        //        cloud->setPosition(position);
        //    }
        //}
        

        for(platformTag = kPlatformsStartTag; platformTag < kPlatformsStartTag + K_NUM_PLATFORMS; platformTag++)
        {
            CCSprite* platform = dynamic_cast<CCSprite*>(batchNode->getChildByTag(platformTag));
            CCPoint position = platform->getPosition();
            position = ccp(position.x, position.y - delta);
            
            // If the platform just became invisible, reset it to just above the screen
            if(position.y < -platform->getContentSize().height * 0.5f)
            {
                currentPlatformTag = platformTag;
                _resetPlatform();
            }
            else
            {
                // If the platform is still visible, decrease its Y coordinates so it looks like the scene is scrolling
                platform->setPosition(position);
            }
        }

        
        CCSprite* exit = dynamic_cast<CCSprite*>(getChildByTag(kExit));
        if(exit->isVisible())
        {
            CCPoint position = exit->getPosition();
            // check if RocketMan and the exit are colliding, if so, finish the game
            float range = 20.0f;
            if(rm_position.x > position.x - range &&
               rm_position.x < position.x + range &&
               rm_position.y > position.y - range &&
               rm_position.y < position.y + range)
            {
                // TODO: (would be nice to show an animation when exiting)
                _showHighScores();
            }
            
            position.y -= delta;
            if(position.y < -exit->getContentSize().height*0.5)
            {
              // the exit just passed out of the screen... you should show a "Game Over... you lose" kind of thing here
               // TODO: show an animation or something like... you lose!!
                _showHighScores();
            }
            
            exit->setPosition(position);
            
        }
        
        // if the bonus was visible and is going to become invisible, reset it.
        if(bonus->isVisible())
        {
            CCPoint position = bonus->getPosition();
            position.y -= delta;
            if(position.y < -bonus->getContentSize().height * 0.5f)
            {
                _resetBonus();
            }
            else
            {
                bonus->setPosition(position);
            }
        }
        
        score += (int) delta;
        CCString* scoreStr = CCString::createWithFormat("%d", score);
        CCLabelBMFont* scoreLabel = dynamic_cast<CCLabelBMFont*>(getChildByTag(kScoreLabel));
        scoreLabel->setString(scoreStr->getCString());
    }
    
    // draw RocketMan at its new position
    rocketMan->setPosition(rm_position);
}

void GameLayer::_initPlatform()
{
    CCRect rect;
    switch((int)(CCRANDOM_0_1() *2))
    {
        case 0:
            //rect = CCRectMake(608, 64, 102, 36);
			//EMChange: change the size and position of the beam
			rect = CCRectMake(608, 90, 102, 22);
            break;
        case 1:
            //rect = CCRectMake(608, 128, 90, 32);
			//EMChange: change the size and position of the sprite
			rect = CCRectMake(608, 140, 102, 22);
            break;

		default:
			return;
    }
    CCSpriteBatchNode* batchNode = dynamic_cast<CCSpriteBatchNode*>(getChildByTag(kSpriteManager));
    CCSprite* platform = CCSprite::createWithTexture(batchNode->getTexture(), rect);
    batchNode->addChild(platform, 3, currentPlatformTag);
}

void GameLayer::_initPlatforms()
{
    currentPlatformTag = kPlatformsStartTag;
    while(currentPlatformTag < kPlatformsStartTag + K_NUM_PLATFORMS)
    {
        _initPlatform();
        currentPlatformTag++;
    }
}

void GameLayer::_resetPlatform()
{
    if(platformCount >= K_MAX_PLATFORMS_IN_GAME) {
    
        isGameOver = true;
    }
    
    if(currentPlatformY < 0)
        currentPlatformY = 30.0f;
    else
    {
        currentPlatformY += CCRANDOM_0_1() * (int)(currentMaxPlatformStep - K_MIN_PLATFORM_STEP) + K_MIN_PLATFORM_STEP;
        if(currentMaxPlatformStep < K_MAX_PLATFORM_STEP)
            currentMaxPlatformStep += 0.5f;
    }
    
    CCSpriteBatchNode* batchNode = dynamic_cast<CCSpriteBatchNode*>(getChildByTag(kSpriteManager));
    CCSprite* platform = dynamic_cast<CCSprite*>(batchNode->getChildByTag(currentPlatformTag));
    
    if(CCRANDOM_0_1() * 2 == 1)
        platform->setScaleX(-1.0f);
    
    float x;
    
    CCSize size = platform->getContentSize();
    
    if(currentPlatformY == 30.0f)
        x = SCREEN_WIDTH * 0.5f;
    else
        x = CCRANDOM_0_1() * (SCREEN_WIDTH - (int) size.width) + size.width * 0.5f;
    
    platform->setPosition(ccp(x, currentPlatformY));
    platformCount++;
    
    if(platformCount == currentBonusPlatformIndex && platformCount != K_MAX_PLATFORMS_IN_GAME)
    {
        CCSprite* bonus = dynamic_cast<CCSprite*>(batchNode->getChildByTag(kBonusStartTag + currentBonusType));
        bonus->setPosition(ccp(x, currentPlatformY + 30));
        bonus->setVisible(true);
    }
    
    // TODO: If the platform count reaches K_MAX_PLATFORMS, Add the "Exit" icon onto the platform.
    if (platformCount == K_MAX_PLATFORMS_IN_GAME)
    {
        // Add the "Exit" icon onto the platform.
        CCSprite* exit = dynamic_cast<CCSprite*>(getChildByTag(kExit));
        exit->setPosition(ccp(x, currentPlatformY + 48));
        exit->setVisible(true);
    }
}

void GameLayer::_resetPlatforms()
{
    currentPlatformY = -1;
    currentPlatformTag = kPlatformsStartTag;
    currentMaxPlatformStep = 60.0f;
    currentBonusPlatformIndex = 0;
    currentBonusType = 0;
    platformCount = 0;
    
    while(currentPlatformTag < kPlatformsStartTag + K_NUM_PLATFORMS)
    {
        _resetPlatform();
        currentPlatformTag++;
    }
    
    gameSuspended = false;
}

// RocketMan logic
void GameLayer::_resetRocketMan()
{
    CCSpriteBatchNode* batchNode = dynamic_cast<CCSpriteBatchNode*>(getChildByTag(kSpriteManager));
    CCSprite* rocketMan = dynamic_cast<CCSprite*>(batchNode->getChildByTag(kRocketMan));
    
    rm_position.x = SCREEN_WIDTH * 0.5f;
    rm_position.y = SCREEN_WIDTH * 0.5f;
    rocketMan->setPosition(rm_position);
    
    rm_velocity.x = 0;
    rm_velocity.y = 0;
    
    rm_acceleration.x = 0;
    rm_acceleration.y = -550.0f;
    
    rm_lookingRight = true;
    rocketMan->setScaleX(1.0f);
}

void GameLayer::_resetBonus()
{
    CCSpriteBatchNode* batchNode = dynamic_cast<CCSpriteBatchNode*>(getChildByTag(kSpriteManager));
    CCSprite* bonus = dynamic_cast<CCSprite*>(batchNode->getChildByTag(kBonusStartTag + currentBonusType));
    bonus->setVisible(false);
    
    currentBonusPlatformIndex += CCRANDOM_0_1() * (K_MAX_BONUS_STEP - K_MIN_BONUS_STEP) + K_MIN_BONUS_STEP;
    
    if(score < 10000)
        currentBonusType = 0;
    else if(score < 50000)
        currentBonusType = CCRANDOM_0_1() * 2;
    else if(score < 100000)
        currentBonusType = CCRANDOM_0_1() * 3;
    else
        currentBonusType = CCRANDOM_0_1() * 2 + 2;
    
}

void GameLayer::_startGame()
{
    score = 0;
    //resetClouds();
    _resetPlatforms();
    _resetRocketMan();
    _resetBonus();
}

// when RocketMan is jumping, this is  its velocity
void GameLayer::_jump()
{
    // play sound effect when player jumps
#if K_PLAY_SOUND_EFFECTS
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Sounds/jump.wav");
#endif
    rm_velocity.y = 350.0f + fabsf(rm_velocity.x);
}


// on receving a boost, RocketMan jumps super high with a super velocity
void GameLayer::_superJump()
{
    rm_velocity.y = 1000.0f + fabsf(rm_velocity.x);
}

// Assuming that when the game gets over, the high scores need to be shown, so transition over to that
void GameLayer::_showHighScores()
{
    gameSuspended = true;
    stopAllActions();
    unscheduleUpdate();
    
    CCTransitionFade* scene = CCTransitionFade::create(1.0f, HighScoreLayer::scene(score), ccWHITE);
    CCDirector::sharedDirector()->replaceScene(scene);
}


