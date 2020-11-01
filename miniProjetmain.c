#include "raylib.h"
#include "raymath.h"

#define G 400
#define PLAYER_JUMP_SPD 350.f
#define PLAYER_HOR_SPD 500.f
#define NUMBER_ITEMS 300

// NOTE: Storage positions must start with 0, directly related to file memory layout
typedef enum {
    STORAGE_POSITION_SCORE      = 0,
    STORAGE_POSITION_HISCORE    = 1
} StorageData;
typedef struct Player {
    Vector2 position;
    float speed;
    bool canJump;
    double timeSinceJump;
} Player;

typedef struct EnvItem {
    Rectangle rect;
    int blocking;
    int actif;
    Color color;
} EnvItem;
//---------------------------
//Custom Color
//-------------------------
Color ultramarine = { 65, 55, 99, 255 };
Color altYellow = { 255, 242, 110, 255 };
Color altPurple = { 216, 0, 87, 255 };

//-----------------------------------
//Global Variable
//-----------------------
static int screenWidth = 800;
static int screenHeight = 1000;

static Player player = { 0 };
static EnvItem envItems[NUMBER_ITEMS];

//---------------------
//Default
//----------------
static bool isGameOver =false;
static bool inGame = false;
static bool inSkin = false;
static bool isStageWon = false;
static int score = 0;
static int hiscore = 0; 
static int iMax = 0;
static Color skinColor = RED;
static float stageSpeed;

static  Sound fxCollision;
static double startTime;

void UpdatePlayer(Player *player, EnvItem *envItems, int envItemsLength, float delta);
void InitGame(void);
void UpdateStage(Player *player, EnvItem *envItems, int envItemsLength);
void ReInitStage(EnvItem *envItems, int envItemsLength);
void UpdateStageOnHit(EnvItem *envItems, int envItemsLength);

