#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

/* function prototypes */
SDL_HitTestResult my_callback (SDL_Window *window, const SDL_Point *area, void *data);


/* file-static function prototypes */
static int  initialize       (void);
static int  make_window      (void);
static int  make_texture     (const char *filname);
static void keydown_event    (SDL_Event e);
static void mousedown_event  (SDL_Event e);
static void mousewheel_event (SDL_Event e);
static void resize           (double scale);
static void rotate           (int direction);
static void move_window      (void);
static double radtodeg       (double rad);
static void project          (void);

/* file-static global variables */
static SDL_Window   *window;
static SDL_Renderer *renderer;
static SDL_RWops    *rwop;
static SDL_Texture  *texture;
static int           texture_width,       texture_height;
static double        texture_scale_width, texture_scale_height;
static double        texture_scale;
static double        texture_rotation;
static SDL_Rect      texture_rect;
static SDL_Point     texture_origin;

static bool main_runtime;


enum ROTATE_DIRECTION
{
    CLOCKWISE = -1,
    COUNTERCLOCKWISE = 1
};


/* mian entry point */
int
main (int argc, char *argv[])
{
    int exit_code = EXIT_SUCCESS;
    SDL_Event e;
   
    if (argc < 2)
    {
        fprintf (stderr, "no input file\n");
        fflush (stderr);
        exit_code = EXIT_FAILURE;
        goto main_exit_0;
    }

    /* SDL */ 
    exit_code = initialize ();
    if (exit_code != EXIT_SUCCESS) 
    {
        goto main_exit_0;
    }

    /* window */
    exit_code = make_window ();
    if (exit_code != EXIT_SUCCESS)
    {
        goto main_exit_1;
    }
 
    /* image */
    exit_code = make_texture (argv[1]);
    if (exit_code != EXIT_SUCCESS)
    {
        goto main_exit_2;
    }

    /* display image in window */
    SDL_SetWindowSize (window, (int)texture_scale_width, (int)texture_scale_height);
    /*(void)SDL_SetWindowHitTest (window, my_callback, 0);*/
    SDL_ShowWindow (window);

    /* runtime loop */
    main_runtime = true;
    while (main_runtime)
    {
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_KEYDOWN:
                keydown_event (e);
                break;
            case SDL_MOUSEBUTTONDOWN:
                mousedown_event (e);
                break;
            case SDL_MOUSEWHEEL:
                mousewheel_event (e);
                break;
            case SDL_QUIT:
                main_runtime = false;
                break;
            }
        }

         
        SDL_RenderClear (renderer);
        SDL_RenderCopyEx (renderer, texture, NULL, &texture_rect, (int)radtodeg (texture_rotation), NULL, SDL_FLIP_NONE);
        SDL_RenderPresent (renderer);
    } 
       
    SDL_RWclose (rwop);
    SDL_DestroyTexture (texture);
main_exit_2:
    SDL_DestroyRenderer (renderer);
    SDL_DestroyWindow (window);
main_exit_1:    
    SDL_Quit ();
main_exit_0:
    return (exit_code);
} 


static int
initialize (void)
{
    /* SDL */ 
    if (SDL_Init (SDL_INIT_VIDEO) < 0) 
    {
        fprintf (stderr, "could not initialize sdl2: %s\n", SDL_GetError ());
        fflush (stderr);
        return EXIT_FAILURE;    /* fail */
    }

    return EXIT_SUCCESS;
}


static int
make_window (void)
{
    window   = NULL;
    renderer = NULL;
    
    /* window */    
    window = SDL_CreateWindow (
        "hello_sdl2",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        0, 0,
        SDL_WINDOW_HIDDEN | SDL_WINDOW_BORDERLESS
    );
    if (window == NULL)
    {
        fprintf (stderr, "could not create window: %s\n", SDL_GetError ());
        fflush (stderr);
        goto make_window_exit_failure_0;
    }

    /* renderer */
    renderer = SDL_CreateRenderer (window, -1, SDL_RENDERER_ACCELERATED);
    if (window == NULL)
    {
        fprintf (stderr, "could not create window renderer: %s\n", SDL_GetError ());
        fflush (stderr);
        goto make_window_exit_failure_1;
    }

    /* successfull exit */
    return EXIT_SUCCESS;

    /* failed exit */
make_window_exit_failure_1:
    SDL_DestroyWindow (window);
make_window_exit_failure_0:
    return EXIT_FAILURE;
}


static int
make_texture (const char *filename)
{
    rwop = NULL;
    texture = NULL;
    texture_scale = 1.0;
    texture_rotation = 0;
    
    rwop = SDL_RWFromFile (filename, "rb");
    if (rwop == NULL)
    {
        fprintf (stderr, "could not load texture: %s\n", SDL_GetError ());
        fflush (stderr);
        goto make_texture_exit_failure_0;
    }
    texture = IMG_LoadTexture_RW (renderer, rwop, 0);
    if (texture == NULL)
    {
        fprintf (stderr, "could not load texture: %s\n", SDL_GetError ());
        fflush (stderr);
        goto make_texture_exit_failure_1;
    }
    SDL_QueryTexture (texture, NULL, NULL, &texture_width, &texture_height);
    texture_scale_width  = texture_width  * texture_scale;
    texture_scale_height = texture_height * texture_scale;
    
    texture_rect.x = 0;
    texture_rect.y = 0;
    texture_rect.w = (int)texture_scale_width;
    texture_rect.h = (int)texture_scale_height;

    texture_origin.x = 0;
    texture_origin.y = 0;

/* make_texture_exit_success_0: */
    return EXIT_SUCCESS;

make_texture_exit_failure_1:
    SDL_RWclose (rwop);
make_texture_exit_failure_0:
    return EXIT_FAILURE; 
}


