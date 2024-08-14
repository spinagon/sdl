#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "SDL.h"
#include "SDL_events.h"
#include "SDL_rect.h"

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *win = SDL_CreateWindow("", 100, 100, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);
    srand(time(NULL));
    SDL_Event e;
    int quit = 0;
    int w = 600;
    int h = 400;
    int steps = 0;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);
        int old_x = 0;
        int old_y = 0;
        for (int x = 0; x < w; x++) {
            double y = sin(x / 10.) * sin(steps / 17.) + cos(x / 10.) * sin(steps / 111.);
            double coord_y = y * 100 + (h / 2);
            SDL_SetRenderDrawColor(ren, x % 256, 255, 255, 255);
            SDL_RenderDrawLineF(ren, old_x, old_y, x, coord_y);
            old_x = x;
            old_y = coord_y;
        }
        SDL_RenderPresent(ren);
        SDL_Delay(16);
        steps++;
    }
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}