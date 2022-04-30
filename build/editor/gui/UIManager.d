build/editor/gui/UIManager.o: editor/gui/UIManager.cpp \
 editor/gui/UIManager.h define.h utils/List.h utils/os/Log.h \
 utils/String.h utils/math3d/LinearAlgebra.h utils/math3d/Math.h \
 editor/main/Window.h editor/main/Tool.h editor/main/Operation.h \
 lib/opengl/gl/gl.h main.h utils/os/AppFrame.h utils/os/Font.h \
 editor/main/ViewManager.h utils/gl/GLUtils.h utils/gl/GLTexture2D.h
	g++ -c editor/gui/UIManager.cpp -o build/editor/gui/UIManager.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
