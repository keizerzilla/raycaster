/***********************************
    raycaster.c
    Artur Rodrigues Rocha Neto
    artur.rodrigues26@gmail.com
***********************************/

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

struct str_vector {
    float x;
    float y;
};
typedef struct str_vector Vector;

struct str_game {
    SDL_Window* window;
    SDL_Renderer* renderer;
    int width;
    int height;
    SDL_bool running;
};
typedef struct str_game Game;

Game* Game_Create(int width, int height) {
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
    free(game);
    game = NULL;
}

void Game_Poll(Game* game) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            game->running = SDL_FALSE;
        }
    }
}

void Game_Update(Game* game) {

}

void Game_Draw(Game* game) {
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->renderer);
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
    Game* game = Game_Create(640, 480);
    Game_Run(game);    
    Game_Destroy(game);
        
    return 0;
}

