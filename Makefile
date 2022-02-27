# OpenGL Program Makefile

# 依赖库:
# OpenGL
# 可能加入的依赖库:
# OpenAL ffmpeg stb_image glfw glm glew

# 大小写平台名称
# 关于跨平台方面，以后将资源部分进行抽象后将完成
PLATFORM 	= windows
PLATFORM_U	= WINDOWS

GCC			= g++.exe
RM			= del
CFLAGS 		= -I"openal\\" -I"opengl\\" -I"." -m64
OFLAGS		= -m64 -g
LIB			= -lopengl32 -lglu32 -lgdi32 -lcomdlg32 "openal\OpenAL32.lib"
RES  		= windres.exe
MKDIR   	= mkdir

BUILD_PATH	= build
PROGOBJ		= main.o vecmath.o menu.o gltools.o uimgr.o mesh.o utils\StringBuilder.o utils\DataBuffer.o\
				geodef.o nodemap.o container.o attrtable.o\
				audio.o paint.o viewobject.o tree.o anim.o\
				soundtouch\SoundTouch.o soundtouch\TDStretch.o soundtouch\RateTransposer.o\
				soundtouch\AAFilter.o soundtouch\FIRFilter.o soundtouch\FIFOSampleBuffer.o\
				soundtouch\PeakFinder.o soundtouch\BPMDetect.o
PLATOBJ		= utils\String.o utils\File.o colorboard.o thread.o log.o\
			  viewmgr.o timetools.o font.o glfunc.o appframe.o shell.o
EXTRAOBJ	= soundtouch\mmx_optimized.o soundtouch\sse_optimized.o soundtouch\cpu_detect_x86.o
RESOBJ		= res.o string.o
OUTPUT 		= main.exe

SIGNTOOL	= "C:\Program Files (x86)\Windows Kits\10\bin\10.0.19041.0\x86\signtool.exe"
CERT 		= "D:\code\.Certificate\lin-boheng.pfx"
#TIMESTAMP 	= "http://timestamp.wosign.com/timestamp"
#TIMESTAMP	= "http://timestamp.verisign.com/scripts/timstamp.dll"
TIMESTAMP 	= "http://timestamp.digicert.com"
#TIMESTAMP 	= "http://timestamp.entrust.net/TSS/RFC3161sha2TS"
#TIMESTAMP 	= "http://timestamp.globalsign.com"
#TIMESTAMP 	= "http://rfc3161timestamp.globalsign.com/advanced"
#TIMESTAMP 	= "http://sha256timestamp.ws.symantec.com/sha256/timestamp"
#TIMESTAMP 	= "http://timestamp.comodoca.com/rfc3161"
#TIMESTAMP 	= "http://timestamp.wosign.com/rfc3161"
#TIMESTAMP 	= "http://tsa.starfieldtech.com"
#TIMESTAMP 	= "http://tsa.swisssign.net"

PROGOBJ 	:= $(addprefix $(BUILD_PATH)\, $(PROGOBJ))
PLATOBJ 	:= $(addprefix $(BUILD_PATH)\, $(PLATOBJ))
EXTRAOBJ 	:= $(addprefix $(BUILD_PATH)\, $(EXTRAOBJ))
RESOBJ		:= $(addprefix $(BUILD_PATH)\, $(RESOBJ))

.PHONY:all build clean run rebuild reexec commit merge sign

all: build

run: build
	$(OUTPUT)

build: $(OUTPUT)

rebuild: clean build

reexec: clean run

$(OUTPUT): $(PROGOBJ) $(PLATOBJ) $(RESOBJ) $(EXTRAOBJ)
	$(GCC) $(PROGOBJ) $(PLATOBJ) $(RESOBJ) $(EXTRAOBJ) -o $(OUTPUT) $(LIB) $(OFLAGS)

$(PROGOBJ): $(BUILD_PATH)\\%.o: %.cpp %.h
	$(GCC) -c $< -o $@ $(CFLAGS) -D$(addprefix PLATFORM_, $(PLATFORM_U))

$(PLATOBJ): $(BUILD_PATH)\\%.o: platform\$(PLATFORM)\\%.cpp %.h
	$(GCC) -c $< -o $@ $(CFLAGS) -D$(addprefix PLATFORM_, $(PLATFORM_U))

$(EXTRAOBJ): $(BUILD_PATH)\\%.o: %.cpp
	$(GCC) -c $< -o $@ $(CFLAGS) -D$(addprefix PLATFORM_, $(PLATFORM_U))

$(RESOBJ): $(BUILD_PATH)\\%.o: %.rc
	$(RES) -i $< -o $@

BRANCH		= AVdevelop
COMMITMSG	= "update"
MERGE 		= AVDevelop

commit:
	-$(RM) temp.wav
	git checkout $(BRANCH)
	git add *
	git commit -m $(COMMITMSG)
	git push -u origin $(BRANCH)

merge:
	git checkout $(MERGE)
	git pull origin $(MERGE)
	git merge $(BRANCH)
	git status
	git push origin $(MERGE)
	git checkout $(BRANCH)

# git log 日志 ('q'退出)
# git reflog 历史操作日志 (包含回退操作)('q'退出)
# git reset --hard HEAD^ 回退到上一版本
# git reset --hard id (id 在 git reflog 里，用于撤销)

# 删除某个文件的所有历史
# git filter-branch --force --index-filter 'git rm --cached --ignore-unmatch ${file}' --prune-empty --tag-name-filter cat -- --all

# 删除仓库缓冲
# rm -rf .git/refs/original/
# git reflog expire --expire=now --all
# git gc --prune=now
# git gc --aggressive --prune=now

# $(SIGNTOOL) sign /f $(CERT) /p $(PASSWORD) $(OUTPUT)
# signtool.exe sign /f "D:\code\.Certificate\lin-boheng.pfx" /p ... main.exe
sign: $(OUTPUT)
	$(SIGNTOOL) timestamp /t $(TIMESTAMP) $(OUTPUT)

clean:
	$(RM) $(OUTPUT) $(PROGOBJ) $(PLATOBJ) $(RESOBJ) $(EXTRAOBJ)
	-@$(MKDIR) build
	-@$(MKDIR) build\soundtouch
	-@$(MKDIR) build\utils
