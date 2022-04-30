build/utils/gl/GLRenderTexture2D.o: utils/gl/GLRenderTexture2D.cpp \
 utils/gl/GLRenderTexture2D.h define.h utils/gl/GLTexture2D.h \
 lib/glew/gl/glew.h utils/os/Log.h
	g++ -c utils/gl/GLRenderTexture2D.cpp -o build/utils/gl/GLRenderTexture2D.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
