#include "raylib.h"
#include <cstdlib>

char Title[]{"Elf Run"}; 

int velocity{0};
const int jumpVelocity{-800};
const int gravity{1600};  
bool isInAir{};

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

int initialMushroomGap{0};
int windowDimensions[2];

bool isOnGround(AnimData data, int windowHeight);

AnimData updateAnimationData(AnimData data, float deltaTime, int maxFrame);

float bgX{};
float mgX{};
float fgX{};

