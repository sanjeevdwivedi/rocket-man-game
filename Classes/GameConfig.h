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

#define K_PLAY_BACKGROUND_MUSIC_VOLUME 0.2f

enum {
    kSpriteManager = 0,
    kRocketMan,
    kPlatform
};


#endif
