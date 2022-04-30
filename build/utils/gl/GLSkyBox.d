build/utils/gl/GLSkyBox.o: utils/gl/GLSkyBox.cpp utils/gl/GLSkyBox.h \
 define.h lib/glew/gl/glew.h utils/gl/GLTexture2D.h \
 utils/gl/GLSimplified.h utils/math3d/Math.h utils/math3d/LinearAlgebra.h
	g++ -c utils/gl/GLSkyBox.cpp -o build/utils/gl/GLSkyBox.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
