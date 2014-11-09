//
//  HighScoreLayer.h
//  TweeJumpCocos2dx
//
//  Created by Carlos Pinan on 13/10/13.
//
//

#ifndef __TweeJumpCocos2dx__HighScoreLayer__
#define __TweeJumpCocos2dx__HighScoreLayer__

#include "MainLayer.h"

class HighScoreLayer : public MainLayer
{
public:
    static cocos2d::CCScene* scene(int lastScore);
    HighScoreLayer(int lastScore);
    virtual void draw();
    
private:
    int currentScore;
    
private:

    void button1Callback(cocos2d::CCObject* pSender);
    
};

#endif /* defined(__TweeJumpCocos2dx__HighScoreLayer__) */