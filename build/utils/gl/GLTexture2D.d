build/utils/gl/GLTexture2D.o: utils/gl/GLTexture2D.cpp \
 utils/gl/GLTexture2D.h define.h lib/glew/gl/glew.h utils/String.h \
 utils/DataBuffer.h utils/os/Resource.h utils/os/Log.h \
 utils/gl/GLRenderTexture2D.h
	g++ -c utils/gl/GLTexture2D.cpp -o build/utils/gl/GLTexture2D.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
