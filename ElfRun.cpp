#include "ElfRun.h"

int main()
{
    windowDimensions[0] = 1500;
    windowDimensions[1] = 800;

    InitWindow(windowDimensions[0], windowDimensions[1], Title);       
    SetTargetFPS(60);

    Texture2D background = LoadTexture("Assets/Background.png");    

    Texture2D midground = LoadTexture("Assets/Foreground.png");  

    Texture2D foreground = LoadTexture("Assets/Mainground.png");  

    bool collision{}; 
    
    //elfie textures
    Texture2D elfie = LoadTexture("Assets/Elf.png");
    AnimData elfieData;

    elfieData.rec.width = elfie.width/8;
    elfieData.rec.height = elfie.height;
    elfieData.rec.x = 0;
    elfieData.rec.y = 0;
    elfieData.pos.x = windowDimensions[0]/2 - elfieData.rec.width/2;
    elfieData.pos.y = windowDimensions[1] - elfieData.rec.height;
    elfieData.frame = 0;
    elfieData.updateTime = 1.0/6.0;
    elfieData.runningTime = 0.0;

    //poison mushroom textures
    Texture2D pmushroom = LoadTexture("Assets/poisonmushroom.png");
    
    const int numberOfMushrooms{3};
    AnimData poisonousmushrooms[numberOfMushrooms]{};

    for (int i = 0; i < numberOfMushrooms; i++)
    {      
        poisonousmushrooms[i].rec.x = 0.0;
        poisonousmushrooms[i].rec.y = 0.0;
        poisonousmushrooms[i].rec.width = pmushroom.width/4;
        poisonousmushrooms[i].rec.height = pmushroom.height/4;
        poisonousmushrooms[i].pos.y = windowDimensions[1] - pmushroom.height/4;
        poisonousmushrooms[i].frame = 0;
        poisonousmushrooms[i].runningTime = 0.0;
        poisonousmushrooms[i].updateTime = 1.0/3.0;
        
        // Set the x position for each poisonous mushroom individually
        poisonousmushrooms[i].pos.x = windowDimensions[0] + initialMushroomGap;
        
        initialMushroomGap += 00;
    }

    
    //nebula X velocity (pixles/second)
    int pmushroomVelocity{-200};   

    while (!WindowShouldClose())
    {
        //DELTA TIME
        const float dT{GetFrameTime()};

        BeginDrawing();
        ClearBackground(WHITE);

        bgX -= 10 * dT;
        if (bgX <= -background.width*2)
        {
            bgX = 0.0;
        }

        mgX -= 60 * dT;
        if (mgX <= -midground.width)
        {
            mgX = 0.0;
        }

        fgX -= 200 * dT; // Keep the scrolling speed at 80 pixels per second
        if (fgX <= -foreground.width)
        {
            fgX = 0.0;
        }

        Vector2 bg1Position{bgX, 0.0};
        DrawTextureEx(background, bg1Position, 0.0, 1.0, WHITE);

        Vector2 bg2Position{bgX + background.width, 0.0};
        DrawTextureEx(background, bg2Position, 0.0, 1.0, WHITE);

        Vector2 mg1Position{mgX, windowDimensions[1] - midground.height};
        DrawTextureEx(midground, mg1Position, 0.0, 1.0, WHITE);

        Vector2 mg2Position{mgX + midground.width, windowDimensions[1] - midground.height};
        DrawTextureEx(midground, mg2Position, 0.0, 1.0, WHITE);

        Vector2 fg1Position{fgX, windowDimensions[1] - foreground.height};
        DrawTextureEx(foreground, fg1Position, 0.0, 1.0, WHITE);

        Vector2 fg2Position{fgX + foreground.width, windowDimensions[1] - foreground.height};
        DrawTextureEx(foreground, fg2Position, 0.0, 1.0, WHITE);

        //Check if Character on the ground       
        if (isOnGround(elfieData, windowDimensions[1]))
        {
              velocity = 0;
              isInAir = false;    
        }
        // If not, gravity will pull character down
        else
        {
            velocity += gravity * dT; 
            isInAir = true;
        }
        
        //KeyPresses
        ////Jump
        if ((IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W)) && !isInAir)
        {
            velocity += jumpVelocity;         
        }        

        // Adds the velocity into the Y axis 
        elfieData.pos.y += velocity * dT;    

        elfieData.runningTime += dT;  

        if (!isInAir)
        {
             elfieData = updateAnimationData(elfieData, dT, 7);         
        }

        for (int i = 0; i < numberOfMushrooms; i++)
        {
             poisonousmushrooms[i].pos.x += pmushroomVelocity * dT;
        }     

        for (AnimData pmushroom : poisonousmushrooms)
        {
            float pad{20};
            Rectangle pmushroomVelocityRec{
                pmushroom.pos.x + pad,
                pmushroom.pos.y + pad,
                pmushroom.rec.width - 2*pad,
                pmushroom.rec.height - 2*pad
            };
            Rectangle elfieRec
            {
                elfieData.pos.x,
                elfieData.pos.y,
                elfieData.rec.width,
                elfieData.rec.height
            };

            if (CheckCollisionRecs(pmushroomVelocityRec, elfieRec))
            {
                collision = true;
            }
        }

        float finishLine {poisonousmushrooms[numberOfMushrooms - 1].pos.x};  

        // update finishline
        finishLine += pmushroomVelocity * dT + 400;

        for (int i = 0; i < numberOfMushrooms; i++)
        {   
             poisonousmushrooms[i] = updateAnimationData(poisonousmushrooms[i], dT, 4);
            
        }

        if (collision)
        {
            DrawText("Game Over!", windowDimensions[0]/2.4, windowDimensions[1]/2, 55, WHITE);
        }
        else if (elfieData.pos.x >= finishLine)
        {
            DrawText("You Win!", windowDimensions[0]/2.4, windowDimensions[1]/2, 55, WHITE);
        }
        else
        {
            for (int i = 0; i < numberOfMushrooms; i++)
            {
                // draw poison mushroom
            DrawTextureRec(pmushroom, poisonousmushrooms[i].rec, poisonousmushrooms[i].pos, WHITE);            
            }

            //draw elfie
            DrawTextureRec(elfie, elfieData.rec, elfieData.pos, WHITE);
        }
        
        EndDrawing();
    }

    UnloadTexture(elfie);
    UnloadTexture(pmushroom);
    UnloadTexture(background);
    UnloadTexture(midground);

    CloseWindow();
}

bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
};

AnimData updateAnimationData(AnimData data, float deltaTime, int maxFrame)
{
    // update running time
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;

        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if (data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }
    return data;
}