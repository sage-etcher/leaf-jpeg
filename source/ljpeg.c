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
#include <stdlib.h>
#include <string.h>

#if _WIN32 && ! __CYGWIN__
    #include "ljpeg_windows.h"
#else
    #include "ljpeg_posix.h"
#endif
#define POSIX_FILE_SEPERATOR '/'
#define WINDOWS_FILE_SEPERATOR '\\'

/* file static variables */
enum ARG_INDEX
{
    EXEC_NAME,
    INPUT_FILE,
    ARG_COUNT
};


/* file static function prototypes */
static char *get_image_path (int argc, char *argv[]);
static char *get_file_ext (char *file_path);

/* main program-entry-point */
int
main (int argc, char *argv[])
{
    int exit_code = EXIT_SUCCESS;
    char *image_path;
    char *image_ext;

    /* get the image path from console parameters */
    image_path = get_image_path (argc, argv);
    if (NULL == image_path)
    {
        fprintf (stderr, "%s: error: no input file\n", argv[EXEC_NAME]);
        exit_code = EXIT_FAILURE;
        goto main_exit_0;
    }

    /* extract the file extension from image_path */
    image_ext = get_file_ext (image_path);
    if (NULL == image_ext)
    {
        fprintf (stderr, "%s: error: unrecognized file type\n", image_path);
        exit_code = EXIT_FAILURE;
        goto main_exit_0;
    }


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


/* get a pointer to the given file's extension */
static char *
get_file_ext (char *file_path)
{
    size_t path_len = strlen (file_path);
    char *iter      = file_path + path_len - 1;

    /* iterate through the file name, end to start */
    for (; iter >= file_path; iter--)
    {
        /* check the current character */
        switch (*iter)
        {
        case '.':                       /* found the extension */
            return (iter + 1);          /* return the base of the extension */

        case POSIX_FILE_SEPERATOR:      /* disallow walking over POSIX or */
        case WINDOWS_FILE_SEPERATOR:    /* Windows style file seperators */
            goto get_file_ext_exit_0;
        }
    }
   
get_file_ext_exit_0:
    return (char *)NULL;
}


/* End of File */
