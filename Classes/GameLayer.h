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
    
    
private:
    
private:
    void _resetRocketMan();
    void _startGame();
};

#endif /* defined(__TweeJumpCocos2dx__GameLayer__) */
