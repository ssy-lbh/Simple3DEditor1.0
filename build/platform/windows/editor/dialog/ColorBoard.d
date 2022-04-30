build/platform/windows/editor/dialog/ColorBoard.o: \
 platform/windows/editor/dialog/ColorBoard.cpp editor/dialog/ColorBoard.h \
 utils/gl/GLUtils.h define.h utils/math3d/LinearAlgebra.h \
 utils/math3d/Math.h lib/opengl/gl/gl.h utils/os/Log.h utils/os/System.h
	g++ -c platform/windows/editor/dialog/ColorBoard.cpp -o build/platform/windows/editor/dialog/ColorBoard.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
