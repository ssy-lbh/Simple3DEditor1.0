build/utils/render/BufferBuilder.o: utils/render/BufferBuilder.cpp \
 utils/render/BufferBuilder.h define.h utils/List.h utils/os/Log.h \
 utils/math3d/LinearAlgebra.h utils/math3d/Math.h \
 utils/render/VertexBuffer.h lib/glew/gl/glew.h
	g++ -c utils/render/BufferBuilder.cpp -o build/utils/render/BufferBuilder.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
