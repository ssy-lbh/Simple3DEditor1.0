# OpenGL Program Makefile

# 依赖库:
# OpenGL
# 可能加入的依赖库:
# OpenAL ffmpeg stb_image glfw glm glew

GCC			= g++.exe
RM			= del
CFLAGS 		= -I"ffmpeg\\" -m64
OFLAGS		= -m64 -s
LIB			= -lopengl32 -lglu32 -lgdi32 -lcomdlg32\
				"ffmpeg\avcodec.lib" "ffmpeg\avformat.lib" "ffmpeg\avutil.lib" "ffmpeg\swscale.lib"\
				"glew\glew32.lib" "openal\OpenAL32.lib"
RES  		= windres.exe
MKDIR   	= mkdir

BUILD_PATH	= build
PROGOBJ		= main.o log.o vecmath.o font.o menu.o gltools.o uimgr.o mesh.o\
				geodef.o colorboard.o nodemap.o shell.o container.o attrtable.o\
				audio.o avcodec.o\
				soundtouch\SoundTouch.o soundtouch\TDStretch.o soundtouch\RateTransposer.o\
				soundtouch\AAFilter.o soundtouch\FIRFilter.o soundtouch\FIFOSampleBuffer.o\
				soundtouch\PeakFinder.o soundtouch\BPMDetect.o
EXTRAOBJ	= soundtouch\mmx_optimized.o soundtouch\sse_optimized.o soundtouch\cpu_detect_x86.o
RESOBJ		= res.o
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
EXTRAOBJ 	:= $(addprefix $(BUILD_PATH)\, $(EXTRAOBJ))
RESOBJ		:= $(addprefix $(BUILD_PATH)\, $(RESOBJ))

.PHONY:all build clean run rebuild reexec commit merge sign

all: build

run: build
	$(OUTPUT)

build: $(OUTPUT)

rebuild: clean build

reexec: clean run

$(OUTPUT): $(PROGOBJ) $(RESOBJ) $(EXTRAOBJ)
	$(GCC) $(PROGOBJ) $(RESOBJ) $(EXTRAOBJ) -o $(OUTPUT) $(LIB) $(OFLAGS)

$(PROGOBJ): $(BUILD_PATH)\\%.o: %.cpp %.h
	$(GCC) -c $< -o $@ $(CFLAGS)

$(EXTRAOBJ): $(BUILD_PATH)\\%.o: %.cpp
	$(GCC) -c $< -o $@ $(CFLAGS)

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

# $(SIGNTOOL) sign /f $(CERT) /p $(PASSWORD) $(OUTPUT)
# signtool.exe sign /f "D:\code\.Certificate\lin-boheng.pfx" /p ... main.exe
sign: $(OUTPUT)
	$(SIGNTOOL) timestamp /t $(TIMESTAMP) $(OUTPUT)

clean:
	$(RM) $(OUTPUT) $(PROGOBJ) $(RESOBJ) $(EXTRAOBJ)
	-$(MKDIR) build
