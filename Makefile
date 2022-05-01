# Simple3DEditor Makefile
# Author: lin-boheng@gitee.com

# 依赖库:
# OpenGL OpenAL stb_image SoundTouch FFmpeg
# 可能加入的依赖库:
# glfw glm glew FreeType "FBX SDK" OpenRL json-cpp ShaderConductor ReactPhysics3D
# 以后创建发行版带上glew32和OpenAL32的动态库

# 说明一下，我写过Vulkan，但是因为不够熟练才没使用
# 今后我会挑战一下

# 以后SoundTouch和ReactPhysics3D物理引擎我打算单独编译成一个动态库

# 这仓库里面的类我会在以后复用

RM			:= bin/linux/rm
TR 			:= bin/linux/tr
DIRNAME 	:= bin/linux/dirname
INCLIST 	:= bin/tools/inclist
MKDIR   	:= bin/linux/mkdir
ECHO 		:= echo

ifeq ($(OS), Windows_NT)
    ifeq ($(PROCESSOR_ARCHITEW6432), AMD64)
        ARCH := amd64
    else
        ifeq ($(PROCESSOR_ARCHITECTURE), AMD64)
            ARCH := amd64
        endif
        ifeq ($(PROCESSOR_ARCHITECTURE), x86)
            ARCH := ia32
        endif
    endif
else
	UNAME_P := $(shell uname -p)
    ifeq ($(UNAME_P), x86_64)
        ARCH := amd64
    endif
    ifneq ($(filter %86, $(UNAME_P)),)
        ARCH := ia32
    endif
    ifneq ($(filter arm%, $(UNAME_P)),)
        ARCH := arm
    endif
endif

ifeq ($(OS), Windows_NT)
    PLATFORM := windows
else
	ifneq ($(findstring /cygdrive/, $(PATH)),)
		PLATFORM := cygwin
	else
		UNAME := $(shell uname || echo unknown)
		ifeq ($(UNAME), Linux)
			PLATFORM := linux
		endif
		ifeq ($(UNAME), Solaris)
			PLATFORM := solaris
		endif
	endif
endif

BUILD_PATH	:= build
TEST_PATH	:= test
PLUGIN_PATH := plugins
SOURCE_PATH := src
LIB_PATH 	:= $(SOURCE_PATH)/lib

toupper		= $(shell $(ECHO) $1 | $(TR) a-z A-Z)
tolower		= $(shell $(ECHO) $1 | $(TR) A-Z a-z)

