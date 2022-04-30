build/editor/AnimationWindow.o: editor/AnimationWindow.cpp \
 editor/AnimationWindow.h define.h editor/gui/UIManager.h utils/List.h \
 utils/os/Log.h utils/String.h utils/math3d/LinearAlgebra.h \
 utils/math3d/Math.h editor/main/Window.h editor/main/Tool.h \
 editor/main/Operation.h editor/gui/Menu.h utils/Value.h utils/Delegate.h \
 lib/opengl/gl/gl.h main.h utils/os/AppFrame.h utils/os/Font.h res.h \
 utils/os/Time.h utils/gl/GLUtils.h utils/math3d/Property.h \
 editor/main/ViewObject.h utils/math3d/Geometry.h editor/main/Transform.h \
 editor/gui/AnimationCurve.h
	g++ -c editor/AnimationWindow.cpp -o build/editor/AnimationWindow.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