int main(void)
{
    ReInitStage(envItems, 200);
    // Initialization
    //--------------------------------------------------------------------------------------
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "DoodleJump ECO+");
    int envItemsLength = sizeof(envItems)/sizeof(envItems[0]);
    
     // Load WAV audio file
    InitAudioDevice(); 
    fxCollision = LoadSound("ressource/collision.wav");
    SetSoundVolume(fxCollision, 0.2);

    DrawText("Press enter to start the game ", 20, 20, 50, BLACK);
    
    
    InitGame();
   
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) 
    {
        // Update
        //----------------------------------------------------------------------------------
        //UpdateMusicStream(fxGameOver);
        
        float deltaTime = GetFrameTime();
        
        if(inGame)
        {
            
            //PlayMusicStream(fxGameOver);
            //stage speed at the beginning
            
            //the game's on
            if(!isGameOver && inGame && !isStageWon && !inSkin)
            {
                UpdatePlayer(&player, envItems, envItemsLength, deltaTime);
                UpdateStage(&player, envItems, envItemsLength);
                
                // to end the game 
                if(IsKeyPressed(KEY_BACKSPACE)) isGameOver = true;
                
                // real time score
                score = GetTime() - startTime;
               
            }
            // gameOver or Game Won
            else if ((isGameOver && inGame) || (isStageWon && inGame)) 
            {
                if (IsKeyPressed(KEY_ENTER)) // replay the game
                {
                    InitGame(); 
                    isGameOver = false;
                    isStageWon = false;
                    ReInitStage(envItems, envItemsLength);
                }
                else if(IsKeyPressed(KEY_BACKSPACE)){ // go to menu
                    inGame = false;
                    isGameOver = false;
                }
                // save the actual score and update the highscore if needed
                SaveStorageValue(STORAGE_POSITION_SCORE, score);
                if(score>LoadStorageValue(STORAGE_POSITION_HISCORE)) SaveStorageValue(STORAGE_POSITION_HISCORE, score);
                
                // to print the highScore
                hiscore = LoadStorageValue(STORAGE_POSITION_HISCORE);          
            }
        }
        // skin menu
        else if(inSkin ==true){
            if(IsKeyPressed(KEY_Q)) skinColor = GREEN;
            else if (IsKeyPressed(KEY_W)) skinColor = PURPLE;
            else if(IsKeyPressed(KEY_E)) skinColor = RED;
            else if(IsKeyPressed(KEY_R)) skinColor = YELLOW;
            else if(IsKeyPressed(KEY_BACKSPACE)){ 
                inSkin = false;
            }
        }
        // menu
        else{
            score = LoadStorageValue(STORAGE_POSITION_SCORE);
            hiscore = LoadStorageValue(STORAGE_POSITION_HISCORE);
            if(IsKeyPressed(KEY_ENTER)){ // start the game
                inGame = true;
                ReInitStage(envItems, envItemsLength);
                InitGame(); 
            }
            else if (IsKeyPressed(KEY_Q)){ // press A on azerty keybord || enter in the skin selection menu
                inSkin = true;
            }
            else if (IsKeyPressed(KEY_R)){
                SaveStorageValue(STORAGE_POSITION_SCORE, 0);
                SaveStorageValue(STORAGE_POSITION_HISCORE, 0);
            }
        }
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(ultramarine);
            if(!isGameOver && inGame && !isStageWon){
                
                for (int i = 0; i < envItemsLength; i++) {
                    DrawRectangleRec(envItems[i].rect, envItems[i].color);
                }

                Rectangle playerRect = { player.position.x - 20, player.position.y - 40, 40, 40 };
                DrawRectangleRec(playerRect, skinColor);
                
                DrawText("Controls:", 20, 20, 30, WHITE);
                DrawText("- Right/Left to move", 40, 60, 20, GRAY);
                DrawText("- Backspace to end the game", 40, 90, 20, GRAY);
                DrawText(TextFormat("%04i sec", score), 600, 20, 40, GRAY);
                
                }
               
            else if (isGameOver==true) {
                DrawText ("GAME OVER :'( ",20, 120, 60, RED);
                DrawText(TextFormat("%04i sec.", score), 80, 200, 40, altYellow);
                DrawText ("'BACKSPACE' to return to the menu",20, 300, 30, WHITE);
                DrawText("'ENTER' to try again",20, 350, 30, WHITE);
                DrawText(TextFormat("LAST SCORE: %i", score), 200, 450, 50, MAROON);
                DrawText(TextFormat("HI-SCORE: %i", hiscore), 200, 550, 70, BLACK);
                
            }
            else if (inGame == false && inSkin==false) {
                DrawText("'ENTER' to start ", 200, 120, 50, WHITE);
                DrawText("press 'A' to change skin ", 20, 300, 40, altYellow);
                DrawText("press 'R' to reset scores  ", 20, 360, 35, PURPLE);
                DrawText(TextFormat("LAST SCORE: %i", score), 200, 600, 40, MAROON);
                DrawText(TextFormat("HI-SCORE: %i", hiscore), 200, 700, 50, BLACK);
           
            }
            else if ( isStageWon == true){
                DrawText("You won the stage !",20, 120, 60, WHITE );
                DrawText(TextFormat("%04i sec.", score), 80, 200, 40, GRAY);
            }
            else if(inGame==false && inSkin==true){
                DrawText("Skin selection",20, 50, 60, WHITE );
                DrawText("The actual Skin",20,120, 30, altYellow);
                DrawRectangle(300, 120, 30, 30, skinColor);
                //choix des couleurs
                DrawRectangle(40, 300, 40, 40, GREEN);
                DrawText("press A",100,300, 40, WHITE);
                
                DrawRectangle(40, 400, 40, 40, PURPLE);
                DrawText("press Z",100,400, 40, WHITE);
                
                DrawRectangle(40, 500, 40, 40, RED);
                DrawText("press E",100,500, 40, WHITE);
                
                DrawRectangle(40, 600, 40, 40, YELLOW);
                DrawText("press R",100,600, 40, WHITE);
                
                DrawText("return to menu, press 'BACKSPACE' ",100,700, 40, GRAY);
            } 
            

            
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
     UnloadSound(fxCollision);

    CloseAudioDevice();
    CloseWindow();      // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void UpdatePlayer(Player *player, EnvItem *envItems, int envItemsLength, float delta)
{   
    if(!isStageWon){
        
        if (IsKeyDown(KEY_LEFT)) player->position.x -= PLAYER_HOR_SPD*delta;
        if (IsKeyDown(KEY_RIGHT)) player->position.x += PLAYER_HOR_SPD*delta;
        
        if(player->position.x > screenWidth) {			//	if the player jumps to the right border of the window, move the player to the left border
            player->position.x = player->position.x-screenWidth;
        }
        if(player->position.x < 0) {			//	if the player jumps to the right border of the window, move the player to the left border
            player->position.x = player->position.x+screenWidth;
        }
        if(player->position.y <= envItems[iMax].rect.y){
            isStageWon=true;
        } 
        
        if (player->canJump) 
        {
            player->speed = -PLAYER_JUMP_SPD;
            player->timeSinceJump = GetTime();
            player->canJump = false;
            PlaySound(fxCollision);
        }
        
        // GameOver Conditions
        if(GetTime() - player->timeSinceJump > 4){ // 4 sec without touching a platform : gameOver
            isGameOver=true;
           // printf("ligne 167");
        }
        int hitObstacle = 0;
        for (int i = 0; i < envItemsLength; i++) 
        {     
            Vector2 *p = &(player->position);
            if (envItems[i].blocking &&
                envItems[i].rect.x <= p->x && 
                envItems[i].rect.x + envItems[i].rect.width >= p->x &&
                envItems[i].rect.y >= p->y &&
                envItems[i].rect.y < p->y + player->speed*delta &&
                envItems[i].actif)
            {
                hitObstacle = 1;
                player->speed = 0.0f;
                p->y = envItems[i].rect.y;
                //UpdateStageOnHit(envItems, envItemsLength, delta);
            }
        }
        
        UpdateStageOnHit(envItems, envItemsLength);
        if (!hitObstacle) 
        {
            player->position.y += player->speed*delta;
            player->speed += G*delta;
            player->canJump = false;
            
            
        } 
        else{ player->canJump = true;}
       
    }
}


void InitGame()
{
    player.position = (Vector2){ 350, 900 };//starting position
    player.speed = 0;
    player.canJump = false;
    player.timeSinceJump = GetTime();
    
    // startTime allows to calculate the score
    startTime = GetTime();


}
void UpdateStage(Player *player, EnvItem *envItems, int envItemsLength)
{
    for(int i =1; i<envItemsLength; i++){
        if (envItems[i].rect.y  > 950 ){
            envItems[i].actif = 0 ;
            envItems[i].color = BLANK;
        }
    }
}
void ReInitStage(EnvItem *envItems, int envItemsLength)
{   // randomly generate envItems but for the 2 first ones
    envItems[0].color = ultramarine ;
    envItems[0].rect.y = -1000;
    envItems[0].rect.x = 0;
    envItems[0].rect.width = 800;
    envItems[0].rect.height = 2000 ;
    envItems[0].blocking = 0;
    envItems[0].actif = 1;

    envItems[1].color = VIOLET ;
    envItems[1].rect.y = 900;
    envItems[1].rect.x = 0;
    envItems[1].rect.width = 800;
    envItems[1].rect.height = 400 ;
    envItems[1].blocking = 1;
    envItems[1].actif = 1;
    iMax=0;
    for(int i =2; i<envItemsLength; i++){      
        if (i>1){
            envItems[i].color = altYellow;
            envItems[i].rect.y = GetRandomValue(envItems[i-1].rect.y,envItems[i-1].rect.y-200); // p
            envItems[i].rect.x = GetRandomValue(0, screenWidth-100 ); // 100 : with of envItems
            envItems[i].rect.width = 100;
            envItems[i].rect.height = 10 ;
            envItems[i].blocking = 1;
            envItems[i].actif = 1;
            if(envItems[i].rect.y< envItems[iMax].rect.y) iMax = i;            
        }
    }   
}
void UpdateStageOnHit(EnvItem *envItems, int envItemsLength)
{
    if(score < 15) stageSpeed = 1;
    else if ( score >=15 && score < 30) stageSpeed = 1.2;
    else if ( score >=30 && score < 60) stageSpeed = 1.4;
    else if (score >= 60) stageSpeed = 1.6;
    for(int i =1; i <= envItemsLength; i++){
        envItems[i].rect.y += stageSpeed;
    }
}
