build/utils/gl/GLVertexBuffer.o: utils/gl/GLVertexBuffer.cpp \
 utils/gl/GLVertexBuffer.h define.h lib/glew/gl/glew.h utils/os/Log.h
	g++ -c utils/gl/GLVertexBuffer.cpp -o build/utils/gl/GLVertexBuffer.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
