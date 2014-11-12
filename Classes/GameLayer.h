//
//  GameLayer.h
//  RocketManCocos2dx
//
//  Modified by Sanjeev Dwivedi, Dale Stammen and Eric Mitelette
//  Created by Carlos Pinan on 13/10/13.
//
//

#ifndef __RocketManCocos2dx__GameLayer__
#define __RocketManCocos2dx__GameLayer__

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

    int score;
    int currentPlatformTag;
    int platformCount;
    float currentPlatformY;
    float currentMaxPlatformStep;
    int currentBonusType;
    int currentBonusPlatformIndex;

    cocos2d::CCSprite *pHealthSprite;
    cocos2d::CCProgressTimer *pHealthBar;
    int fuelInTank;

    void _jump();
    void _initPlatform();
    void _initPlatforms();
    void _resetPlatform();
    void _resetPlatforms();
    void _startGame();
    void _resetRocketMan();
    void _resetBonus();
    void _superJump();
    void _showHighScores();
};

#endif /* defined(__RocketManCocos2dx__GameLayer__) */
