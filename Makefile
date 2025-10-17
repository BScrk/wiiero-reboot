MODULE 	= $(shell basename `pwd`)
SYSTEM := $(shell uname -s)

# Date command (BSD vs GNU)
ifeq ($(SYSTEM),FreeBSD)
DATE	= gdate
else
DATE	= date
endif

ARCHIVE = backup/$(MODULE)-$(shell $(DATE) '+%Y-%m-%d-%Hh%M').tar.gz

# Common tools
CC	= gcc
MKDIR	= mkdir -p
RMDIR	= rmdir
MV	= mv
CP	= cp
RM	= rm -f
ZIP	= zip

# Common flags
DEFINES	= -D_GNU_SOURCE=1
MYCFLAGS = -O3 -Wall -Werror -fomit-frame-pointer
MYPROFILECFLAGS = -Wall -Werror -pg

# Source files
SRCS = $(notdir $(basename $(wildcard src/*.c)))
OBJS = $(SRCS:%=bin/%.o)

TARGET = wiiero
WIIERO = bin/wiiero
EXES = $(WIIERO)
VPATH = src:bin

#-------------------- Auto-detect system and configure
ifeq ($(SYSTEM),Darwin)
    # macOS (Apple Silicon or Intel) - SDL2
    PLATFORM = macOS
    HOMEBREW_PREFIX := $(shell brew --prefix 2>/dev/null || echo /opt/homebrew)
    LOCALBASE = $(HOMEBREW_PREFIX)
    SDL2_CFLAGS := $(shell $(LOCALBASE)/bin/sdl2-config --cflags 2>/dev/null || echo -I$(LOCALBASE)/include/SDL2)
    SDL2_LIBS := $(shell $(LOCALBASE)/bin/sdl2-config --libs 2>/dev/null || echo -L$(LOCALBASE)/lib -lSDL2)
#    PLATFORM_DEF = -DDESKTOP_MODE --DIOKIT_INPUT_MODE -D_THREAD_SAFE
    PLATFORM_DEF = -DDESKTOP_MODE -DSDL_INPUT_MODE -D_THREAD_SAFE
    PLATFORM_LIBS = -lm $(SDL2_LIBS) -lSDL2_mixer -lSDL2_image -Wl,-framework,Cocoa -framework IOKit -framework CoreFoundation
    PLATFORM_CFLAGS = $(SDL2_CFLAGS)
else ifeq ($(SYSTEM),Linux)
    # Linux - SDL2
    PLATFORM = Linux
    LOCALBASE = /usr
    SDL2_CFLAGS := $(shell sdl2-config --cflags 2>/dev/null || echo -I/usr/include/SDL2)
    SDL2_LIBS := $(shell sdl2-config --libs 2>/dev/null || echo -lSDL2)
    PLATFORM_DEF = -DDESKTOP_MODE -DSDL_INPUT_MODE -D_THREAD_SAFE -DLINUX_BUILD
    PLATFORM_LIBS = -lm $(SDL2_LIBS) -lSDL2_mixer -lSDL2_image
    PLATFORM_CFLAGS = $(SDL2_CFLAGS)
else ifeq ($(SYSTEM),FreeBSD)
    # FreeBSD - SDL2
    PLATFORM = FreeBSD
    LOCALBASE = /usr/local
    SDL2_CFLAGS := $(shell sdl2-config --cflags 2>/dev/null || echo -I/usr/local/include/SDL2)
    SDL2_LIBS := $(shell sdl2-config --libs 2>/dev/null || echo -lSDL2)
    PLATFORM_DEF = -DDESKTOP_MODE -DSDL_INPUT_MODE -D_THREAD_SAFE -DLINUX_BUILD
    PLATFORM_LIBS = -lm $(SDL2_LIBS) -lSDL2_mixer -lSDL2_image
    PLATFORM_CFLAGS = $(SDL2_CFLAGS)
else
    # Generic Unix fallback - SDL2
    PLATFORM = Unix
    LOCALBASE = /usr/local
    SDL2_CFLAGS := $(shell sdl2-config --cflags 2>/dev/null || echo -I/usr/local/include/SDL2)
    SDL2_LIBS := $(shell sdl2-config --libs 2>/dev/null || echo -lSDL2)
    PLATFORM_DEF = -DDESKTOP_MODE -DSDL_INPUT_MODE -D_THREAD_SAFE -DLINUX_BUILD
    PLATFORM_LIBS = -lm $(SDL2_LIBS) -lSDL2_mixer -lSDL2_image
    PLATFORM_CFLAGS = $(SDL2_CFLAGS)
endif

# Construct final flags
CFLAGS = $(MYCFLAGS) $(PLATFORM_CFLAGS) -I$(LOCALBASE)/include $(PLATFORM_DEF)
LDFLAGS = -L$(LOCALBASE)/lib $(PLATFORM_LIBS)

compil: createbin $(EXES)

dev: CFLAGS += -DDEBUG_ON -g
dev: profile 

profile: CFLAGS  = $(MYPROFILECFLAGS) -I$(LOCALBASE)/include $(PLATFORM_DEF) -g -pg
profile: LDFLAGS = -L$(LOCALBASE)/lib $(PLATFORM_LIBS) -pg
profile: compil


all: default 

$(WIIERO): $(OBJS)
		$(CC) $(CFLAGS) $(DEFINES) -o $@ $^ $(LDFLAGS)

bin/%.o: 	src/%.c
		$(CC) $(CFLAGS) $(COPTS) -o $@ -c $<

createbin:
		mkdir -p ./bin

depend:		clean
		cd src && makedepend -f- -Y -p bin/ *.c *.h \
		> ../makefile.d 2> /dev/null

backup:         clean
		rm ./src/*~
		cd .. && tar cvzf $(ARCHIVE) $(MODULE)

clean:
		$(RM) $(OBJS)
		$(RM) $(EXES)
		$(RM) -r ./bin

# Wii cross-compilation target
wii:
		$(MAKE) -f Makefile.wii

.PHONY:		all default dev compil profile depend backup clean createbin wii

include makefile.d
