#include "raylib.h"
#include <stdio.h>

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) > (y) ? (y) : (x))

#define BIRD_WIDTH 50
#define BIRD_HEIGHT 30
#define STRENGHT -10
#define GRAVITY 0.7

#define MAX_PIPES 2 
#define PIPE_WIDTH 60
#define PIPE_HEIGHT 400 
#define PIPE_GAP 160

#define SKY_COLOR (Color) {173, 216, 230}

typedef struct {
    Rectangle rect;
    float vel;
    Color color;
} Bird;

typedef struct {
    Rectangle rect;
    Color color;
} Pipe;

// Don't even think about changing this 
static const int screenWidth  = 400;
static const int screenHeight = 700;
static Bird bird;
static bool gameOver = false;

static Sound die = { 0 }; 
static Sound hit = { 0 }; 
static Sound point = { 0 }; 
static Sound swoosh = { 0 };
static Sound wing = { 0 }; 

static float scrollSpeed = -200.0f;
static int score = 0;
static Pipe pipes[MAX_PIPES] = { 0 };


void InitGame(void);
void UpdateDrawFrame(void);
void UpdateGame(void);
void DrawGame(void);
void LoadGame(void);
void UnloadGame(void);


int main(void)
{
    InitWindow(screenWidth, screenHeight, "raylib: flappy bird!");
    InitAudioDevice();
    LoadGame();
    InitGame();
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }

    UnloadGame();
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

void InitGame(void)
{
    gameOver = false;
    score = 0;
    scrollSpeed = -200.0f;
    bird.rect = (Rectangle){BIRD_WIDTH, (screenWidth / 2) + BIRD_HEIGHT, BIRD_WIDTH, BIRD_HEIGHT};
    bird.vel  = 0.0f;
    bird.color= YELLOW;

    for (int i = 0; i < MAX_PIPES * 2; i += 2) {
        pipes[i].rect = (Rectangle){screenWidth + 250 * i / 2, -GetRandomValue(0, 200), PIPE_WIDTH, PIPE_HEIGHT};
        pipes[i].color = BROWN;

        pipes[i + 1].rect = (Rectangle){pipes[i].rect.x, pipes[i].rect.y + PIPE_HEIGHT + PIPE_GAP, PIPE_WIDTH, PIPE_HEIGHT};
        pipes[i + 1].color = GRAY;
    }


}

void UpdateDrawFrame(void)
{

    UpdateGame();
    DrawGame();
}

void UpdateGame(void)
{
    if (!gameOver) {
        if ((score + 1) % 10 == 0) scrollSpeed -= 2; 
        // Update Pipes x
        for (int i = 0; i < MAX_PIPES * 2; i++) {
            pipes[i].rect.x += scrollSpeed * GetFrameTime();
            if (pipes[i].rect.x <= -PIPE_WIDTH) {
                    pipes[i].rect.x = screenWidth;
                   if (i % 2 == 0) {
                        pipes[i].rect.y = -GetRandomValue(0, 200);
                        pipes[i + 1].rect.y = pipes[i].rect.y + PIPE_HEIGHT + PIPE_GAP;
                        PlaySound(point);
                        score++;
                    }
            }
        }
        // FLAP 
        if (IsKeyPressed(KEY_SPACE)) {
            PlaySound(wing);
            bird.vel = STRENGHT;
        }
    
        // pipes
        for (int i = 0; i < MAX_PIPES * 2; i++) {
            if (CheckCollisionRecs(bird.rect, pipes[i].rect)) {
                PlaySound(hit);
                PlaySound(die);
                gameOver = true;
            }
        }

        if(bird.rect.y >= (screenHeight - (BIRD_HEIGHT + 50))) {
            PlaySound(hit);
            gameOver = true;
         }
 
    }

    // Apply gravity
    bird.vel += GRAVITY;
    if (bird.vel >= 8)
        bird.vel = 8;
    bird.rect.y += bird.vel;
    bird.rect.y = MAX(0, MIN(bird.rect.y, (screenHeight - bird.rect.height - 50)));

    if (gameOver && IsKeyPressed(KEY_R))
        InitGame();
}

void DrawGame(void)
{
    BeginDrawing();
        ClearBackground(SKY_COLOR);
        for (int i = 0; i < MAX_PIPES * 2; i++) DrawRectangleRec(pipes[i].rect, pipes[i].color);
        DrawRectangle(0, screenHeight - 50, screenWidth, 50, DARKGREEN);
        DrawRectangleRec(bird.rect, bird.color);
        if (!gameOver) DrawText(TextFormat("%i", score), screenWidth / 2, 10,  40, WHITE);
        if (gameOver) {
            DrawText("Game Over",90 , 100, 40, YELLOW);
            DrawRectangle(75, 150, 250, 200,LIGHTGRAY);
            DrawText(TextFormat("Score: %i", score), 100, 200, 40, WHITE);
            DrawText("Press r To reset!", 90, 300, 22, WHITE);
        }
    EndDrawing();
 
}

void LoadGame(void)
{
    die    = LoadSound("./Sounds/die.wav");
    hit    = LoadSound("./Sounds/hit.wav");
    point  = LoadSound("./Sounds/point.wav");
    swoosh = LoadSound("./Sounds/swoosh.wav");
    wing   = LoadSound("./Sounds/wing.wav");

}

void UnloadGame(void)
{
    // die.wav  hit.wav  point.wav  swoosh.wav  wing.wav 
    UnloadSound(die);
    UnloadSound(hit);
    UnloadSound(point);
    UnloadSound(swoosh);
    UnloadSound(wing);
}
