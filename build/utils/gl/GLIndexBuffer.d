build/utils/gl/GLIndexBuffer.o: utils/gl/GLIndexBuffer.cpp \
 utils/gl/GLIndexBuffer.h define.h lib/glew/gl/glew.h utils/os/Log.h
	g++ -c utils/gl/GLIndexBuffer.cpp -o build/utils/gl/GLIndexBuffer.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
