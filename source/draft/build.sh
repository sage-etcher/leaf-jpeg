#!/bin/bash
clang main.c -o ljpeg-draft -IC:/msys64/clang64/include/SDL2 -Dmain=SDL_main -lmingw32 -mwindows -lSDL2main -lSDL2 -IC:/msys64/clang64/include/SDL2 -Dmain=SDL_main -IC:/msys64/clang64/include/webp -DLIBDEFLATE_DLL -DHWY_SHARED_DEFINE -IC:/msys64/clang64/include/libpng16 -lSDL2_image -lmingw32 -mwindows -lSDL2main -lSDL2
