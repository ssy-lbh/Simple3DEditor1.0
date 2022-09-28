# Simple3DEditor Makefile
# Author: lin-boheng@gitee.com

# ������:
# OpenGL OpenAL stb_image SoundTouch FFmpeg
# ���ܼ����������:
# glfw glm glew FreeType "FBX SDK" OpenRL json-cpp ShaderConductor ReactPhysics3D
# �Ժ󴴽����а����glew32��OpenAL32�Ķ�̬��

# ˵��һ�£���д��Vulkan��������Ϊ����������ûʹ��
# ����һ���սһ��

# �Ժ�SoundTouch��ReactPhysics3D���������Ҵ��㵥�������һ����̬��

# ��ֿ���������һ����Ժ���

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

# �ݹ��wildcard
rwildcard	= $(foreach D, $(wildcard $(1:=/*)), $(call rwildcard, $D, $2) $(filter $(subst *, %, $2), $D))
src2obj		= $(1:$(SOURCE_PATH)/%.cpp=$(BUILD_PATH)/%.o)

CC			:= g++#clang --target=x86_64-w64-windows-gnu
DLLTOOL		:= dlltool
CFLAGS 		:= -I"$(SOURCE_PATH)" -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8\
				-DPLATFORM_$(call toupper, $(PLATFORM)) -DARCH_$(call toupper, $(ARCH))
OFLAGS		:= -m64 -s
LFLAGS		:= -m64 -shared
LIB_FILES	:= openal/OpenAL32 glew/glew32 freetype/freetype\
				glut/glut64 ftgl/ftgl_D
LIB			:= -lopengl32 -lglu32 -lgdi32 -lcomdlg32 -lws2_32\
				$(foreach FILE, $(LIB_FILES),$(LIB_PATH)/$(FILE).lib)
#				-lstdc++ -lmingw32 -lgcc -lmingwex -lmsvcrt -lkernel32 -lpthread -lshell32 -luser32
RES  		:= windres
GIT  		:= git

# �����ĺ���.h��.cpp�Ĵ���
PROGOBJ 	:= lib/soundtouch util editor manager base io
PROGOBJ 	:= $(addprefix $(SOURCE_PATH)/, $(PROGOBJ))
PROGOBJ 	:= $(basename $(call rwildcard, $(PROGOBJ), *.cpp))
PROGOBJ 	:= $(PROGOBJ:$(SOURCE_PATH)/%=%)
PROGOBJ 	+= main
# ����Ҫ�Ĵ���
REMOVEOBJ 	:= lib/soundtouch/soundtouch_wapper
# ֻ��.cpp�Ĵ���
EXTRAOBJ	:= lib/soundtouch/mmx_optimized lib/soundtouch/sse_optimized lib/soundtouch/cpu_detect_x86
# ƽ̨��ش���
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
# ��̬��
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

# build֮ǰӦ��mkdir
all: mkdir build lib

run: build
	$(OUTPUT)

build: $(OUTPUT)

lib: $(OUTPUTDLIB)

rebuild: clean build

reexec: clean run

$(OUTPUTDLIB): $(ALLOBJ)
	$(CC) $(ALLOBJ) -o $@ $(LIB) $(LFLAGS)

$(OUTPUT): $(ALLOBJ)
	$(CC) $(ALLOBJ) -o $@ $(LIB) $(OFLAGS)

# �������ļ�
DEPFILES	:= $(CPPOBJ:%.o=%.d)

# ��ʹ��ʱ��'#'ע�͵�
# һ��������˴���������Ķ������Ҫʹ��
#! ���ʹ�ã��벻Ҫ���׸Ķ�ͷ�ļ�������
-include $(DEPFILES)

$(DEPFILES): $(BUILD_PATH)/%.d: $(SOURCE_PATH)/%.cpp
	$(CC) -MM $(CFLAGS) $< -MT $(call src2obj, $<) > $@
	$(ECHO) "	$(CC) -c $< -o $(call src2obj, $<) $(CFLAGS)" >> $@

# ע:��ʹ���������ļ�ʱʹ�����±�������
# �����Դ����
#$(PROGOBJ): $(BUILD_PATH)/%.o: $(SOURCE_PATH)/%.cpp $(SOURCE_PATH)/%.h
#	$(CC) -c $< -o $@ $(CFLAGS)

# ��һ��Ϊƽ̨��ش���Դ�ļ�
#$(PLATOBJ): $(BUILD_PATH)/platform/$(PLATFORM)/%.o: $(SOURCE_PATH)/platform/$(PLATFORM)/%.cpp $(SOURCE_PATH)/%.h
#	$(CC) -c $< -o $@ $(CFLAGS)

# ��һ��Դ�ļ�û��ͷ�ļ���ֱ�ӱ���
#$(EXTRAOBJ): $(BUILD_PATH)/%.o: $(SOURCE_PATH)/%.cpp
#	$(CC) -c $< -o $@ $(CFLAGS)

# ��ʵ�Ҹ��˾��ã���nasm������ָ��incbin�ú��˵���Դ�������൱���еģ�������ֻҪ���Ҽӻ������ƽ̨��û�����
# ����nasm�Ļ���res.h����Ͳ��Ǹ���id�ˣ�����һ��extern char[0]��ָ����Դ���ݵ�ָ�룬�����˷������ӵĹ���
$(RESOBJ): $(BUILD_PATH)/%.o: $(SOURCE_PATH)/%.rc
	$(RES) -i $< -o $@

OBJDIRS		:= $(sort $(shell $(DIRNAME) $(ALLOBJ)))
# ��һЩĿ¼���׶�㣬���м�û���ļ�
OBJDIRS 	:= $(sort $(OBJDIRS) $(shell $(DIRNAME) $(OBJDIRS)))
OBJDIRS 	:= $(sort $(OBJDIRS) $(shell $(DIRNAME) $(OBJDIRS)))

mkdir:
	-$(MKDIR) $(OBJDIRS)

# ��ǰ��֧
BRANCH		= master-2.0
# �ύ��Ϣ
COMMITMSG	= "update"
# �ϲ���֧ʱ��Ŀ���֧
MERGE 		= master-2.0

# һ���ύ����
commit:
	$(GIT) checkout $(BRANCH)
	$(GIT) commit -a -m $(COMMITMSG)
	$(GIT) push -u origin $(BRANCH)

# һ���ϲ���֧
merge:
	$(GIT) checkout $(MERGE)
	$(GIT) pull origin $(MERGE)
	$(GIT) merge $(BRANCH)
	$(GIT) status
	$(GIT) push origin $(MERGE)
	$(GIT) checkout $(BRANCH)

# git log ��־ ('q'�˳�)
# git reflog ��ʷ������־ (�������˲���)('q'�˳�)
# git reset --hard HEAD^ ���˵���һ�汾
# git reset --hard id (id �� git reflog ����ڳ���)

# ɾ��ĳ���ļ���������ʷ
# git filter-branch --force --index-filter 'git rm --cached --ignore-unmatch ${file}' --prune-empty --tag-name-filter cat -- --all

# ɾ���ֿ⻺��
# rm -rf .git/refs/original/
# git reflog expire --expire=now --all
# git gc --prune=now
# git gc --aggressive --prune=now

# $(SIGNTOOL) sign /f $(CERT) /p $(PASSWORD) $(OUTPUT)
# signtool.exe sign /f "D:/code/.Certificate/lin-boheng.pfx" /p ... main.exe
sign: $(OUTPUT)
	$(SIGNTOOL) timestamp /t $(TIMESTAMP) $(OUTPUT)

cleanall: clean cleandep

# ���ٶ��Ĳ���
FIXED_OBJ 	:= util/physics3d
FIXED_OBJ 	:= $(addprefix $(SOURCE_PATH)/, $(FIXED_OBJ))
FIXED_OBJ 	:= $(call rwildcard, $(FIXED_OBJ), *.cpp)
FIXED_OBJ 	:= $(FIXED_OBJ:$(SOURCE_PATH)/%.cpp=$(BUILD_PATH)/%.o)

clean:
	-$(RM) $(filter-out $(FIXED_OBJ), $(OUTPUT) $(OUTPUTDLIB) $(ALLOBJ))

cleandep:
	-$(RM) $(DEPFILES)

# glslc <file> --target-spv=spv1.0 [-x glsl/hlsl] -o <out> ����GLSL/HLSLΪSPIR-V
# dxc -T {stage}[[ps/vs/gs/hs/ds/cs/lib/ms]_6_[0-7]] <file> -E <entry> -I <include> [-spirv]
# spirv-as <file> -o <out> --target-env [spv1.[0-5]|vulkan1.[0-2]|opencl[1.2/2.[0-2]][embedded]|opengl4.[0,1,2,3,5]|vulkan1.1spv1.4]
# spirv-dis <file> -o <out> �����SPIR-V
# spirv-cross SPIR-V <file> [--es/--hlsl/--msl/--vulkan-semantics(-V)/--reflect/--cpp] --output <out> �������SPIR-VΪGLSL/ESSL/HLSL/MSL/JSON����/C++

# ����
.PHONY: dllboot plugin physics

# ����Ĳ������ֻ����һ�����.o�ļ���include��Ӧ��.h�������������Դ��룬�����ǵ�Ԫ������ô

# ��һ�����ԣ����������򵱳ɿ�ʹ�ò�����
dllboot: $(OUTPUTDLIB)
	$(CC) $(TEST_PATH)/$@/boot.cpp $(OUTPUTDLIB) -I"." -o $(TEST_PATH)/$@/boot.exe
	$(TEST_PATH)/$@/boot.exe
	$(RM) $(TEST_PATH)/$@/boot.exe

plugin: $(OUTPUTDLIB)
	$(CC) $(TEST_PATH)/$@/main.cpp $(OUTPUTDLIB) -I"." -o $(TEST_PATH)/$@/main.exe
	$(CC) $(TEST_PATH)/$@/plugin.cpp $(OUTPUTDLIB) -I"." -shared -o $(PLUGIN_PATH)/plugin.dll
	$(TEST_PATH)/$@/main.exe
	$(RM) $(TEST_PATH)/$@/main.exe
	$(RM) $(PLUGIN_PATH)/plugin.dll

physics: $(OUTPUTDLIB)
	$(CC) -c $(TEST_PATH)/$@/main.cpp -I"." -o $(TEST_PATH)/$@/main.o
	$(CC) -c $(TEST_PATH)/$@/Test.cpp -I"." -o $(TEST_PATH)/$@/Test.o
	$(CC) -c $(TEST_PATH)/$@/TestSuite.cpp -I"." -o $(TEST_PATH)/$@/TestSuite.o
	$(CC) $(addprefix $(TEST_PATH)/$@/, main.o Test.o TestSuite.o) $(OUTPUTDLIB) -o $(TEST_PATH)/$@/main.exe
	$(TEST_PATH)/$@/main.exe
	$(RM) $(TEST_PATH)/$@/main.exe $(addprefix $(TEST_PATH)/$@/, main.o Test.o TestSuite.o)
