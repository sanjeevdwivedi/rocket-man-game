//
//  HighScoreLayer.cpp
//  RocketManCocos2dx
//
//  Modified by Sanjeev Dwivedi, Dale Stammen and Eric Mitelette
//  Created by Carlos Pinan on 13/10/13.
//
//

#include "HighScoreLayer.h"
#include "GameLayer.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace extension;
using namespace std;

CCScene* HighScoreLayer::scene(int lastScore)
{
    CCScene* scene = CCScene::create();
    HighScoreLayer* layer = new HighScoreLayer(lastScore);
    scene->addChild(layer);
    layer->autorelease();
    return scene;
}


HighScoreLayer::HighScoreLayer(int lastScore) : currentScore(lastScore)
{
    _loadCurrentPlayer();
    _loadHighScores();
    _updateHighScores();

    CCSpriteBatchNode* batchNode = dynamic_cast<CCSpriteBatchNode*>(getChildByTag(kSpriteManager));
    CCSprite* title = CCSprite::createWithTexture(batchNode->getTexture(), CCRectMake(608, 192, 225, 57));
    batchNode->addChild(title, 5);
    title->setPosition(ccp(160, 420));

    float start_y = 360.0f;
    float step = 27.0f;
    int count = 0;

    //
    CCUserDefault* defaults = CCUserDefault::sharedUserDefault();
    string highscores = defaults->getStringForKey("highscores", "");
    vector<string> vectorScores = split(highscores, "\n");
    if (vectorScores.size() > 0)
    {
        for (unsigned int i = 0; i < vectorScores.size(); i++)
        {
            vector<string> data = split(vectorScores[i], ";");
            if (data.size() > 0)
            {
                string player = data[0];
                int score = atoi(data[1].c_str());

                CCLabelTTF* label1 = CCLabelTTF::create(CCString::createWithFormat("%d", (count + 1))->getCString(), "Arial", 14, CCSizeMake(30, 40), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
                addChild(label1, 5);
                label1->setColor(ccWHITE);
                label1->setOpacity(200);
                label1->setPosition(ccp(15, start_y - count * step - 2.0f));

                CCLabelTTF* label2 = CCLabelTTF::create(player.c_str(), "Arial", 16, CCSizeMake(240, 40), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
                addChild(label2, 5);
                label2->setColor(ccWHITE);
                label2->setPosition(ccp(160, start_y - count * step));

                CCLabelTTF* label3 = CCLabelTTF::create(CCString::createWithFormat("%d", score)->getCString(), "Arial", 16, CCSizeMake(290, 40), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
                addChild(label3, 5);
                label3->setColor(ccWHITE);
                label3->setOpacity(200);
                label3->setPosition(ccp(160, start_y - count * step));

                count++;
                if (count == 10)
                    break;

            }
        }
    }

    //

    CCMenuItemImage* button1 = CCMenuItemImage::create("playAgainButton.png", "playAgainButton.png", this, menu_selector(HighScoreLayer::button1Callback));

    CCMenu* menu = CCMenu::create(button1, NULL);
    menu->alignItemsVerticallyWithPadding(9.0f);
    menu->setPosition(ccp(160, 58));
    addChild(menu);
}

void HighScoreLayer::_loadCurrentPlayer()
{
    currentPlayer = "";
    currentPlayer = CCUserDefault::sharedUserDefault()->getStringForKey("player", "anonymous");
}

void HighScoreLayer::_loadHighScores()
{
    CCUserDefault* defaults = CCUserDefault::sharedUserDefault();

#ifdef RESET_DEFAULTS
    defaults->setStringForKey("highscores", "");
    defaults->flush();
#endif

    string highscores = defaults->getStringForKey("highscores", "");

    if (highscores.length() == 0)
    {
        string save_scores = "";
        save_scores += "RocketMan;750000\n";
        save_scores += "RocketMan;500000\n";
        save_scores += "RocketMan;250000\n";
        save_scores += "RocketMan;100000\n";
        save_scores += "RocketMan;50000\n";
        save_scores += "RocketMan;20000\n";
        save_scores += "RocketMan;10000\n";
        save_scores += "RocketMan;5000\n";
        save_scores += "RocketMan;1000\n";
        defaults->setStringForKey("highscores", save_scores);
        defaults->flush();
    }
}

void HighScoreLayer::_updateHighScores()
{
    CCUserDefault* defaults = CCUserDefault::sharedUserDefault();

    currentScorePosition = -1;
    int count = 0;
    string highscores = defaults->getStringForKey("highscores", "");
    if (highscores.length() > 0)
    {
        vector<string> vectorScores = split(highscores, "\n");
        if (vectorScores.size() > 0)
        {
            for (unsigned int i = 0; i < vectorScores.size(); i++)
            {
                vector<string> data = split(vectorScores[i], ";");
                if (data.size() > 0)
                {
                    int score = atoi(data[1].c_str());
                    if (currentScore >= score)
                    {
                        currentScorePosition = count;
                        break;
                    }
                    count++;
                }
            }
        }

        if (currentScorePosition >= 0)
        {
            highscores = defaults->getStringForKey("highscores", "");
            vectorScores = split(highscores, "\n");
            CCString* dataScore = CCString::createWithFormat("%s;%d", currentPlayer.c_str(), currentScore);

            string tmpScore = "";
            for (int i = vectorScores.size() - 2; i > currentScorePosition; i--)
                vectorScores[i + 1] = vectorScores[i];

            vectorScores[currentScorePosition] = string(dataScore->getCString());
            string scores = "";
            for (unsigned int i = 0; i < vectorScores.size(); i++)
                scores += vectorScores[i] + "\n";

            defaults->setStringForKey("highscores", scores);
            defaults->flush();
        }

    }

}

vector<string> HighScoreLayer::split(string str, string value)
{
    vector<string> split;
    char *line = strtok(const_cast<char*>(str.c_str()), value.c_str());
    while (line) {
        split.push_back(line);
        line = strtok(NULL, value.c_str());
    }
    return split;
}

void HighScoreLayer::_saveCurrentPlayer()
{
    CCUserDefault* defaults = CCUserDefault::sharedUserDefault();
    defaults->setStringForKey("player", currentPlayer);
    defaults->flush();
}

void HighScoreLayer::draw()
{
    MainLayer::draw();

    if (currentScorePosition < 0)
        return;

    float width = 320.0f;
    float height = 27.0f;
    float x = (320.0f - width) / 2.0f;
    float y = 348 - currentScorePosition * height;

    ccDrawSolidRect(ccp(x, y), ccp(x + width, y + height), ccc4f(1.0f, 0.0f, 0.0f, 0.4f));
}


void HighScoreLayer::button1Callback(CCObject *pSender)
{
    CCTransitionFade* scene = CCTransitionFade::create(0.5f, GameLayer::scene(), ccWHITE);
    CCDirector::sharedDirector()->replaceScene(scene);
}


