# Leaf JPEG

___version:___ 0.4.1.6-dev

A cross-platform [VJPEG](http://stereopsis.com/vjpeg/) clone, with added scroll-wheel scaling and hardware acceleration.

### Shortcuts

`Left click (drag)`: move  
`Right click`: quit  
`Double Right click`: view image 1:1 scale  
`Scroll Wheel`: scale up/down  

`Escape`: quit  
`a`: reset scale & rotation  
`Right Arrow` or `r`:rotate clockwise  
`Left Arrow` or `Shift r`: rotate counter-clockwise  

`Ctrl 1` or `Alt 1`: 50% scale  
`Ctrl 2` or `Alt 2`: 100% scale   
`Ctrl 3` or `Alt 3`: 200% scale  

`Ctrl Alt 0`: 100% scale  
`Ctrl +` or `Ctrl =`: next bigger size  
`Ctrl -` or `Ctrl _`: next smaller size  



## Building the Program using GNU Make

>
> ### Build Dependencies
> 
> - bash
> - gnu-make
> - libsdl2-devel
> - libsdl2\_image-devel
> - pkgconf
>
> ### Runtime Dependencies
>
> - libsdl2
> - libsdl2\_image
> 

```
$ make build
# make install
```

## Project Files

| File | Description |
|:-----|-----------|
| VERSION | Contains program's name, version, and licensing information |
| Makefile | GNU Makefile used to build and install the program |
| source/draft/\* | Draft build of the program (quick test) |
| source/ljpeg.c | Program entry point/main source file |
| source/ljpeg\_config.h | Compile time configuration file |
| source/ljpeg\_graphics.\* | Graphical operation wrapper |


## License

Copyright 2023 Sage I. Hendricks   

Licensed under the Apache License, Version 2.0 (the "License");  
you may not use this file except in compliance with the License.  
You may obtain a copy of the License at  

[//]: # ( add 4 unit "tab" )
&nbsp;&nbsp;&nbsp;&nbsp;<http://www.apache.org/licenses/LICENSE-2.0>  

Unless required by applicable law or agreed to in writing, software  
distributed under the License is distributed on an "AS IS" BASIS,  
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  
See the License for the specific language governing permissions and  
limitations under the License.  

