# Simple3DEditor Makefile
# Author: lin-boheng@gitee.com

# 依赖库:
# OpenGL OpenAL stb_image SoundTouch FFmpeg
# 可能加入的依赖库:
# glfw glm glew FreeType "FBX SDK" OpenRL json-cpp ShaderConductor ReactPhysics3D
# 以后创建发行版带上glew32和OpenAL32的动态库

# 说明一下，我写过Vulkan，但是因为不够熟练才没使用
# 今后我会挑战一下

# 大小写平台名称
PLATFORM 	= windows
PLATFORM_U	= WINDOWS

GCC			= g++
DLLTOOL		= dlltool
RM			= bin/linux/rm
DIRNAME 	= bin/linux/dirname

CFLAGS 		= -I"." -I"./lib/freetype" -I"./lib/ftgl"\
				-m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8
OFLAGS		= -m64 -s
LFLAGS		= -m64 -shared
LIB			= -lopengl32 -lglu32 -lgdi32 -lcomdlg32\
				"lib\openal\OpenAL32.lib" "lib\glew\glew32.lib" "lib\freetype\freetype.lib"\
				"lib\glut\glut64.lib" "lib\ftgl\ftgl_D.lib"
RES  		= windres.exe
MKDIR   	= mkdir
GIT  		= git

BUILD_PATH	= build
TEST_PATH	= test
PLUGIN_PATH = plugins

