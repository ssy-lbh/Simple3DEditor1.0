build/utils/gl/GLVertexArray.o: utils/gl/GLVertexArray.cpp \
 utils/gl/GLVertexArray.h define.h lib/glew/gl/glew.h \
 utils/gl/GLVertexBuffer.h
	g++ -c utils/gl/GLVertexArray.cpp -o build/utils/gl/GLVertexArray.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
