build/utils/math3d/Camera.o: utils/math3d/Camera.cpp \
 utils/math3d/Camera.h define.h utils/math3d/LinearAlgebra.h \
 utils/math3d/Math.h lib/opengl/gl/gl.h lib/opengl/gl/glu.h
	g++ -c utils/math3d/Camera.cpp -o build/utils/math3d/Camera.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
