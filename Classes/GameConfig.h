//
//  GameConfig.h
//  TweeJumpCocos2dx
//
//  Created by Carlos Pinan on 13/10/13.
//
//

#ifndef TweeJumpCocos2dx_GameConfig_h
#define TweeJumpCocos2dx_GameConfig_h

#include <ctime>
#include <cstdlib>

#define RANDOM_SEED() srand(time(0))

// #define RESET_DEFAULTS

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 480
#define DISPLAY_STATS true


enum {
    kSpriteManager = 0,
};



#endif
