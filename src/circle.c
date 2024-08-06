#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "SDL.h"
#include "SDL_events.h"
#include "SDL_rect.h"

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

double clip(double n, double min, double max) {
    if (n > max) return max;
    if (n < min) return min;
    return n;
}

void draw_circle_lines(SDL_Renderer *ren, double x, double y, double r, int sides) {
    SDL_FPoint points[sides + 1];
    for (int i = 0; i < sides; i++) {
        points[i].x = x + r * cos(2 * M_PI * (double)i / sides);
        points[i].y = y + r * sin(2 * M_PI * (double)i / sides);
    }
    points[sides] = points[0];
    SDL_RenderDrawLinesF(ren, points, sides + 1);
}

void draw_circle_geo(SDL_Renderer *ren, double x, double y, double r, int sides, SDL_Color c) {
    SDL_Vertex points[sides];
    for (int i = 0; i < sides; i++) {
        points[i].position.x = x + r * cos(2 * M_PI * (double)i / sides);
        points[i].position.y = y + r * sin(2 * M_PI * (double)i / sides);
        points[i].color = c;
    }
    int indices[3 * (sides - 2)];
    for (int i = 0; i < sides - 2; i++) {
        indices[i * 3] = 0;
        indices[i * 3 + 1] = i + 1;
        indices[i * 3 + 2] = i + 2;
    }
    SDL_RenderGeometry(ren, NULL, points, sides, indices, 3 * (sides - 2));
    SDL_free(points);
}

int main(int argc, char* argv[]) {
    int res;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
    }
    SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (win == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_Event e;
    int quit = 0;
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);
    srand(time(NULL));
    int max_n = 1000;
    double x[max_n];
    double y[max_n];
    double xs[max_n];
    double ys[max_n];
    int n = 0;
    int r = rand() % 256;
    int g = rand() % 256;
    int b = rand() % 256;
    int w = 800;
    int h = 600;
    int t = clock();
    int t_fps = clock();
    int steps = 0;
    double attr_strength = 0;
    double attr_x = 0;
    double attr_y = 0;
    while (!quit) {
        steps++;
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (e.button.button == 1) {
                    x[n] = e.button.x;
                    y[n] = e.button.y;
                    xs[n] = 0.0;
                    ys[n] = 0.0;
                    n++;
                } else {
                    attr_strength = 1;
                    attr_x = e.button.x;
                    attr_y = e.button.y;
                }
            }
            if (e.type == SDL_MOUSEBUTTONUP) {
                attr_strength = 0;
            }
            if (e.type == SDL_MOUSEMOTION) {
                attr_x = e.motion.x;
                attr_y = e.motion.y;
            }
        }
        SDL_SetRenderDrawColor(ren, r % 256, g % 256, b % 256, 255);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                double dx = x[i] - x[j];
                double dy = y[i] - y[j];
                if (fabs(dx) > 1 || fabs(dy) > 1) {
                    double angle = atan2(dy, dx);
                    double mag = 1 / hypot(dx, dy);
                    xs[i] -= pow(mag, 2) * cos(angle) * 1e1;
                    ys[i] -= pow(mag, 2) * sin(angle) * 1e1;
                    xs[i] += pow(mag * 10, 5) * cos(angle) * 1e1;
                    ys[i] += pow(mag * 10, 5) * sin(angle) * 1e1;
                }

                dx = x[i] - attr_x;
                dy = y[i] - attr_y;
                if (fabs(dx) > 10 || fabs(dy) > 10){
                    double angle = atan2(dy, dx);
                    double mag = 1 / hypot(dy, dx);
                    xs[i] -= mag * attr_strength * cos(angle) * 1e-1;
                    ys[i] -= mag * attr_strength * sin(angle) * 1e-1;
                }
            }
        }
        for (int i = 0; i < n; i++) {
            SDL_Color c = hsv2rgb(cos(xs[i])*1000 + sin(ys[i])*1000, 0.8, 1);
            draw_circle_geo(ren, x[i], y[i], 5, 9, c);
            x[i] += xs[i];
            y[i] += ys[i];
            xs[i] *= 0.98;
            ys[i] *= 0.98;
            xs[i] = clip(xs[i], -1e0, 1e0);
            ys[i] = clip(ys[i], -1e0, 1e0);
            if (x[i] < 0) {
                xs[i] = fabs(xs[i]);
                x[i] = 0;
            }
            if (x[i] > w) {
                xs[i] = -fabs(xs[i]);
                x[i] = w;
            }
            if (y[i] < 0) {
                ys[i] = fabs(ys[i]);
                y[i] = 0;
            }
            if (y[i] > h) {
                ys[i] = -fabs(ys[i]);
                y[i] = h;
            }
        }
        if (clock() - t > 10) {
            SDL_RenderPresent(ren);
            SDL_GetRendererOutputSize(ren, &w, &h);
            t = clock();
        }
        if (clock() - t_fps > 1000) {
            double fps = steps * 1000. / (clock() - t_fps);
            char s[100];
            sprintf(s, "%.1lf", fps);
            SDL_SetWindowTitle(win, s);
            t_fps = clock();
            steps = 0;
        }
        SDL_Delay(1);
    }
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
