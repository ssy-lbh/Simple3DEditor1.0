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
DLLTOOL		= dlltool.exe
RM			= del
CFLAGS 		= -I"." -m64 -O3
OFLAGS		= -m64 -s
LFLAGS		= -m64 -shared
LIB			= -lopengl32 -lglu32 -lgdi32 -lcomdlg32 "lib\openal\OpenAL32.lib"
RES  		= windres.exe
MKDIR   	= mkdir

BUILD_PATH	= build
PROGOBJ		= main\
				lib\soundtouch\SoundTouch lib\soundtouch\TDStretch lib\soundtouch\RateTransposer\
				lib\soundtouch\AAFilter lib\soundtouch\FIRFilter lib\soundtouch\FIFOSampleBuffer\
				lib\soundtouch\PeakFinder lib\soundtouch\BPMDetect\
				utils\StringBuilder utils\DataBuffer utils\AudioUtils\
				utils\math3d\Math utils\math3d\LinearAlgebra utils\math3d\Mesh\
				utils\math3d\ViewObject utils\math3d\Geometry utils\math3d\Property\
				utils\gl\GLFrameBuffer utils\gl\GLIndexBuffer utils\gl\GLSkyBox\
				utils\gl\GLLights utils\gl\GLProgram utils\gl\GLShader\
				utils\gl\GLUtils utils\gl\GLVertexArray utils\gl\GLVertexBuffer\
				utils\gl\GLTexture2D utils\gl\GLRenderTexture2D utils\gl\GLComputeProgram\
				editor\AnimationWindow editor\AudioPlayerWindow editor\AudioCaptureWindow\
				editor\NodeMapWindow editor\TreeWindow editor\UVEditWindow editor\PaintWindow\
				editor\MainWindow editor\RenderWindow editor\gui\Container editor\gui\Menu\
				editor\gui\UIManager editor\gui\AnimationCurve
PLATOBJ		= utils\String utils\File\
				utils\os\Shell utils\os\Log utils\os\GLFunc utils\os\Thread\
				utils\os\Time utils\os\Font utils\os\Appframe utils\os\Resource\
				editor\gui\ViewManager\
				editor\dialog\ColorBoard editor\dialog\Tips
EXTRAOBJ	= lib\soundtouch\mmx_optimized lib\soundtouch\sse_optimized lib\soundtouch\cpu_detect_x86
RESOBJ		= res string
OUTPUT 		= main.exe
# 动态库
OUTPUTDLIB	= main.dll
# 静态导入库
OUTPUTSLIB	= libmain.a

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

lib: $(OUTPUTDLIB) $(OUTPUTSLIB)

rebuild: clean build

reexec: clean run

$(OUTPUTSLIB): $(OUTPUTDLIB)
	$(DLLTOOL) --dllname $< --output-lib $(OUTPUTSLIB)

$(OUTPUTDLIB): $(PROGOBJ) $(PLATOBJ) $(RESOBJ) $(EXTRAOBJ)
	$(GCC) $(PROGOBJ) $(PLATOBJ) $(RESOBJ) $(EXTRAOBJ) -o $@ $(LIB) $(LFLAGS)

$(OUTPUT): $(PROGOBJ) $(PLATOBJ) $(RESOBJ) $(EXTRAOBJ)
	$(GCC) $(PROGOBJ) $(PLATOBJ) $(RESOBJ) $(EXTRAOBJ) -o $@ $(LIB) $(OFLAGS)

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
# signtool.exe sign /f "D:/code/.Certificate/lin-boheng.pfx" /p ... main.exe
sign: $(OUTPUT)
	$(SIGNTOOL) timestamp /t $(TIMESTAMP) $(OUTPUT)

clean:
	-$(RM) $(OUTPUT) $(OUTPUTSLIB) $(OUTPUTDLIB) $(PROGOBJ) $(PLATOBJ) $(RESOBJ) $(EXTRAOBJ)

