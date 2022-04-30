build/editor/gui/AnimationCurve.o: editor/gui/AnimationCurve.cpp \
 editor/gui/AnimationCurve.h define.h utils/List.h utils/os/Log.h \
 utils/math3d/LinearAlgebra.h utils/math3d/Math.h editor/gui/UIManager.h \
 utils/String.h editor/main/Window.h editor/main/Tool.h \
 editor/main/Operation.h lib/opengl/gl/gl.h main.h utils/os/AppFrame.h \
 utils/os/Font.h editor/gui/Menu.h utils/Value.h utils/Delegate.h \
 editor/main/ViewManager.h
	g++ -c editor/gui/AnimationCurve.cpp -o build/editor/gui/AnimationCurve.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
