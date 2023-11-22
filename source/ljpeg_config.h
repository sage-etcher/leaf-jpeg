/*
   source/ljpeg_config.h
   LJPEG configuration file. 
 
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
#ifndef __LJPEG_CONFIG_HEADER__
#define __LJPEG_CONFIG_HEADER__

/*
Reload the window while moving the window.
Makes things look nicer if window was partialy offscreen. =D
But is EXTREMELY GPU intensive and is NOT recomended for low spec systems
*/
/* #define RELOAD_WINDOW_ON_MOVE */


/*
Background color in RGB (0-255 or 0x00-0xFF)
Default: 0xFF 0xFF 0xFF (WHITE)
*/
#define BACKGROUND_RED   0xFF
#define BACKGROUND_GREEN 0xFF
#define BACKGROUND_BLUE  0xFF


/*
Initial Image scale
Default: 100%
*/
#define INITIAL_SCALE 1.0


/*
Mouse scroll wheel multiplier/divisor
Default: 10%
*/
#define SCROLL_MULTDIV 1.10


/* 
scale preset #1 (ctrl 1)
Default: 50%
*/
#define SCALE_PRESET_1 0.5


/* 
scale preset #2 (ctrl 2)
Default: 100%
*/
#define SCALE_PRESET_2 1.0


/* 
scale preset #3 (ctrl 3)
Default: 200%
*/
#define SCALE_PRESET_3 2.0


#endif /* end run once */


/* End of File */
