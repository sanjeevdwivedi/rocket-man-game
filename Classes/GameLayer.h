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

private:
    cocos2d::CCPoint rm_position;

    void _startGame();
    void _resetRocketMan();
};

#endif /* defined(__RocketManCocos2dx__GameLayer__) */
