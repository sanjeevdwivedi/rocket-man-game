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
    
private:
    void _resetRocketMan();
    void _startGame();
};

#endif /* defined(__TweeJumpCocos2dx__GameLayer__) */
