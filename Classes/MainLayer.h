//
//  MainLayer.h
//  TweeJumpCocos2dx
//
//  Created by Carlos Pinan on 13/10/13.
//
//

#ifndef __TweeJumpCocos2dx__MainLayer__
#define __TweeJumpCocos2dx__MainLayer__

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

#endif /* defined(__TweeJumpCocos2dx__MainLayer__) */
