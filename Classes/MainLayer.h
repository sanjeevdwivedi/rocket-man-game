//
//  MainLayer.h
//  RocketManCocos2dx
//
//  Modified by Sanjeev Dwivedi, Dale Stammen and Eric Mitelette
//  Created by Carlos Pinan on 13/10/13.
//
//

#ifndef __RocketManCocos2dx__MainLayer__
#define __RocketManCocos2dx__MainLayer__

#include "cocos2d.h"
#include "GameConfig.h"

class MainLayer : public cocos2d::CCLayer
{
public:
    
    MainLayer();

    static cocos2d::CCScene* scene();
	virtual void update(float dt);
    
protected:
    
};

#endif /* defined(__RocketManCocos2dx__MainLayer__) */
