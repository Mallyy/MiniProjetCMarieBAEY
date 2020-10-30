#include "raylib.h"
#include "raymath.h"

#define G 400
#define PLAYER_JUMP_SPD 250.f
#define PLAYER_HOR_SPD 500.f

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
static EnvItem envItems[300];

//---------------------
//Default
//----------------
static bool isGameOver =false;
static bool inGame = false;
static bool inSkin = false;
static bool isStageWon = false;
static int score = 0;
static int iMax = 0;
static Color skinColor = RED;

static  Sound fxCollision;
static double startTime;

void UpdatePlayer(Player *player, EnvItem *envItems, int envItemsLength, float delta);
void InitGame(void);
void UpdateStage(Player *player, EnvItem *envItems, int envItemsLength);
void ReInitStage(EnvItem *envItems, int envItemsLength);
void UpdateStageOnHit(EnvItem *envItems, int envItemsLength, float delta);

int main(void)
{
    ReInitStage(envItems, 200);
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "DoodleJump ECO+");
    int envItemsLength = sizeof(envItems)/sizeof(envItems[0]);
            // Load WAV audio file
    
    InitAudioDevice(); 
    fxCollision = LoadSound("ressource/collision.wav");
    //Music fxGameOver = LoadMusicStream("OneShot_OST.ogg");
    
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
        
        if(inGame){
            
            //PlayMusicStream(fxGameOver);
            if(!isGameOver && inGame && !isStageWon && !inSkin){
                UpdatePlayer(&player, envItems, envItemsLength, deltaTime);
                UpdateStage(&player, envItems, envItemsLength);
                
                if(IsKeyPressed(KEY_DELETE)) isGameOver = true;
                score = GetTime() - startTime;

            }
            else if ((isGameOver && inGame) || (isStageWon && inGame)) {
               // camera->offset = (Vector2){ width/2, height/2 };
               
                if (IsKeyPressed(KEY_ENTER))
                {
                    InitGame(); 
                    isGameOver = false;
                    isStageWon = false;
                    ReInitStage(envItems, envItemsLength);
                }
                else if(IsKeyPressed(KEY_DELETE)){
                    inGame = false;
                    isGameOver = false;
                } 
            }
        }
        else if(inSkin ==true){
            if(IsKeyPressed(KEY_Q)) skinColor = GREEN;
            else if (IsKeyPressed(KEY_W)) skinColor = PURPLE;
            else if(IsKeyPressed(KEY_E)) skinColor = RED;
            else if(IsKeyPressed(KEY_R)) skinColor = YELLOW;
            else if(IsKeyPressed(KEY_DELETE)){ // Touche Suppr 
                inSkin = false;
            }
        }
        else{
            if(IsKeyPressed(KEY_ENTER)){
                inGame = true;
                ReInitStage(envItems, envItemsLength);
                InitGame(); 
            }
            else if (IsKeyPressed(KEY_Q)){ // press A on azerty keybord 
                inSkin = true;
            }
        }
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(LIGHTGRAY);
            if(!isGameOver && inGame && !isStageWon){
                
                for (int i = 0; i < envItemsLength; i++) {
                    DrawRectangleRec(envItems[i].rect, envItems[i].color);
                }

                Rectangle playerRect = { player.position.x - 20, player.position.y - 40, 40, 40 };
                DrawRectangleRec(playerRect, skinColor);
                
                //EndMode2D();

                DrawText("Controls:", 20, 20, 10, BLACK);
                DrawText("- Right/Left to move", 40, 40, 10, DARKGRAY);
                DrawText(TextFormat("%04i sec", score), 600, 20, 40, GRAY);
                //DrawText("- Space to jump", 40, 60, 10, DARKGRAY);
                //DrawText("- Mouse Wheel to Zoom in-out, R to reset zoom", 40, 80, 10, DARKGRAY);
                //DrawText("- C to change camera mode", 40, 100, 10, DARKGRAY);
                //DrawText("Current camera mode:", 20, 120, 10, BLACK);
                
                }
               
            else if (isGameOver==true) {
                DrawText ("GAME OVER",20, 120, 50, BLACK);
                DrawText(TextFormat("%04i sec.", score), 80, 200, 40, GRAY);
                DrawText ("Press 'SUPPR' to return to the menu or 'ENTER' to try again",20, 300, 20, BLACK);
                
            }
            else if (inGame == false && inSkin==false) {
                DrawText("press enter to start ", 20, 120, 60, BLACK);
                DrawText("press A to change skin ", 20, 200, 30, GRAY);
           
            }
            else if ( isStageWon == true){
                DrawText("You won the stage !",20, 120, 60, BLACK );
                DrawText(TextFormat("%04i sec.", score), 80, 200, 40, GRAY);
            }
            else if(inGame==false && inSkin==true){
                DrawText("Skin selection",20, 50, 60, BLACK );
                DrawText("The actual Skin",20,120, 30, DARKGRAY);
                DrawRectangle(300, 120, 30, 30, skinColor);
                //choix des couleurs
                DrawRectangle(40, 300, 40, 40, GREEN);
                DrawText("press A",100,300, 40, BLACK);
                
                DrawRectangle(40, 400, 40, 40, PURPLE);
                DrawText("press Z",100,400, 40, BLACK);
                
                DrawRectangle(40, 500, 40, 40, RED);
                DrawText("press E",100,500, 40, BLACK);
                
                DrawRectangle(40, 600, 40, 40, YELLOW);
                DrawText("press R",100,600, 40, BLACK);
                
                DrawText("return to menu, press suppr ",100,700, 40, GRAY);
            } 
            

            
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
     UnloadSound(fxCollision); 
     //UnloadMusicStream(fxGameOver);// Unload sound data
    // Unload sound data
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
        if(GetTime() - player->timeSinceJump > 3){
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
                UpdateStageOnHit(envItems, envItemsLength, delta);
            }
        }
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
    player.position = (Vector2){ 350, 900 };
    player.speed = 0;
    player.canJump = false;
    player.timeSinceJump = GetTime();
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
{   // place les platforme aleatoirement
    iMax=0;
    for(int i =0; i<envItemsLength; i++){    
            envItems[i].actif = 1 ;
            if (i>1){
                envItems[i].color = altYellow;
                envItems[i].rect.y = GetRandomValue(-10000,1000);
                envItems[i].rect.x = GetRandomValue(0, screenWidth-100 ); // 100 : la largeur d 'une platforme
                envItems[i].rect.width = 100;
                envItems[i].rect.height = 10 ;
                envItems[i].blocking = 1;
                envItems[i].actif = 1;
                if(envItems[i].rect.y< envItems[iMax].rect.y) iMax = i;
            }
            
            
    }
    envItems[0].color = ultramarine ;
    envItems[0].rect.y = -1000;
    envItems[0].rect.x = 0;
    envItems[0].rect.width = 800;
    envItems[0].rect.height = 2000 ;
    envItems[0].blocking = 0;
    envItems[0].actif = 1;

    envItems[1].color = altPurple ;
    envItems[1].rect.y = 900;
    envItems[1].rect.x = 0;
    envItems[1].rect.width = 800;
    envItems[1].rect.height = 400 ;
    envItems[1].blocking = 1;
    envItems[0].actif = 1;
    
}
void UpdateStageOnHit(EnvItem *envItems, int envItemsLength, float delta)
{
    for(int i =1; i <= envItemsLength; i++){
        envItems[i].rect.y += PLAYER_JUMP_SPD;
    }
}
