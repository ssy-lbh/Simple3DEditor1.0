build/utils/gl/GLLights.o: utils/gl/GLLights.cpp utils/gl/GLLights.h \
 define.h lib/opengl/gl/gl.h utils/os/Log.h
	g++ -c utils/gl/GLLights.cpp -o build/utils/gl/GLLights.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
