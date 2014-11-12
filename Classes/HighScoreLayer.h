//
//  HighScoreLayer.h
//  RocketManCocos2dx
//
//  Modified by Sanjeev Dwivedi, Dale Stammen and Eric Mitelette
//  Created by Carlos Pinan on 13/10/13.
//
//

#ifndef __RocketManCocos2dx__HighScoreLayer__
#define __RocketManCocos2dx__HighScoreLayer__

#include "MainLayer.h"

class HighScoreLayer : public MainLayer
{
public:
    static cocos2d::CCScene* scene(int lastScore);
    HighScoreLayer(int lastScore);
    virtual void draw();

private:
    cocos2d::CCLayerColor* _playerPopup;
    std::string currentPlayer;
    int currentScore;
    int currentScorePosition;

private:
    std::vector<std::string> split(std::string str, std::string value);
    void _loadCurrentPlayer();
    void _loadHighScores();
    void _updateHighScores();
    void _saveCurrentPlayer();

    void button1Callback(cocos2d::CCObject* pSender);

};

#endif /* defined(__RocketManCocos2dx__HighScoreLayer__) */