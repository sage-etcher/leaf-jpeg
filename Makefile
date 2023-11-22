#
#  Makefile
#  Simple GNU Makefile to build and install the project
#
#  Copyright 2023 Sage I. Hendricks
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#

PROJECT_NAME := Leaf JPEG
PROJECT_VERSION := 0.3.0.2-dev
# VERSION in the form MAJOR.MINOR.PATCH.BUILD-AUDIENCE
# Example for Developer Build, version 2.14 patch 3
# 2.14.3.841-dev
# Example for Stable Release, version 2.15 patch 0
# 2.15.0.857


# Important Directories
BUILD_DIR  := ./build
SOURCE_DIR := ./source
DESTDIR    := /usr/local

INCLUDE_DIR := $(SOURCE_DIR)/include
LIBRARY_DIR := $(SOURCE_DIR)/lib


# Important Files
#EXAMPLE_EXEC := example.exe
#EXAMPLE_SOURCE_FILENAMES := example/main.c example/example.c
#EXAMPLE_SOURCE_FILES := $(foreach filename,$(EXAMPLE_SOURCE_FILENAMES),$(SOURCE_DIR)/$(filename))
#EXAMPLE_OBJECT_FILES := $(foreach filename,$(EXAMPLE_SOURCE_FILES),$(BUILD_DIR)/$(filename).o)

LJPEG_EXEC := ljpeg.exe
LJPEG_SOURCE_FILENAMES := ljpeg.c graphics.c
LJPEG_SOURCE_FILES := $(foreach filename,$(LJPEG_SOURCE_FILENAMES),$(SOURCE_DIR)/$(filename))
LJPEG_OBJECT_FILES := $(foreach filename,$(LJPEG_SOURCE_FILES),$(BUILD_DIR)/$(filename).o)

DRAFT_EXEC := draft-ljpeg.exe
DRAFT_SOURCE_FILENAMES := draft/ljpeg-draft.c
DRAFT_SOURCE_FILES := $(foreach filename,$(DRAFT_SOURCE_FILENAMES),$(SOURCE_DIR)/$(filename))
DRAFT_OBJECT_FILES := $(foreach filename,$(DRAFT_SOURCE_FILES),$(BUILD_DIR)/$(filename).o)
DRAFT_L_FLAGS := -IC:/msys64/clang64/include/SDL2 -Dmain=SDL_main -lmingw32 -mwindows -lSDL2main -lSDL2 -IC:/msys64/clang64/include/SDL2 -Dmain=SDL_main -IC:/msys64/clang64/include/webp -DLIBDEFLATE_DLL -DHWY_SHARED_DEFINE -IC:/msys64/clang64/include/libpng16 -lSDL2_image -lmingw32 -mwindows -lSDL2main -lSDL2

# Compiler and Linker Options
CC := clang
LD := clang

INCLUDE_FLAGS := -I$(INCLUDE_DIR) -I$(SOURCE_DIR)
LIBRARY_FLAGS := -L$(LIBRARY_DIR)

C_FLAGS := $(INCLUDE_FLAGS) 
#C_FLAGS += -O3 
C_FLAGS += -O0 
#C_FLAGS += -ansi 
C_FLAGS += -std=c99
C_FLAGS += -pedantic -Wpedantic 
C_FLAGS += -Wall 
C_FLAGS += -Wno-unused-label
C_FLAGS += -Wno-unused-function
C_FLAGS += -Werror
C_FLAGS += `pkgconf --cflags SDL2 SDL2_image` 

L_FLAGS := $(LIBRARY_FLAGS)
L_FLAGS := `pkgconf --libs SDL2 SDL2_image`


# Build
.PHONY: build
build: $(BUILD_DIR)/$(LJPEG_EXEC)
#build: $(BUILD_DIR)/$(LJPEG_EXEC) $(BUILD_DIR)/draft/$(DRAFT_EXEC)
.PHONY: draft-build
draft-build: $(BUILD_DIR)/draft/$(DRAFT_EXEC)

# LJPEG main 
$(BUILD_DIR)/$(LJPEG_EXEC): $(LJPEG_OBJECT_FILES)
	mkdir -pv $(dir $@)
	$(LD) -o $@ $(L_FLAGS) $(LJPEG_OBJECT_FILES)

$(BUILD_DIR)/$(SOURCE_DIR)/%.c.o: $(SOURCE_DIR)/%.c
	mkdir -pv $(dir $@)
	$(CC) -c -o $@ $(C_FLAGS) $<

# Draft
$(BUILD_DIR)/draft/$(DRAFT_EXEC): $(DRAFT_OBJECT_FILES)
	mkdir -pv $(dir $@)
	$(LD) -o $@ $(L_FLAGS) $(DRAFT_L_FLAGS) $(DRAFT_OBJECT_FILES)

$(BUILD_DIR)/$(SOURCE_DIR)/draft/%.c.o: $(SOURCE_DIR)/draft/%.c
	mkdir -pv $(dir $@)
	$(CC) -c -o $@ $(C_FLAGS) $<


# Clean
.PHONY: clean
clean:
	rm -rfv $(BUILD_DIR)


# Install
.PHONY: install
install: $(DESTDIR)/bin/$(LJPEG_EXEC)


$(DESTDIR)/bin/$(LJPEG_EXEC): $(BUILD_DIR)/$(LJPEG_EXEC)
	mkdir -pv $(dir $@)
	cp -fv $< $@


# Uninstall
.PHONY: uninstall
uninstall:
	rm -fv $(DESTDIR)/bin/$(LJPEG_EXEC)