static void
keydown_event (SDL_Event e)
{
    if (e.key.keysym.sym == SDLK_ESCAPE)
    {   
        main_runtime = false;
    }
    else if ((e.key.keysym.sym == '1') && ((e.key.keysym.mod & (KMOD_CTRL | KMOD_ALT)) != 0))
    {
        /* scale to 50% */
        resize (0.5);
    }
    else if ((e.key.keysym.sym == '2') && ((e.key.keysym.mod & (KMOD_CTRL | KMOD_ALT)) != 0))
    {
        /* scale to 100% */
        resize (1.0);
    }
    else if ((e.key.keysym.sym == '3') && ((e.key.keysym.mod & (KMOD_CTRL | KMOD_ALT)) != 0))
    {
        /* scale to 200% */
        resize (2.0);
    }
    else if ((e.key.keysym.sym == '0') && ((e.key.keysym.mod & KMOD_CTRL) != 0) && ((e.key.keysym.mod & KMOD_ALT) != 0))
    {
        resize (1.0);
    }
    else if ((e.key.keysym.sym == '=') && ((e.key.keysym.mod & KMOD_CTRL) != 0))
    {
        resize (texture_scale * 2);
    }
    else if ((e.key.keysym.sym == '-') && ((e.key.keysym.mod & KMOD_CTRL) != 0))
    {
        resize (texture_scale / 2);
    }
    else if ((e.key.keysym.sym == 'r') && ((e.key.keysym.mod & KMOD_SHIFT) != 0))
    {
        /* rotate left */
        rotate (CLOCKWISE);
    }
    else if (e.key.keysym.sym == 'r')
    {
        /* rotate right */
        rotate (COUNTERCLOCKWISE);
    }
    else if (e.key.keysym.sym == 'a')
    {
        /* restore defaults */
        texture_rotation = 0;
        texture_scale = 1.0;
        project ();
    }

    return;
}

static void
mousedown_event (SDL_Event e)
{
    if (e.button.button == SDL_BUTTON_RIGHT)
    {
        main_runtime = false;
    }
    else if ((e.button.button == SDL_BUTTON_LEFT) && (e.button.clicks == 2))
    {
        /* scale to 100% */
        resize (1.0);
    }
    else if (e.button.button == SDL_BUTTON_LEFT)
    {
        /* drag window arround */
        move_window ();
    }

    return; 
}


static void 
mousewheel_event (SDL_Event e)
{
    if (e.wheel.y >= 1)
    {
        resize (texture_scale * 1.10);
    }
    else if (e.wheel.y <= -1)
    {
        resize (texture_scale / 1.10);
    }

    return; 
}


static void
project (void)
{
    double new_width;
    double new_height;

    /* scale */
    texture_scale_width  = (int)((double)texture_width  * texture_scale);
    texture_scale_height = (int)((double)texture_height * texture_scale);
    texture_rect.w = texture_scale_width;
    texture_rect.h = texture_scale_height;
  
    /* rotation */ 
    texture_rect.x = -(fmod (texture_rotation, M_PI) / (M_PI/2)) * ((texture_rect.w / 2) - (texture_rect.h / 2));
    texture_rect.y = -(fmod (texture_rotation, M_PI) / (M_PI/2)) * ((texture_rect.h / 2) - (texture_rect.w / 2));
     
    new_width = cos (texture_rotation) * texture_scale_width - sin (texture_rotation) * texture_scale_height;
    new_height = sin (texture_rotation) * texture_scale_width - cos (texture_rotation) * texture_scale_height;

    texture_scale_height = fabs(new_height);
    texture_scale_width = fabs(new_width);

    SDL_SetWindowSize (window, (int)texture_scale_width, (int)texture_scale_height); 
}


static void
resize (double scale)
{
    texture_scale = scale;
    project ();
}

static void
rotate (int direction)
{
    if (fabs (texture_rotation) >= (2*M_PI))
    {   
        texture_rotation = 0;
    }

    texture_rotation += direction * (M_PI/2);
    project ();
   
}

static void
move_window (void)
{
    int mouse_btns;
    int prev_mx, prev_my;
    int new_mx, new_my;
    int window_x, window_y;

    (void)SDL_GetGlobalMouseState (&prev_mx, &prev_my);

    do 
    { 
        mouse_btns = SDL_GetGlobalMouseState (&new_mx, &new_my);
        SDL_GetWindowPosition (window, &window_x, &window_y);
        window_x += (new_mx - prev_mx);
        window_y += (new_my - prev_my);
        SDL_SetWindowPosition (window, window_x, window_y);
    
        prev_mx = new_mx;
        prev_my = new_my;
    }  
    while (mouse_btns & SDL_BUTTON (SDL_BUTTON_LEFT));

    return;
}


double
radtodeg (double rad)
{
    return ((rad/M_PI) * 180);
}



/* end of file */
