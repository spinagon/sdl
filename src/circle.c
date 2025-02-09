#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void draw_circle_lines(SDL_Renderer *ren, double x, double y, double r, int sides) {
    SDL_FPoint points[sides + 1];
    for (int i = 0; i < sides; i++) {
        points[i].x = x + r * cos(2 * M_PI * (double)i / sides);
        points[i].y = y + r * sin(2 * M_PI * (double)i / sides);
    }
    points[sides] = points[0];
    SDL_RenderDrawLinesF(ren, points, sides + 1);
}

void draw_circle_geo(SDL_Renderer *ren, double x, double y, double r, int sides) {
    SDL_Vertex points[sides];
    for (int i = 0; i < sides; i++) {
        points[i].position.x = x + r * cos(2 * M_PI * (double)i / sides);
        points[i].position.y = y + r * sin(2 * M_PI * (double)i / sides);
        points[i].color.r = 255;
        points[i].color.g = 0;
        points[i].color.b = 0;
        points[i].color.a = 255;
        points[i].tex_coord.x = 0;
        points[i].tex_coord.y = 0;
    }

    SDL_RenderGeometry(ren, NULL, points, sides, NULL, 0);
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
    }
    SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 1000, 1000, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Event e;
    int quit = 0;
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);
    srand(time(NULL));
    int t = clock();
    int n = 0;
    double x[1000];
    double y[1000];
    double xs[1000];
    double ys[1000];
    int count = 0;
    while (!quit) {
        count++;
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                x[n] = e.button.x;
                y[n] = e.button.y;
                n++;
            }
        }
        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
        for (int i = 0; i < n; i++) {
            xs[i] = 0;
            ys[i] = 0;
            for (int j = 0; j < n; j++) {
                if (i == j) continue;
                double dx = x[i] - x[j];
                double dy = y[i] - y[j];
                double angle = atan2(dy, dx);
                double mag = 1. / hypot(dx, dy);
                xs[i] -= mag * cos(angle) * 1;
                ys[i] -= mag * sin(angle) * 1;
                xs[i] += mag * mag * cos(angle) * 100;
                ys[i] += mag * mag * sin(angle) * 100;
            }
        }
        for (int i = 0; i < n; i++) {
            draw_circle_lines(ren, x[i], y[i], 5, 8);
            x[i] += xs[i];
            y[i] += ys[i];
        }
        if (clock() - t > 10000) {
            SDL_RenderPresent(ren);
            t = clock();
            count = 0;
        }
    }
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
