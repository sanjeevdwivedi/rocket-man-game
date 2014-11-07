//
//  GameLayer.h
//  TweeJumpCocos2dx
//
//  Created by Carlos Pinan on 13/10/13.
//
//

#ifndef __TweeJumpCocos2dx__GameLayer__
#define __TweeJumpCocos2dx__GameLayer__

#include "MainLayer.h"

class GameLayer : public MainLayer
{
public:
    GameLayer();
    virtual ~GameLayer();

    void update(float dt);
    static cocos2d::CCScene* scene();
    
    void didAccelerate(cocos2d::CCAcceleration *pAccelerationValue);
    
private:
    cocos2d::CCPoint rm_position;
    cocos2d::ccVertex2F rm_velocity;
    cocos2d::ccVertex2F rm_acceleration;
    
    bool gameSuspended;
    bool rm_lookingRight;
    bool isGameOver;
    
    int score;
    int platformCount;
    int currentBonusType;
    float currentPlatformY;
    int currentPlatformTag;
    float currentMaxPlatformStep;
    int currentBonusPlatformIndex;
    
    cocos2d::CCSprite *pHealthSprite;
    cocos2d::CCProgressTimer *pHealthBar;
    int fuelInTank;
    
private:
    void _initPlatform();
    void _initPlatforms();
    void _resetPlatform();
    void _resetPlatforms();
    void _resetRocketMan();
    void _resetBonus();
    void _startGame();
    void _jump();
    void _superJump();
    void _showHighScores();
    
};

#endif /* defined(__TweeJumpCocos2dx__GameLayer__) */
