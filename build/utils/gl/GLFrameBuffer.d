build/utils/gl/GLFrameBuffer.o: utils/gl/GLFrameBuffer.cpp \
 utils/gl/GLFrameBuffer.h define.h utils/math3d/LinearAlgebra.h \
 utils/math3d/Math.h lib/glew/gl/glew.h editor/main/ViewManager.h \
 utils/List.h utils/os/Log.h utils/gl/GLRenderTexture2D.h \
 utils/gl/GLTexture2D.h
	g++ -c utils/gl/GLFrameBuffer.cpp -o build/utils/gl/GLFrameBuffer.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
