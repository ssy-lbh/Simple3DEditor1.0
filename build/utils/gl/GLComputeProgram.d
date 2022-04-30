build/utils/gl/GLComputeProgram.o: utils/gl/GLComputeProgram.cpp \
 utils/gl/GLComputeProgram.h define.h lib/glew/gl/glew.h res.h \
 utils/DataBuffer.h utils/os/Resource.h utils/os/Log.h \
 utils/gl/GLTexture2D.h utils/gl/GLRenderTexture2D.h
	g++ -c utils/gl/GLComputeProgram.cpp -o build/utils/gl/GLComputeProgram.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
