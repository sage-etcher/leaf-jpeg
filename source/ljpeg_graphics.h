/*
   source/graphics.h
   LJPEG SDL Graphics header. 
 
   Copyright 2023 Sage I. Hendricks

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. 

*/


/* run once */
#pragma once
#ifndef __LJPEG_GRAPHICS_HEADER__
#define __LJPEG_GRAPHICS_HEADER__

/* include headers */
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

#include "ljpeg_config.h"


/* custom datatypes */
typedef struct texture 
{
    SDL_RWops   *rwop;
    SDL_Texture *texture;
    SDL_Rect     source;
    double       scale;
    int          rotation;
    SDL_Rect     projection;
    SDL_Rect     display;
} texture;


/* constants */
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#define CLOCKWISE 1
#define COUNTERCLOCKWISE -1


/* global variables */
extern SDL_Window   *g_win;
extern SDL_Renderer *g_rend;
extern texture       g_img;


/* external function prototypes */
int graphics_init_sdl     (void);
int graphics_init_window  (void);
int graphics_load_texture (const char *filename);

void graphics_project (texture *tex);
void graphics_render  (SDL_Renderer *rend, texture *tex);
void graphics_manual_move_window (void);
void graphics_texture_rotate (texture *tex, int direction);

#endif /* end run once */


/* End of File */
