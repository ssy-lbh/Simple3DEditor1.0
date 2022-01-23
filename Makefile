# OpenGL Program Makefile

GCC			= g++.exe
RM			= del
CFLAGS 		= 
LIB			= -lopengl32 -lglu32 -lgdi32
RES  		= windres.exe

BUILD_PATH	= build
PROGOBJ		= main.o log.o vecmath.o font.o menu.o gltools.o uimgr.o mesh.o geodef.o colorboard.o nodemap.o
RESOBJ		= res.o
OUTPUT 		= main.exe

PROGOBJ 	:= $(addprefix $(BUILD_PATH)\, $(PROGOBJ))
RESOBJ		:= $(addprefix $(BUILD_PATH)\, $(RESOBJ))

.PHONY:all build clean run rebuild reexec commit merge

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

commit:
	git checkout develop
	git add *
	git commit -m "update"
	git push -u origin develop

merge:
	git checkout master
	git pull origin master
	git merge develop
	git status
	git push origin master

# git log 日志 ('q'退出)
# git reflog 历史操作日志 (包含回退操作)('q'退出)
# git reset --hard HEAD^ 回退到上一版本
# git reset --hard id (id 在 git reflog 里，用于撤销)

clean:
	$(RM) $(OUTPUT) $(PROGOBJ) $(RESOBJ)