# 递归的wildcard
rwildcard	= $(foreach D, $(wildcard $(1:=/*)), $(call rwildcard, $D, $2) $(filter $(subst *, %, $2), $D))
src2obj		= $(1:%.cpp=$(BUILD_PATH)/%.o)

GCC			:= g++
DLLTOOL		:= dlltool
CFLAGS 		:= -I"$(SOURCE_PATH)" -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8\
				-DPLATFORM_$(call toupper, $(PLATFORM)) -DARCH_$(call toupper, $(ARCH))
OFLAGS		:= -m64 -s
LFLAGS		:= -m64 -shared
LIB_FILES	:= openal/OpenAL32 glew/glew32 freetype/freetype\
				glut/glut64 ftgl/ftgl_D
LIB			:= -lopengl32 -lglu32 -lgdi32 -lcomdlg32 -lws2_32\
				$(foreach FILE, $(LIB_FILES), $(LIB_PATH)/$(FILE).lib)
RES  		:= windres
GIT  		:= git

# 正常的含有.h和.cpp的代码
PROGOBJ 	:= lib/soundtouch util editor manager base io
PROGOBJ 	:= $(addprefix $(SOURCE_PATH)/, $(PROGOBJ))
PROGOBJ 	:= $(basename $(call rwildcard, $(PROGOBJ), *.cpp))
PROGOBJ 	:= $(PROGOBJ:$(SOURCE_PATH)/%=%)
PROGOBJ 	+= main
# 不需要的代码
REMOVEOBJ 	:= lib/soundtouch/soundtouch_wapper
# 只有.cpp的代码
EXTRAOBJ	:= lib/soundtouch/mmx_optimized lib/soundtouch/sse_optimized lib/soundtouch/cpu_detect_x86
# 平台相关代码
PLATOBJ 	:= platform/$(PLATFORM)
PLATOBJ 	:= $(addprefix $(SOURCE_PATH)/, $(PLATOBJ))
PLATOBJ 	:= $(basename $(call rwildcard, $(PLATOBJ), *.cpp))
PLATOBJ 	:= $(PLATOBJ:$(SOURCE_PATH)/%=%)

PROGOBJ 	:= $(filter-out $(EXTRAOBJ) $(REMOVEOBJ), $(PROGOBJ))

RESOBJ 		:= .
RESOBJ 		:= $(addprefix $(SOURCE_PATH)/, $(RESOBJ))
RESOBJ 		:= $(basename $(call rwildcard, $(RESOBJ), *.rc))
RESOBJ 		:= $(RESOBJ:$(SOURCE_PATH)/%=%)

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

CPPOBJ 		:= $(PROGOBJ) $(PLATOBJ) $(EXTRAOBJ)
ALLOBJ		:= $(CPPOBJ) $(RESOBJ)

.PHONY: all mkdir build lib cleanall cleandep clean run rebuild reexec sign commit merge

# build之前应该mkdir
all: mkdir build lib

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

# 注:不使用依赖项文件时使用如下编译流程
# 常规的源代码
$(PROGOBJ): $(BUILD_PATH)/%.o: $(SOURCE_PATH)/%.cpp $(SOURCE_PATH)/%.h
	$(GCC) -c $< -o $@ $(CFLAGS)

# 这一类为平台相关代码源文件
$(PLATOBJ): $(BUILD_PATH)/platform/$(PLATFORM)/%.o: $(SOURCE_PATH)/platform/$(PLATFORM)/%.cpp $(SOURCE_PATH)/%.h
	$(GCC) -c $< -o $@ $(CFLAGS)

# 这一类源文件没有头文件，直接编译
$(EXTRAOBJ): $(BUILD_PATH)/%.o: $(SOURCE_PATH)/%.cpp
	$(GCC) -c $< -o $@ $(CFLAGS)

# 其实我个人觉得，把nasm的内置指令incbin用好了当资源表用是相当可行的，理论上只要不乱加汇编代码跨平台是没问题的
# 采用nasm的话，res.h里面就不是各种id了，而是一堆extern char[0]，指向资源数据的指针，借助了符号链接的功能
$(RESOBJ): $(BUILD_PATH)/%.o: $(SOURCE_PATH)/%.rc
	$(RES) -i $< -o $@

# 依赖项文件
DEPFILES	:= $(CPPOBJ:%.o=%.d)

# 不使用时用'#'注释掉
# 一般而言有了大批量定义改动后才需要使用
#! 如果使用，请不要轻易改动头文件！！！
-include $(DEPFILES)

$(DEPFILES): $(BUILD_PATH)/%.d: $(SOURCE_PATH)/%.cpp
	$(GCC) -MM $(CFLAGS) $< -MT $(call src2obj, $<) > $@
	$(ECHO) "	$(GCC) -c $< -o $(call src2obj, $<) $(CFLAGS)" >> $@

OBJDIRS		:= $(sort $(shell $(DIRNAME) $(ALLOBJ)))
# 有一些目录会套多层，其中间没有文件
OBJDIRS 	:= $(sort $(OBJDIRS) $(shell $(DIRNAME) $(OBJDIRS)))
OBJDIRS 	:= $(sort $(OBJDIRS) $(shell $(DIRNAME) $(OBJDIRS)))

mkdir:
	-$(MKDIR) $(OBJDIRS)

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

cleanall: clean cleandep

clean:
	-$(RM) $(OUTPUT) $(OUTPUTDLIB) $(ALLOBJ)

cleandep:
	-$(RM) $(DEPFILES)

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
	$(GCC) -c $(TEST_PATH)/$@/main.cpp -I"." -o $(TEST_PATH)/$@/main.o
	$(GCC) -c $(TEST_PATH)/$@/Test.cpp -I"." -o $(TEST_PATH)/$@/Test.o
	$(GCC) -c $(TEST_PATH)/$@/TestSuite.cpp -I"." -o $(TEST_PATH)/$@/TestSuite.o
	$(GCC) $(addprefix $(TEST_PATH)/$@/, main.o Test.o TestSuite.o) $(OUTPUTDLIB) -o $(TEST_PATH)/$@/main.exe
	$(TEST_PATH)/$@/main.exe
	$(RM) $(TEST_PATH)/$@/main.exe $(addprefix $(TEST_PATH)/$@/, main.o Test.o TestSuite.o)
