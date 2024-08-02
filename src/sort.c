#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "SDL.h"
#include "SDL_events.h"
//#include "SDL_rect.h"

SDL_Color hsv2rgb(double h, double s, double v);

void use_texture(SDL_Renderer* ren) {
    SDL_Texture* tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, 800, 600);
    unsigned char* pixels;
    int pitch;
    SDL_LockTexture(tex, NULL, (void**)&pixels, &pitch);
    for (int i = 0; i < 100000; i++) {
        int x = rand() % (800 * 4);
        int y = rand() % 600;
        int offset = y * pitch + x;
        pixels[offset] = rand();
    }
    SDL_UnlockTexture(tex);
    SDL_RenderCopy(ren, tex, NULL, NULL);
    SDL_DestroyTexture(tex);
}

void use_surface(SDL_Renderer* ren) {
    SDL_Surface* surf = SDL_CreateRGBSurfaceWithFormat(0, 800, 600, 32, SDL_PIXELFORMAT_RGBA32);
    for (int i = 0; i < 100000; i++) {
        int x = rand() % (800 * 4);
        int y = rand() % 600;
        int offset = y * surf->pitch + x;
        ((unsigned char*)surf->pixels)[offset] = 255;
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);
    SDL_RenderCopy(ren, tex, NULL, NULL);
    SDL_DestroyTexture(tex);
    SDL_FreeSurface(surf);
}

void use_draw(SDL_Renderer* ren) {
    for (int i = 0; i < 100000; i++) {
        int x = rand() % 800;
        int y = rand() % 600;
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderDrawPoint(ren, x, y);
    }
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *win = SDL_CreateWindow("", 100, 100, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);
    srand(time(NULL));
    SDL_Event e;
    int quit = 0;
    int steps = 0;
    int t_fps = clock();
    int mode = 0;
    while (!quit) {
        steps++;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                mode = !mode;
            }
        }
        SDL_Rect r = {100, 100, 200, 200};
        if (mode == 0) {
            use_texture(ren);
        } else {
            use_surface(ren);
        }
        SDL_RenderPresent(ren);
        SDL_Delay(1);
        if (clock() - t_fps > 1000) {
            double fps = steps * 1000. / (clock() - t_fps);
            char s[100];
            sprintf(s, "%.1lf", fps);
            SDL_SetWindowTitle(win, s);
            t_fps = clock();
            steps = 0;
        }
    }
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

SDL_Color hsv2rgb(double h, double s, double v) {
    double hh, p, q, t, ff;
    int vv, pp, qq, tt;
    long i;
    SDL_Color out = {0, 0, 0, 0xFF};

    if(s <= 0.0) {       // < is bogus, just shuts up warnings
        out.r = v;
        out.g = v;
        out.b = v;
        return out;
    }
    hh = h;
    while(hh >= 360.0) hh -= 360;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = v * (1.0 - s);
    q = v * (1.0 - (s * ff));
    t = v * (1.0 - (s * (1.0 - ff)));
    vv = (int)round((v * 255));
    pp = (int)round((p * 255));
    qq = (int)round((q * 255));
    tt = (int)round((t * 255));

    switch(i) {
    case 0:
        out.r = vv;
        out.g = tt;
        out.b = pp;
        break;
    case 1:
        out.r = qq;
        out.g = vv;
        out.b = pp;
        break;
    case 2:
        out.r = pp;
        out.g = vv;
        out.b = tt;
        break;
    case 3:
        out.r = pp;
        out.g = qq;
        out.b = vv;
        break;
    case 4:
        out.r = tt;
        out.g = pp;
        out.b = vv;
        break;
    case 5:
    default:
        out.r = vv;
        out.g = pp;
        out.b = qq;
        break;
    }
    return out;
}