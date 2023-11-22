/*
   source/ljpeg.c
   LJPEG main entry point source file
 
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


/* include headers */
#include <stdio.h>

#include "ljpeg_graphics.h"
#include "ljpeg_config.h"


/* file static variables */
enum ARG_INDEX
{
    EXEC_NAME,
    INPUT_FILE,
    ARG_COUNT
};
static bool g_runtime_bool;


/* file static function prototypes */
static char *get_image_path (int argc, char *argv[]);
static void key_event (SDL_Event *evt);
static void mouse_btn_event (SDL_Event *evt);
static void mouse_wheel_event (SDL_Event *evt);


/* main program-entry-point */
int
main (int argc, char *argv[])
{
    int exit_code = EXIT_SUCCESS;
    char *image_path;
    SDL_Event evt;

    /* get the image path from console parameters */
    image_path = get_image_path (argc, argv);
    if (NULL == image_path)
    {
        fprintf (stderr, "%s: error: no input file\n", argv[EXEC_NAME]);
        exit_code = EXIT_FAILURE;
        goto main_exit_0;
    }

    /* initialize required SDL elements */
    exit_code = graphics_init_sdl ();
    if (exit_code != EXIT_SUCCESS)
    {
        goto main_exit_1;
    }
    exit_code = graphics_init_window ();
    if (exit_code != EXIT_SUCCESS)
    {
        goto main_exit_2;
    }
    exit_code = graphics_load_texture (image_path);
    if (exit_code != EXIT_SUCCESS)
    {
        goto main_exit_3;
    } 

    /* set the window size and display the window */
    SDL_SetWindowSize (g_win, (int)g_img.source.w, (int)g_img.source.h);
    SDL_ShowWindow (g_win);    

    /* set the background color for transparent images */
    /* 255 255 255 == White */
    /*   0   0   0 == Black */
    SDL_SetRenderDrawColor (g_rend, 255, 255, 255, SDL_ALPHA_OPAQUE);
    
    /* initial draw */
    SDL_RenderClear (g_rend);
    graphics_render (g_rend, &g_img);
    SDL_RenderPresent (g_rend);

    /* start the main runtime loop */
    g_runtime_bool = true;
    while (g_runtime_bool)
    {
        /* await for an event */
        if (SDL_WaitEvent (&evt) == 1)
        {
            switch (evt.type)
            {
            case SDL_KEYDOWN:
                key_event (&evt);
                break;
            case SDL_MOUSEBUTTONDOWN:
                mouse_btn_event (&evt);
                break;
            case SDL_MOUSEWHEEL:
                mouse_wheel_event (&evt);
                break;
            case SDL_QUIT:
                g_runtime_bool = false;
                break;
            }
        }

        /* display the image */ 
        SDL_RenderClear (g_rend);
        graphics_render (g_rend, &g_img);
        SDL_RenderPresent (g_rend);
        
    } 


    /* exit routines */
main_exit_4: 
    SDL_RWclose (g_img.rwop);
    SDL_DestroyTexture (g_img.texture);
main_exit_3:
    SDL_DestroyRenderer (g_rend);
    SDL_DestroyWindow (g_win);
main_exit_2:
    SDL_Quit ();
main_exit_1:
main_exit_0:
    return (exit_code);
}


/* function definitions */
/* get the inputted file from the command line arguements */
static char *
get_image_path (int argc, char *argv[])
{
    /* make sure the user gave an input file */
    if (argc < INPUT_FILE)
    {
        return (char *)NULL;
    }

    /* return first param */
    return argv[INPUT_FILE];
}


static void
key_event (SDL_Event *evt)
{
    SDL_Event e = *evt;
    
    /* ESC */
    if (e.key.keysym.sym == SDLK_ESCAPE)
    {
        /* quit */
        g_runtime_bool = false;
    }
    /* CTRL+1 or ALT+1 */
    else if ((e.key.keysym.sym == '1') && ((e.key.keysym.mod & (KMOD_CTRL | KMOD_ALT)) != 0))
    {
        /* scale to half size */
        g_img.scale = 0.5;
    }
    /* CTRL+2 or ALT+2 */
    else if ((e.key.keysym.sym == '2') && ((e.key.keysym.mod & (KMOD_CTRL | KMOD_ALT)) != 0))
    {
        /* scale to original size */
        g_img.scale = 1.0;
    }
    /* CTRL+3 or ALT+3 */
    else if ((e.key.keysym.sym == '3') && ((e.key.keysym.mod & (KMOD_CTRL | KMOD_ALT)) != 0))
    {
        /* scale to 2x size */
        g_img.scale = 2.0;
    }
    /* CTRL+ALT+0 */
    else if ((e.key.keysym.sym == '0') && ((e.key.keysym.mod & KMOD_CTRL) != 0) && ((e.key.keysym.mod & KMOD_ALT) != 0))
    {
        /* scale to original size */
        g_img.scale = 1.0;
    }
    /* CTRL+'=' */
    else if ((e.key.keysym.sym == '=') && ((e.key.keysym.mod & KMOD_CTRL) != 0))
    {
        /* scale the image up */
        g_img.scale *= 2;
    }
    /* CTRL+'-' */
    else if ((e.key.keysym.sym == '-') && ((e.key.keysym.mod & KMOD_CTRL) != 0))
    {
        /* scale the image down */
        g_img.scale /= 2;
    }
    /* SHIFT+'r' */
    else if ((e.key.keysym.sym == 'r') && ((e.key.keysym.mod & KMOD_SHIFT) != 0))
    {
        /* rotate image counter-clockwise by 90 degrees */
        graphics_texture_rotate (&g_img, COUNTERCLOCKWISE);
    }
    /* 'r' */
    else if (e.key.keysym.sym == 'r')
    {
        /* rotate image clockwise by 90 degrees */
        graphics_texture_rotate (&g_img, CLOCKWISE);
    }
    else if (e.key.keysym.sym == SDLK_RIGHT)
    {
        /* rotate image clockwise by 90 degrees */
        graphics_texture_rotate (&g_img, CLOCKWISE);
    }
    /* Left Arrow */
    else if (e.key.keysym.sym == SDLK_LEFT)
    {
        /* rotate image counter-clockwise by 90 degrees */
        graphics_texture_rotate (&g_img, COUNTERCLOCKWISE);
    }
    /* 'a' */
    else if (e.key.keysym.sym == 'a')
    {
        /* reset image to original scale and rotation */
        g_img.scale = 1.0;
        g_img.rotation = 0.0;
    }
}


static void
mouse_btn_event (SDL_Event *evt)
{ 
    SDL_Event e = *evt;

    /* Single Right Click */
    if (e.button.button == SDL_BUTTON_RIGHT)
    {   
        /* quit */
        g_runtime_bool = false;
    }
    /* Double Left Click */
    else if ((e.button.button == SDL_BUTTON_LEFT) && (e.button.clicks == 2))
    {
        /* reset scale to default */
        g_img.scale = 1.0;
    }
    /* Single Left Click */
    else if (e.button.button == SDL_BUTTON_LEFT)
    {
        /* move window */
        graphics_manual_move_window ();
    }
}


static void
mouse_wheel_event (SDL_Event *evt)
{ 
    SDL_Event e = *evt;
    
    /* Scroll Wheel Away */
    if (e.wheel.y >= 1)
    {
        /* increase scale by 10% */
        g_img.scale *= 1.10;
    }
    /* Scroll Wheel Towards */
    else if (e.wheel.y <= -1)
    {
        /* decrease scale by 10% */
        g_img.scale /= 1.10;
    }
}



/* End of File */
