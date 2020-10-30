#include "raylib.h"
#include "raymath.h"

#define G 400
#define PLAYER_JUMP_SPD 350.f
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

//-----------------------------------
//Global Variable
//-----------------------
static int screenWidth = 800;
static int screenHeight = 1000;

static Player player = { 0 };
static EnvItem envItems[] = {
        {{ 0, -1000, 850, 2000 }, 0,1, DARKBROWN }, // rect  { x,y,width,height} // background
        {{ 0, 1000, 850, 400 }, 1,1, GRAY }, // platforme initiale
        {{ 300, 200, 100, 10 }, 1,1, GREEN }, 
        {{ 300, 900, 100, 10 }, 1,1, GREEN }, 
        {{ 250, 300, 100, 10 }, 1,1, GREEN },
        {{ 200, 950, 100, 10 }, 1,1, GREEN }, 
        {{ 50, 950, 100, 10 }, 1,1, GREEN },
        {{ 600, 900, 100, 10 }, 1,1, GREEN },  
        {{ 400, 880, 100, 10 }, 1,1, GREEN },       
        {{ 700, 840, 100, 10 }, 1,1, GREEN }, 
        {{ 20, 800, 100, 10 }, 1,1, GREEN }, 
        {{ 640, 750, 100, 10 }, 1,1, GREEN },
        {{ 700, 750, 100, 10 }, 1,1, GREEN },
        {{ 150, 700, 100, 10 }, 1,1, GREEN },
        {{ 300, 650, 100, 10 }, 1,1, GREEN },
        {{ 500, 650, 100, 10 }, 1,1, GREEN },
        {{ 330, 610, 100, 10 }, 1,1, GREEN },
        {{ 600, 770, 100, 10 }, 1,1, GREEN },
        {{ 10, 640, 100, 10 }, 1,1, GREEN },
        {{ 600, 600, 100, 10 }, 1,1, GREEN },
        {{ 250, 570, 100, 10 }, 1,1, GREEN },
        {{ 650, 520, 100, 10 }, 1,1, GREEN },
        {{ 580, 450, 100, 10 }, 1,1, GREEN },
        {{ 650, 300, 100, 10 }, 1,1, GREEN },
        {{ 50, 300, 100, 10 }, 1,1, GREEN },
        {{ 300, 250, 100, 10 }, 1,1, GREEN },
        {{ 30, 230, 100, 10 }, 1,1, GREEN },
        {{ 650, 180, 100, 10 }, 1,1, GREEN },
        {{ 350, 140, 100, 10 }, 1,1, GREEN },
        {{ 700, 90, 100, 10 }, 1,1, GREEN },
        {{ 50, 30, 100, 10 }, 1,1, GREEN },
        {{ 340, 0, 100, 10 }, 1,1, GREEN },
        {{ 550, 100, 100, 10 }, 1,1, GREEN },
        {{ 650, -400, 100, 10 }, 1,1, GREEN },
        {{ 650, -300, 100, 10 }, 1,1, GREEN },
        {{ 650, -200, 100, 10 }, 1,1, GREEN },
        {{ 650, 0, 100, 10 }, 1,1, GREEN },
        {{ 300, -200, 100, 10 }, 1,1, GREEN }, 
        {{ 300, -900, 100, 10 }, 1,1, GREEN }, 
        {{ 250, -300, 100, 10 }, 1,1, GREEN },
        {{ 200, -950, 100, 10 }, 1,1, GREEN }, 
        {{ 50, -950, 100, 10 }, 1,1, GREEN },
        {{ 600, -900, 100, 10 }, 1,1, GREEN },  
        {{ 400, -880, 100, 10 }, 1,1, GREEN },       
        {{ 700, -840, 100, 10 }, 1,1, GREEN }, 
        {{ 20, -800, 100, 10 }, 1,1, GREEN }, 
        {{ 640,-750, 100, 10 }, 1,1, GREEN },
        {{ 700, -750, 100, 10 }, 1,1, GREEN },
        {{ 150, -700, 100, 10 }, 1,1, GREEN },
        {{ 300, -650, 100, 10 }, 1,1, GREEN },
        {{ 500, -650, 100, 10 }, 1,1, GREEN },
        {{ 330, -610, 100, 10 }, 1,1, GREEN },
        {{ 600, -770, 100, 10 }, 1,1, GREEN },
        {{ 10, -640, 100, 10 }, 1,1, GREEN },
        {{ 600, -600, 100, 10 }, 1,1, GREEN },
        {{ 250, -570, 100, 10 }, 1,1, GREEN },
        {{ 650, -520, 100, 10 }, 1,1, GREEN },
        {{ 580, -450, 100, 10 }, 1,1, GREEN },
        {{ 650, -300, 100, 10 }, 1,1, GREEN },
        {{ 50, -300, 100, 10 }, 1,1, GREEN },
        {{ 300, -250, 100, 10 }, 1,1, GREEN },
        {{ 30, -230, 100, 10 }, 1,1, GREEN },
        {{ 650, -180, 100, 10 }, 1,1, GREEN },
        {{ 350, -140, 100, 10 }, 1,1, GREEN },
        {{ 700, -90, 100, 10 }, 1,1, GREEN },
        {{ 50, -30, 100, 10 }, 1,1, GREEN },
        {{ 340, 0, 100, 10 }, 1,1, GREEN },
        {{ 650, 100, 100, 10 }, 1,1, GREEN },
        {{ 650, -1000, 100, 10 }, 1,1 ,ORANGE}
    };
  

