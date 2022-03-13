# Simple3DEditor Makefile
# Author: lin-boheng@gitee.com

# 依赖库:
# OpenGL OpenAL stb_image SoundTouch FFmpeg
# 可能加入的依赖库:
# glfw glm glew FreeType "FBX SDK" OpenRL json-cpp ShaderConductor
# 以后创建发行版带上glew32和OpenAL32的动态库

# 大小写平台名称
PLATFORM 	= windows
PLATFORM_U	= WINDOWS

GCC			= g++.exe
DLLTOOL		= dlltool.exe
RM			= del
CFLAGS 		= -I"." -I".\\lib\\freetype" -m64 -O3 -std=c++11
OFLAGS		= -m64 -s
LFLAGS		= -m64 -shared
LIB			= -lopengl32 -lglu32 -lgdi32 -lcomdlg32\
				"lib\openal\OpenAL32.lib" "lib\glew\glew32.lib" "lib\freetype\freetype.lib"
RES  		= windres.exe
MKDIR   	= mkdir
GIT  		= git

BUILD_PATH	= build
TEST_PATH	= test
PROGOBJ		= main\
				lib\soundtouch\SoundTouch lib\soundtouch\TDStretch lib\soundtouch\RateTransposer\
				lib\soundtouch\AAFilter lib\soundtouch\FIRFilter lib\soundtouch\FIFOSampleBuffer\
				lib\soundtouch\PeakFinder lib\soundtouch\BPMDetect\
				utils\String utils\StringBuilder utils\DataBuffer utils\AudioUtils\
				utils\math3d\Math utils\math3d\LinearAlgebra utils\math3d\Mesh\
				utils\math3d\ViewObject utils\math3d\Geometry utils\math3d\Property\
				utils\gl\GLFrameBuffer utils\gl\GLIndexBuffer utils\gl\GLSkyBox\
				utils\gl\GLLights utils\gl\GLProgram utils\gl\GLShader utils\gl\GLEW\
				utils\gl\GLUtils utils\gl\GLVertexArray utils\gl\GLVertexBuffer\
				utils\gl\GLTexture2D utils\gl\GLRenderTexture2D utils\gl\GLComputeProgram\
				editor\AnimationWindow editor\AudioPlayerWindow editor\AudioCaptureWindow\
				editor\NodeMapWindow editor\TreeWindow editor\UVEditWindow editor\PaintWindow\
				editor\MainWindow editor\RenderWindow editor\gui\Container editor\gui\Menu\
				editor\gui\UIManager editor\gui\AnimationCurve editor\gui\ViewManager
PLATOBJ		=  utils\File utils\os\Shell utils\os\Log utils\os\Thread utils\os\System\
				utils\os\Time utils\os\Font utils\os\Appframe utils\os\Resource\
				editor\dialog\ColorBoard editor\dialog\Tips
EXTRAOBJ	= lib\soundtouch\mmx_optimized lib\soundtouch\sse_optimized lib\soundtouch\cpu_detect_x86
RESOBJ		= res string
OUTPUT 		= main.exe
# 动态库
OUTPUTDLIB	= main.dll

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

PROGOBJ 	:= $(addprefix $(BUILD_PATH)\, $(addsuffix .o, $(PROGOBJ)))
PLATOBJ 	:= $(addprefix $(BUILD_PATH)\, $(addsuffix .o, $(PLATOBJ)))
EXTRAOBJ 	:= $(addprefix $(BUILD_PATH)\, $(addsuffix .o, $(EXTRAOBJ)))
RESOBJ		:= $(addprefix $(BUILD_PATH)\, $(addsuffix .o, $(RESOBJ)))

.PHONY:all build clean run rebuild reexec commit merge sign lib

all: build lib

run: build
	$(OUTPUT)

build: $(OUTPUT)

lib: $(OUTPUTDLIB)

rebuild: clean build

reexec: clean run

$(OUTPUTDLIB): $(PROGOBJ) $(PLATOBJ) $(RESOBJ) $(EXTRAOBJ)
	$(GCC) $(PROGOBJ) $(PLATOBJ) $(RESOBJ) $(EXTRAOBJ) -o $@ $(LIB) $(LFLAGS)

$(OUTPUT): $(PROGOBJ) $(PLATOBJ) $(RESOBJ) $(EXTRAOBJ)
	$(GCC) $(PROGOBJ) $(PLATOBJ) $(RESOBJ) $(EXTRAOBJ) -o $@ $(LIB) $(OFLAGS)

# 常规的源代码
$(PROGOBJ): $(BUILD_PATH)\\%.o: %.cpp %.h
	$(GCC) -c $< -o $@ $(CFLAGS) -D$(addprefix PLATFORM_, $(PLATFORM_U))

# 这一类为平台相关代码源文件
$(PLATOBJ): $(BUILD_PATH)\\%.o: platform\$(PLATFORM)\\%.cpp %.h
	$(GCC) -c $< -o $@ $(CFLAGS) -D$(addprefix PLATFORM_, $(PLATFORM_U))

# 这一类源文件没有头文件，直接编译
$(EXTRAOBJ): $(BUILD_PATH)\\%.o: %.cpp
	$(GCC) -c $< -o $@ $(CFLAGS) -D$(addprefix PLATFORM_, $(PLATFORM_U))

# 其实我个人觉得，把nasm的内置指令incbin用好了当资源表用是相当可行的，理论上只要不乱加汇编代码跨平台是没问题的
# 采用nasm的话，res.h里面就不是各种id了，而是一堆extern char[0]，指向资源数据的指针，借助了符号链接的功能
$(RESOBJ): $(BUILD_PATH)\\%.o: %.rc
	$(RES) -i $< -o $@

# 当前分支
BRANCH		= AVdevelop
# 提交信息
COMMITMSG	= "bugfix"
# 合并分支时的目标分支
MERGE 		= AVDevelop

# 一键提交代码
commit:
	$(GIT) checkout $(BRANCH)
	$(GIT) add *
	$(GIT) commit -m $(COMMITMSG)
	$(GIT) push -u origin $(BRANCH)

# 一键合并分支
merge:
	$(GIT) checkout $(MERGE)
	$(GIT) pull origin $(MERGE)
	$(GIT) merge $(BRANCH)
	$(GIT) status
	$(GIT) push origin $(MERGE)
	$(GIT) checkout $(BRANCH)

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
# signtool.exe sign /f "D:/code/.Certificate/lin-boheng.pfx" /p ... main.exe
sign: $(OUTPUT)
	$(SIGNTOOL) timestamp /t $(TIMESTAMP) $(OUTPUT)

clean:
	-$(RM) $(OUTPUT) $(OUTPUTDLIB) $(PROGOBJ) $(PLATOBJ) $(RESOBJ) $(EXTRAOBJ)

# 测试
.PHONY: dllboot freetype

# 后面的测试如果只链接一个类的.o文件，include对应的.h，加上启动测试代码，不就是单元测试了么

# 第一个测试，将整个程序当成库使用并启动
dllboot: $(OUTPUTDLIB)
	$(GCC) $(TEST_PATH)\$@\boot.cpp $(OUTPUTDLIB) -I"." -o $(TEST_PATH)\$@\boot.exe
	$(TEST_PATH)\$@\boot.exe
