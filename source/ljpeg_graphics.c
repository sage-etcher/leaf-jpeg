/*
   source/ljpeg_graphics.c
   LJPEG SDL Graphics source code. 
 
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


#include "ljpeg_graphics.h"

/* include headers */
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

#include "ljpeg_config.h"


/* global variable declarations */
SDL_Window   *g_win;
SDL_Renderer *g_rend;
texture       g_img;


/* file static variables */


/* file static function prototypes */
static double deg2rad (double deg);
static double rad2deg (double rad);
static void log_sdl_error (const char *string_template);

/* static function definitions */
static double 
rad2deg (double rad)
{
    return ((rad/M_PI) * 180);
}

static double 
deg2rad (double deg)
{
    return ((deg/180) * M_PI);
}

static void
log_sdl_error (const char *string_template)
{
    fprintf (stderr, "%s: %s\n", string_template, SDL_GetError ());
    fflush (stderr);
}


/* function definitions */
int
graphics_init_sdl (void)
{
    /* try to initialize SDL2 */
    if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
        log_sdl_error ("could not initialize SDL2");
        goto graphics_init_sdl_failure_0;
    }

graphics_init_sdl_success_0:
    return EXIT_SUCCESS;

graphics_init_sdl_failure_0:
    return EXIT_FAILURE;
}


int
graphics_init_window (void)
{
    /* try to create an empty window */
    g_win = SDL_CreateWindow ("LJPEG Image Viewer",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              0, 0,
                              SDL_WINDOW_HIDDEN | SDL_WINDOW_BORDERLESS);
    /* check that the window was created successfully */
    if (g_win == NULL)
    {
        log_sdl_error ("could not create window");
        goto graphics_init_window_failure_0;
    } 


    /* try to create a renderer for the empty window */
    g_rend = SDL_CreateRenderer (g_win, -1, SDL_RENDERER_ACCELERATED);
    /* check that it was created propperly */
    if (g_rend == NULL)
    {
        log_sdl_error ("could not create renderer");
        goto graphics_init_window_failure_1;
    }


graphics_init_window_success_0:
    return EXIT_SUCCESS;

graphics_init_window_failure_1:
    SDL_DestroyWindow (g_win);
graphics_init_window_failure_0:
    return EXIT_FAILURE;
}


int
graphics_load_texture (const char *filename)
{
    /* RWop */ 
    g_img.rwop = SDL_RWFromFile (filename, "rb");
    if (g_img.rwop == NULL)
    {
        log_sdl_error ("could not load texture");
        goto graphics_load_texture_failure_0;
    }

    /* Load Texture */
    g_img.texture = IMG_LoadTexture_RW (g_rend, g_img.rwop, 0);
    if (g_img.texture == NULL)
    {
        log_sdl_error ("could not load texture");
        goto graphics_load_texture_failure_1;
    }

    /* set texture sizeing */
    SDL_QueryTexture (g_img.texture, NULL, NULL, &(g_img.source.w), &(g_img.source.h));
    
    /* set default positioning */ 
    g_img.source.x = 0;
    g_img.source.y = 0;

    /* set default rotation and scale */
    g_img.scale    = INITIAL_SCALE;
    g_img.rotation = 0;

    /* project the texture onto projection */
    graphics_project (&g_img);

 
graphics_load_texture_success_0:
    return EXIT_SUCCESS; 
    
graphics_load_texture_failure_1:
    SDL_RWclose (g_img.rwop);
graphics_load_texture_failure_0:
    return EXIT_FAILURE;
}


void
graphics_project (texture *tex)
{
    /* double new_w, new_h; */

    /* scale the texture */
    tex->projection.w = tex->source.w * tex->scale;
    tex->projection.h = tex->source.h * tex->scale;
   
    /* calculate new 0,0 offset after rotate */
    tex->projection.y = abs(((tex->rotation % 180) / 90)) * ((tex->projection.w / 2) - (tex->projection.h / 2));
    tex->projection.x = abs(((tex->rotation % 180) / 90)) * ((tex->projection.h / 2) - (tex->projection.w / 2));

    /* calculate new display dimentions for the rotated image */
    tex->display.w = fabs (cos (deg2rad (tex->rotation)) * tex->projection.w - sin (deg2rad (tex->rotation)) * tex->projection.h);
    tex->display.h = fabs (sin (deg2rad (tex->rotation)) * tex->projection.w - cos (deg2rad (tex->rotation)) * tex->projection.h);

    return;   
}


void
graphics_render (SDL_Renderer *rend, texture *tex)
{
    graphics_project (tex);
    SDL_SetWindowSize (g_win, (int)g_img.display.w, (int)g_img.display.h);
    SDL_RenderCopyEx (rend, tex->texture, NULL, &tex->projection, tex->rotation, NULL, SDL_FLIP_NONE);
}


void
graphics_manual_move_window (void)
{
    int mouse_button_state;
    int mouse_x_prev, mouse_y_prev;
    int mouse_x, mouse_y;
    int window_x, window_y;

    /* get initial mouse state */
    mouse_button_state = SDL_GetGlobalMouseState (&mouse_x_prev, &mouse_y_prev);
 
    /* while the left mouse button is held down */ 
    while (mouse_button_state & SDL_BUTTON (SDL_BUTTON_LEFT))
    {
        /* get the current mouse state */
        mouse_button_state = SDL_GetGlobalMouseState (&mouse_x, &mouse_y);
        /* get the current window position */
        SDL_GetWindowPosition (g_win, &window_x, &window_y);
        /* calculate new window position based on relative movement of the mouse */
        window_x += (mouse_x - mouse_x_prev);
        window_y += (mouse_y - mouse_y_prev);
        /* update the window position */
        SDL_SetWindowPosition (g_win, window_x, window_y);

        #ifdef RELOAD_WINDOW_ON_MOVE
        /* update the window display as the window is moved
        this is verry gpu intensive, and on low spec systems is NOT recomended */
        SDL_RenderPresent (g_rend);
        #endif

        /* save the mouse coordinates for next iteration */
        mouse_x_prev = mouse_x;
        mouse_y_prev = mouse_y;
    }

    return;
}


void
graphics_texture_rotate (texture *tex, int direction)
{
    tex->rotation += direction * 90;
    if (abs (tex->rotation) >= 360)
    {
        tex->rotation = 0;
    }
    
}


/* End of File */
