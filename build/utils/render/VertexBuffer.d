build/utils/render/VertexBuffer.o: utils/render/VertexBuffer.cpp \
 utils/render/VertexBuffer.h define.h lib/glew/gl/glew.h \
 utils/math3d/LinearAlgebra.h utils/math3d/Math.h
	g++ -c utils/render/VertexBuffer.cpp -o build/utils/render/VertexBuffer.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
