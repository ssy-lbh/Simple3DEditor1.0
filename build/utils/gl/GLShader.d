build/utils/gl/GLShader.o: utils/gl/GLShader.cpp utils/gl/GLShader.h \
 define.h lib/glew/gl/glew.h utils/String.h utils/DataBuffer.h \
 utils/os/Log.h utils/os/Resource.h
	g++ -c utils/gl/GLShader.cpp -o build/utils/gl/GLShader.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
