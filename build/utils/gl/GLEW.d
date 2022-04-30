build/utils/gl/GLEW.o: utils/gl/GLEW.cpp utils/gl/GLEW.h \
 lib/glew/gl/glew.h lib/glew/gl/wglew.h utils/os/Log.h
	g++ -c utils/gl/GLEW.cpp -o build/utils/gl/GLEW.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