# 正常的含有.h和.cpp的代码
PROGOBJ 	:= $(basename $(wildcard *.cpp\
				lib/soundtouch/*.cpp\
				utils/*.cpp utils/*/*.cpp utils/*/*/*.cpp utils/*/*/*/*.cpp utils/*/*/*/*/*.cpp utils/*/*/*/*/*/*.cpp\
				editor/*.cpp editor/*/*.cpp editor/*/*/*.cpp editor/*/*/*/*.cpp\
				manager/*.cpp))
# 不需要的代码
REMOVEOBJ 	:= lib/soundtouch/soundtouch_wapper
# 只有.cpp的代码
EXTRAOBJ	:= lib/soundtouch/mmx_optimized lib/soundtouch/sse_optimized lib/soundtouch/cpu_detect_x86
# 平台相关代码
PLATOBJ 	:= $(basename $(wildcard $(addprefix platform/$(PLATFORM)/, *.cpp */*.cpp */*/*.cpp */*/*/*.cpp)))

PROGOBJ 	:= $(filter-out $(EXTRAOBJ) $(REMOVEOBJ), $(PROGOBJ))
PLATOBJ 	:= $(PLATOBJ:platform/$(PLATFORM)/%=%)

RESOBJ		:= res string
OUTPUT 		:= main.exe
# 动态库
OUTPUTDLIB	:= main.dll

SIGNTOOL	:= "C:\Program Files (x86)\Windows Kits\10\bin\10.0.19041.0\x86\signtool.exe"
CERT 		:= "D:\code\.Certificate\lin-boheng.pfx"
#TIMESTAMP 	:= "http://timestamp.wosign.com/timestamp"
#TIMESTAMP	:= "http://timestamp.verisign.com/scripts/timstamp.dll"
TIMESTAMP 	:= "http://timestamp.digicert.com"
#TIMESTAMP 	:= "http://timestamp.entrust.net/TSS/RFC3161sha2TS"
#TIMESTAMP 	:= "http://timestamp.globalsign.com"
#TIMESTAMP 	:= "http://rfc3161timestamp.globalsign.com/advanced"
#TIMESTAMP 	:= "http://sha256timestamp.ws.symantec.com/sha256/timestamp"
#TIMESTAMP 	:= "http://timestamp.comodoca.com/rfc3161"
#TIMESTAMP 	:= "http://timestamp.wosign.com/rfc3161"
#TIMESTAMP 	:= "http://tsa.starfieldtech.com"
#TIMESTAMP 	:= "http://tsa.swisssign.net"

PROGOBJ 	:= $(addprefix $(BUILD_PATH)/, $(addsuffix .o, $(PROGOBJ)))
PLATOBJ 	:= $(addprefix $(BUILD_PATH)/, $(addsuffix .o, $(PLATOBJ)))
EXTRAOBJ 	:= $(addprefix $(BUILD_PATH)/, $(addsuffix .o, $(EXTRAOBJ)))
RESOBJ		:= $(addprefix $(BUILD_PATH)/, $(addsuffix .o, $(RESOBJ)))

ALLOBJ 		:= $(PROGOBJ) $(PLATOBJ) $(EXTRAOBJ) $(RESOBJ)

.PHONY:all build clean run rebuild reexec commit merge sign lib mkdir

all: build lib

run: build
	$(OUTPUT)

build: $(OUTPUT)

lib: $(OUTPUTDLIB)

rebuild: clean build

reexec: clean run

$(OUTPUTDLIB): $(ALLOBJ)
	$(GCC) $(ALLOBJ) -o $@ $(LIB) $(LFLAGS)

$(OUTPUT): $(ALLOBJ)
	$(GCC) $(ALLOBJ) -o $@ $(LIB) $(OFLAGS)

# 常规的源代码
$(PROGOBJ): $(BUILD_PATH)/%.o: %.cpp %.h
	$(GCC) -c $< -o $@ $(CFLAGS) -D$(addprefix PLATFORM_, $(PLATFORM_U))

# 这一类为平台相关代码源文件
$(PLATOBJ): $(BUILD_PATH)/%.o: platform/$(PLATFORM)/%.cpp %.h
	$(GCC) -c $< -o $@ $(CFLAGS) -D$(addprefix PLATFORM_, $(PLATFORM_U))

# 这一类源文件没有头文件，直接编译
$(EXTRAOBJ): $(BUILD_PATH)/%.o: %.cpp
	$(GCC) -c $< -o $@ $(CFLAGS) -D$(addprefix PLATFORM_, $(PLATFORM_U))

# 其实我个人觉得，把nasm的内置指令incbin用好了当资源表用是相当可行的，理论上只要不乱加汇编代码跨平台是没问题的
# 采用nasm的话，res.h里面就不是各种id了，而是一堆extern char[0]，指向资源数据的指针，借助了符号链接的功能
$(RESOBJ): $(BUILD_PATH)/%.o: %.rc
	$(RES) -i $< -o $@

OBJDIRS		:= $(sort $(shell $(DIRNAME) $(ALLOBJ)))
# 有一些目录会套多层，其中间没有文件
OBJDIRS 	:= $(sort $(OBJDIRS) $(shell $(DIRNAME) $(OBJDIRS)))

mkdir:
	$(MKDIR) $(OBJDIRS)

# 当前分支
BRANCH		= master-2.0
# 提交信息
COMMITMSG	= "update"
# 合并分支时的目标分支
MERGE 		= master-2.0

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
	$(RM) $(OUTPUT) $(OUTPUTDLIB) $(ALLOBJ)

# glslc <file> --target-spv=spv1.0 [-x glsl/hlsl] -o <out> 编译GLSL/HLSL为SPIR-V
# dxc -T {stage}[[ps/vs/gs/hs/ds/cs/lib/ms]_6_[0-7]] <file> -E <entry> -I <include> [-spirv]
# spirv-as <file> -o <out> --target-env [spv1.[0-5]|vulkan1.[0-2]|opencl[1.2/2.[0-2]][embedded]|opengl4.[0,1,2,3,5]|vulkan1.1spv1.4]
# spirv-dis <file> -o <out> 反汇编SPIR-V
# spirv-cross SPIR-V <file> [--es/--hlsl/--msl/--vulkan-semantics(-V)/--reflect/--cpp] --output <out> 交叉编译SPIR-V为GLSL/ESSL/HLSL/MSL/JSON反射/C++

# 测试
.PHONY: dllboot plugin physics

# 后面的测试如果只链接一个类的.o文件，include对应的.h，加上启动测试代码，不就是单元测试了么

# 第一个测试，将整个程序当成库使用并启动
dllboot: $(OUTPUTDLIB)
	$(GCC) $(TEST_PATH)/$@/boot.cpp $(OUTPUTDLIB) -I"." -o $(TEST_PATH)/$@/boot.exe
	$(TEST_PATH)/$@/boot.exe
	$(RM) $(TEST_PATH)/$@/boot.exe

plugin: $(OUTPUTDLIB)
	$(GCC) $(TEST_PATH)/$@/main.cpp $(OUTPUTDLIB) -I"." -o $(TEST_PATH)/$@/main.exe
	$(GCC) $(TEST_PATH)/$@/plugin.cpp $(OUTPUTDLIB) -I"." -shared -o $(PLUGIN_PATH)/plugin.dll
	$(TEST_PATH)/$@/main.exe
	$(RM) $(TEST_PATH)/$@/main.exe
	$(RM) $(PLUGIN_PATH)/plugin.dll

physics: $(OUTPUTDLIB)
	$(GCC) $(TEST_PATH)/$@/main.cpp $(OUTPUTDLIB) -I"." -o $(TEST_PATH)/$@/main.exe
	$(TEST_PATH)/$@/main.exe
	$(RM) $(TEST_PATH)/$@/main.exe
