//
//  GameConfig.h
//  RocketManCocos2dx
//
//  Modified by Sanjeev Dwivedi, Dale Stammen and Eric Mitelette
//  Created by Carlos Pinan on 13/10/13.
//
//

#ifndef RocketManCocos2dx_GameConfig_h
#define RocketManCocos2dx_GameConfig_h

#include <ctime>
#include <cstdlib>

#define RANDOM_SEED() srand(time(0))

// #define RESET_DEFAULTS

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 480
#define DISPLAY_STATS true

#define K_MIN_PLATFORM_STEP 50
#define K_MAX_PLATFORM_STEP 300
#define K_NUM_PLATFORMS 15
#define K_PLATFORM_TOP_PADDING 10
#define K_MAX_PLATFORMS_IN_GAME 100

#define K_MIN_BONUS_STEP 30
#define K_MAX_BONUS_STEP 50

#define K_PLAY_BACKGROUND_MUSIC 1
#define K_PLAY_BACKGROUND_MUSIC_VOLUME 0.2f
#define K_PLAY_SOUND_EFFECTS 1

enum {
    kSpriteManager = 0,
    kRocketMan,
    kScoreLabel,
    kExit,
    kPlatformsStartTag = 2000,
    kBonusStartTag = 3000,
    kPopUpPlayerTag = 10000,
    kPopUpEditBoxTag = 150000
};


#endif
