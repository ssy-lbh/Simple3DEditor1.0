build/utils/gl/GLUtils.o: utils/gl/GLUtils.cpp utils/gl/GLUtils.h \
 define.h utils/math3d/LinearAlgebra.h utils/math3d/Math.h \
 lib/opengl/gl/gl.h utils/os/Log.h utils/gl/GLSimplified.h
	g++ -c utils/gl/GLUtils.cpp -o build/utils/gl/GLUtils.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
