build/editor/gui/Menu.o: editor/gui/Menu.cpp editor/gui/Menu.h define.h \
 utils/List.h utils/os/Log.h utils/Value.h utils/Delegate.h \
 utils/math3d/LinearAlgebra.h utils/math3d/Math.h lib/opengl/gl/gl.h \
 utils/os/Font.h utils/gl/GLUtils.h
	g++ -c editor/gui/Menu.cpp -o build/editor/gui/Menu.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