static bool isGameOver =false;
static bool inGame = false;
static int score = 0;

static  Sound fxCollision;

void UpdatePlayer(Player *player, EnvItem *envItems, int envItemsLength, float delta);
void UpdateCameraCenter(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height);
void UpdateCameraCenterInsideMap(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height);
void UpdateCameraCenterSmoothFollow(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height);
void UpdateCameraEvenOutOnLanding(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height);
void UpdateCameraPlayerBoundsPush(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height);
void InitGame(void);
void UpdateStage(Player *player, EnvItem *envItems, int envItemsLength);
void ReInitStage(EnvItem *envItems, int envItemsLength);

int main(void)
{
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
    
    //Initialisation camera 
    //---------------------------------------------------------------------------------------
    Camera2D camera = { 0 };
    camera.target = player.position;
    camera.offset = (Vector2){ screenWidth/2, screenHeight/2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // Store pointers to the multiple update camera functions
    void (*cameraUpdaters[])(Camera2D*, Player*, EnvItem*, int, float, int, int) = {
        UpdateCameraCenter,
        UpdateCameraCenterInsideMap,
        UpdateCameraCenterSmoothFollow,
        UpdateCameraEvenOutOnLanding,
        UpdateCameraPlayerBoundsPush
    };
    
    int cameraOption = 0;
    int cameraUpdatersLength = sizeof(cameraUpdaters)/sizeof(cameraUpdaters[0]);
    
    char *cameraDescriptions[] = {
        "Follow player center",
        "Follow player center, but clamp to map edges",
        "Follow player center; smoothed",
        "Follow player center horizontally; updateplayer center vertically after landing",
        "Player push camera on getting too close to screen edge"
    };
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
            if(!isGameOver && inGame){
                UpdatePlayer(&player, envItems, envItemsLength, deltaTime);
                UpdateStage(&player, envItems, envItemsLength);
                camera.zoom += ((float)GetMouseWheelMove()*0.05f);
                
                if (camera.zoom > 3.0f) camera.zoom = 3.0f;
                else if (camera.zoom < 0.25f) camera.zoom = 0.25f;
                
                if (IsKeyPressed(KEY_R)) 
                {
                    camera.zoom = 1.0f;
                    player.position = (Vector2){ 400, 280 };
                }

            }
            else if (isGameOver && inGame) {
               // camera->offset = (Vector2){ width/2, height/2 };
               
                if (IsKeyPressed(KEY_ENTER))
                {
                    InitGame(); 
                    isGameOver = false;
                    ReInitStage(envItems, envItemsLength);
                }
            }
        }
        else{
            if(IsKeyPressed(KEY_ENTER)){
                inGame = true;
            }
        }

       


       // cameraOption = (cameraOption + 1)%cameraUpdatersLength;

        // Call update camera function by its pointer
        cameraUpdaters[1](&camera, &player, envItems, envItemsLength, deltaTime, screenWidth, screenHeight); // 1 : clamp to map edge
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(LIGHTGRAY);
            if(!isGameOver && inGame){
                BeginMode2D(camera);
                
                for (int i = 0; i < envItemsLength; i++) {
                    DrawRectangleRec(envItems[i].rect, envItems[i].color);
                }

                Rectangle playerRect = { player.position.x - 20, player.position.y - 40, 40, 40 };
                DrawRectangleRec(playerRect, RED);
                
                EndMode2D();

                DrawText("Controls:", 20, 20, 10, BLACK);
                DrawText("- Right/Left to move", 40, 40, 10, DARKGRAY);
                DrawText(TextFormat("%04i", score), 700, 20, 40, GRAY);
                //DrawText("- Space to jump", 40, 60, 10, DARKGRAY);
                //DrawText("- Mouse Wheel to Zoom in-out, R to reset zoom", 40, 80, 10, DARKGRAY);
                //DrawText("- C to change camera mode", 40, 100, 10, DARKGRAY);
                //DrawText("Current camera mode:", 20, 120, 10, BLACK);
                
                }
               
            else if (isGameOver==true) {
                DrawText ("GAME OVER",20, 120, 50, BLACK);
                
            }
            else if (inGame == false ) {
                DrawText("press enter to start ", 20, 120, 10, BLACK);
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
    
    if (IsKeyDown(KEY_LEFT)) player->position.x -= PLAYER_HOR_SPD*delta;
    if (IsKeyDown(KEY_RIGHT)) player->position.x += PLAYER_HOR_SPD*delta;
    
    if(player->position.x > screenWidth) {			//	if the player jumps to the right border of the window, move the player to the left border
        player->position.x = player->position.x-screenWidth;
    }
    if(player->position.x < 0) {			//	if the player jumps to the right border of the window, move the player to the left border
        player->position.x = player->position.x+screenWidth;
	}
    if (player->canJump) 
    {
        player->speed = -PLAYER_JUMP_SPD;
        player->timeSinceJump = GetTime();
        player->canJump = false;
        PlaySound(fxCollision);
    }
    if(GetTime() - player->timeSinceJump > 5){
        isGameOver=true;
       // printf("ligne 167");
    }
    int hitObstacle = 0;
    for (int i = 0; i < envItemsLength; i++) 
    {
        EnvItem *ei = envItems + i;
        Vector2 *p = &(player->position);
        if (ei->blocking &&
            ei->rect.x <= p->x && 
            ei->rect.x + ei->rect.width >= p->x &&
            ei->rect.y >= p->y &&
            ei->rect.y < p->y + player->speed*delta &&
            ei->actif)
        {
            hitObstacle = 1;
            player->speed = 0.0f;
            p->y = ei->rect.y;
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

void UpdateCameraCenter(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height)
{
    camera->offset = (Vector2){ width/2, height/2 };
    camera->target = player->position;
}

void UpdateCameraCenterInsideMap(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height)
{
    camera->target = player->position;
    camera->offset = (Vector2){ width/2, height/2 };
    float minX = 1000, minY = 1000, maxX = -1000, maxY = -1000;
    
    for (int i = 0; i < envItemsLength; i++)
    {
        EnvItem *ei = envItems + i;
        minX = fminf(ei->rect.x, minX);
        maxX = fmaxf(ei->rect.x + ei->rect.width, maxX);
        minY = fminf(ei->rect.y, minY);
        maxY = fmaxf(ei->rect.y + ei->rect.height, maxY);
    }
    
    Vector2 max = GetWorldToScreen2D((Vector2){ maxX, maxY }, *camera);
    Vector2 min = GetWorldToScreen2D((Vector2){ minX, minY }, *camera);
    
    if (max.x < width) camera->offset.x = width - (max.x - width/2);
    if (max.y < height) camera->offset.y = height - (max.y - height/2);
    if (min.x > 0) camera->offset.x = width/2 - min.x;
    if (min.y > 0) camera->offset.y = height/2 - min.y;
}

void UpdateCameraCenterSmoothFollow(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height)
{
    static float minSpeed = 30;
    static float minEffectLength = 10;
    static float fractionSpeed = 0.8f;
    
    camera->offset = (Vector2){ width/2, height/2 };
    Vector2 diff = Vector2Subtract(player->position, camera->target);
    float length = Vector2Length(diff);
    
    if (length > minEffectLength)
    {
        float speed = fmaxf(fractionSpeed*length, minSpeed);
        camera->target = Vector2Add(camera->target, Vector2Scale(diff, speed*delta/length));
    }
}

void UpdateCameraEvenOutOnLanding(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height)
{
    static float evenOutSpeed = 700;
    static int eveningOut = false;
    static float evenOutTarget;
    
    camera->offset = (Vector2){ width/2, height/2 };
    camera->target.x = player->position.x;
    
    if (eveningOut)
    {
        if (evenOutTarget > camera->target.y) 
        {
            camera->target.y += evenOutSpeed*delta;
            
            if (camera->target.y > evenOutTarget) 
            {
                camera->target.y = evenOutTarget;
                eveningOut = 0;
            }
        } 
        else 
        {
            camera->target.y -= evenOutSpeed*delta;
            
            if (camera->target.y < evenOutTarget) 
            {
                camera->target.y = evenOutTarget;
                eveningOut = 0;
            }
        }
    } 
    else 
    {
        if (player->canJump && (player->speed == 0) && (player->position.y != camera->target.y))
        {
            eveningOut = 1;
            evenOutTarget = player->position.y;
        }
    }
}

void UpdateCameraPlayerBoundsPush(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height) 
{
    static Vector2 bbox = { 0.2f, 0.2f };

    Vector2 bboxWorldMin = GetScreenToWorld2D((Vector2){ (1 - bbox.x)*0.5f*width, (1 - bbox.y)*0.5f*height }, *camera);
    Vector2 bboxWorldMax = GetScreenToWorld2D((Vector2){ (1 + bbox.x)*0.5f*width, (1 + bbox.y)*0.5f*height }, *camera);
    camera->offset = (Vector2){ (1 - bbox.x)*0.5f * width, (1 - bbox.y)*0.5f*height };

    if (player->position.x < bboxWorldMin.x) camera->target.x = player->position.x;
    if (player->position.y < bboxWorldMin.y) camera->target.y = player->position.y;
    if (player->position.x > bboxWorldMax.x) camera->target.x = bboxWorldMin.x + (player->position.x - bboxWorldMax.x);
    if (player->position.y > bboxWorldMax.y) camera->target.y = bboxWorldMin.y + (player->position.y - bboxWorldMax.y);
}
void InitGame()
{
    
    player.position = (Vector2){ 350, 800 };
    player.speed = 0;
    player.canJump = false;
    player.timeSinceJump = GetTime();


}
void UpdateStage(Player *player, EnvItem *envItems, int envItemsLength)
{
    for(int i =1; i<envItemsLength; i++){
        if (envItems[i].rect.y  > player->position.y+screenHeight/2+50){
            envItems[i].actif = 0 ;
            envItems[i].color = DARKBROWN;
        }
    }
}
void ReInitStage(EnvItem *envItems, int envItemsLength)
{
    for(int i =0; i<envItemsLength; i++){    
            envItems[i].actif = 1 ;
            if (i>1) envItems[i].color = GREEN;
            
    }
    envItems[1].color = GRAY ;
    envItems[envItemsLength-1].color= ORANGE;
}