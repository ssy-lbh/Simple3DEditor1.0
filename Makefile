# OpenGL Program Makefile

GCC			= g++.exe
RM			= del
CFLAGS 		= -I"C:\VulkanSDK\stb-master"
LIB			= -lopengl32 -lglu32 -lgdi32
RES  		= windres.exe

BUILD_PATH	= build
PROGOBJ		= main.o log.o vecmath.o font.o menu.o gltools.o uimgr.o mesh.o geodef.o colorboard.o
RESOBJ		= res.o
OUTPUT 		= main.exe

PROGOBJ 	:= $(addprefix $(BUILD_PATH)\, $(PROGOBJ))
RESOBJ		:= $(addprefix $(BUILD_PATH)\, $(RESOBJ))

.PHONY:all build clean run rebuild reexec

all: build

run: build
	$(OUTPUT)

build: $(OUTPUT)

rebuild: clean build

reexec: clean run

$(OUTPUT): $(PROGOBJ) $(RESOBJ)
	$(GCC) $(PROGOBJ) $(RESOBJ) -o $(OUTPUT) $(LIB)

$(PROGOBJ): $(BUILD_PATH)\\%.o: %.cpp %.h
	$(GCC) -c $< -o $@ $(LIB) $(CFLAGS)

$(RESOBJ): $(BUILD_PATH)\\%.o: %.rc
	$(RES) -i $< -o $@

clean:
	$(RM) $(OUTPUT) $(PROGOBJ) $(RESOBJ)
