/***********************************
    raycaster.c
    Artur Rodrigues Rocha Neto
    artur.rodrigues26@gmail.com
***********************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define MAP_WIDTH 24
#define MAP_HEIGHT 24

const int worldMap[MAP_WIDTH][MAP_HEIGHT] = {
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

const SDL_Color RGB_Red = {255, 0, 0, 255};
const SDL_Color RGB_Green = {0, 255, 0, 255};
const SDL_Color RGB_Blue = {0, 0, 255, 255};
const SDL_Color RGB_White = {255, 255, 255, 255};
const SDL_Color RGB_Yellow = {255, 255, 0, 255};

struct str_vector {
    float x;
    float y;
};
typedef struct str_vector Vector;

Vector* Vector_Zero() {
    Vector* vec = NULL;
    
    vec = malloc(sizeof(Vector));
    if (vec == NULL) {
        return NULL;
    }
    
    vec->x = 0.0f;
    vec->y = 0.0f;
    
    return vec;
}

Vector* Vector_New(float x, float y) {
    Vector* vec = NULL;
    
    vec = malloc(sizeof(Vector));
    if (vec == NULL) {
        return NULL;
    }
    
    vec->x = x;
    vec->y = y;
    
    return vec;
}

void Vector_Destroy(Vector* v) {
    free(v);
}

void Vector_Add(Vector* v1, Vector* v2) {
    v1->x += v2->x;
    v1->y += v2->y;
}

void Vector_Sub(Vector* v1, Vector* v2) {
    v1->x -= v2->x;
    v1->y -= v2->y;
}

float Vector_Dot(Vector* v1, Vector* v2) {
    return v1->x * v2->x + v1->y * v2->y;
}

float Vector_Distance(Vector* v1, Vector* v2) {
    return sqrt(pow(v1->x - v2->x, 2) + pow(v1->y - v2->y, 2));
}

float Vector_Length(Vector* v) {
    return sqrt(pow(v->x, 2) + pow(v->y, 2));
}

Vector* Vector_Unit(Vector* v) {
    Vector* vec = NULL;
    
    vec = malloc(sizeof(Vector));
    if (vec == NULL) {
        return NULL;
    }
    
    float len = Vector_Length(v);
    
    vec->x = v->x / len;
    vec->y = v->y / len;
    
    return vec;
}

struct str_game {
    SDL_Window* window;
    SDL_Renderer* renderer;
    int width;
    int height;
    SDL_bool running;
    Vector* playerPos;
    Vector* playerDir;
    Vector* camera;
    float time;
    float oldTime;
    float moveSpeed;
    float rotSpeed;
    float delta;
};
typedef struct str_game Game;

Game* Game_Create(int width, int height, float pX, float pY) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("ERRO: Game_Create SDL_Init\n");
        return NULL;
    }
    
    Game* game = NULL;
    
    game = malloc(sizeof(Game));
    if (game == NULL) {
        printf("ERRO: Game_Create malloc\n");
        return NULL;
    }
    
    game->width = width;
    game->height = height;
    game->running = SDL_TRUE;
    game->playerPos = Vector_New(pX, pY);
    game->playerDir = Vector_New(-1.0f, 0.0f);
    game->camera = Vector_New(0.0f, 0.66f);
    game->time = 0.0f;
    game->oldTime = 0.0f;
    game->moveSpeed = 0.0f;
    game->rotSpeed = 0.0f;
    game->delta = 0.0f;
    
    game->window = SDL_CreateWindow("Raycaster",
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    width,
                                    height,
									SDL_WINDOW_SHOWN);
    if (game->window == NULL) {
        printf("ERRO: Game_Create SDL_CreateWindow\n");
        free(game);
        return NULL;
    }
    
    game->renderer = SDL_CreateRenderer(game->window,
                                        -1,
                                        SDL_RENDERER_ACCELERATED);
    
    if (game->renderer == NULL) {
        printf("ERRO: Game_Create SDL_CreateRenderer\n");
        SDL_DestroyWindow(game->window);
        free(game);
        return NULL;
    }
    
    return game;
}

void Game_Destroy(Game* game) {
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    Vector_Destroy(game->playerPos);
    Vector_Destroy(game->playerDir);
    Vector_Destroy(game->camera);
    free(game);
}

void Game_Poll(Game* game) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            game->running = SDL_FALSE;
        }
        
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.scancode == SDLK_UP) {
                if (worldMap[(int)(game->playerPos->x + game->playerDir->x * game->moveSpeed)][(int)game->playerPos->y] == 0) {
                    game->playerPos->x += game->playerDir->x * game->moveSpeed;
                }
                if (worldMap[(int)game->playerPos->x][(int)(game->playerPos->y - game->playerDir->y * game->moveSpeed)] == 0) {
                    game->playerPos->y += game->playerDir->y * game->moveSpeed;
                }
            }
            if (e.key.keysym.scancode == SDLK_DOWN) {
                // asdf
            }
            if (e.key.keysym.scancode == SDLK_LEFT) {
                // asdf
            }
            if (e.key.keysym.scancode == SDLK_RIGHT) {
                // asdf
            }
        }
    }
}

void Game_Update(Game* game) {
    game->oldTime = game->time;
    game->time = SDL_GetTicks();
    game->delta = (game->time - game->oldTime) / 1000.0f;
    game->moveSpeed = game->delta * 5.0f;
    game->rotSpeed = game->delta * 3.0f;
}

void Game_Draw(Game* game) {
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->renderer);
    
    for (int x = 0; x < game->width; x++) {
        float cameraX = 2 * x / (float)game->width - 1;
        float rayDirX = game->playerDir->x + game->camera->x * cameraX;
        float rayDirY = game->playerDir->y + game->camera->y * cameraX;
        
        int mapX = (int)game->playerPos->x;
        int mapY = (int)game->playerPos->y;
        
        float sideDistX;
        float sideDistY;
        
        float deltaDistX = (rayDirY == 0) ? 0 : ((rayDirX == 0) ? 1 : fabs(1 / rayDirX));
        float deltaDistY = (rayDirX == 0) ? 0 : ((rayDirY == 0) ? 1 : fabs(1 / rayDirY));
        float perpWallDist;
        
        int stepX;
        int stepY;
        
        int hit = 0;
        int side;
        
        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (game->playerPos->x - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0f -  game->playerPos->x) * deltaDistX;
        }
        
        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (game->playerPos->y - mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (mapY + 1.0f - game->playerPos->y) * deltaDistY;
        }
        
        while (hit == 0) {
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            
            if (worldMap[mapX][mapY] > 0) {
                hit = 1;
            }
        }
        
        if (side == 0) {
            perpWallDist = (mapX - game->playerPos->x + (1 - stepX) / 2) / rayDirX;
        } else {
            perpWallDist = (mapY - game->playerPos->y + (1 - stepY) / 2) / rayDirY;
        }
        
        int lineHeigt = (int)(game->height / perpWallDist);
        
        int drawStart = -lineHeigt / 2 + game->height / 2;
        if (drawStart < 0) {
            drawStart = 0;
        }
        
        int drawEnd = lineHeigt / 2 + game->height / 2;
        if (drawEnd >= game->height) {
            drawEnd = game->height - 1;
        }
        
        SDL_Color color = RGB_White;
        
        switch(worldMap[mapX][mapY]) {
            case 1: color = RGB_Red; break;
            case 2: color = RGB_Green; break;
            case 3: color = RGB_Blue; break;
            case 4: color = RGB_White; break;
            default: color = RGB_Red; break;
        }
        
        if (side == 1) {
            color.r /= 2;
            color.g /= 2;
            color.b /= 2;
        }
        
        SDL_SetRenderDrawColor(game->renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawLine(game->renderer, x, drawStart, x, drawEnd);
    }
    
    SDL_RenderPresent(game->renderer);
}

void Game_Run(Game* game) {
    while (game->running) {
        Game_Poll(game);
        Game_Update(game);
        Game_Draw(game);
    }
}

int main(int argc, char** argv) {
    Game* game = Game_Create(640, 480, 20, 12);
    Game_Run(game);
    Game_Destroy(game);
    
    return 0;
}
