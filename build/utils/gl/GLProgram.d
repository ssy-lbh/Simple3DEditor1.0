build/utils/gl/GLProgram.o: utils/gl/GLProgram.cpp utils/gl/GLProgram.h \
 define.h lib/glew/gl/glew.h utils/os/Log.h utils/gl/GLShader.h
	g++ -c utils/gl/GLProgram.cpp -o build/utils/gl/GLProgram.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
