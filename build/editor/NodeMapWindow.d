build/editor/NodeMapWindow.o: editor/NodeMapWindow.cpp \
 editor/NodeMapWindow.h define.h utils/math3d/LinearAlgebra.h \
 utils/math3d/Math.h editor/main/Window.h editor/object/GUIObject.h \
 editor/main/ViewObject.h utils/List.h utils/os/Log.h utils/Delegate.h \
 utils/math3d/Geometry.h utils/String.h editor/main/Transform.h \
 utils/math3d/Property.h main.h utils/os/AppFrame.h utils/os/Font.h res.h \
 utils/gl/GLUtils.h utils/gl/GLTexture2D.h utils/gl/GLSimplified.h \
 lib/opengl/gl/gl.h editor/gui/Menu.h utils/Value.h \
 editor/object/GUIManagerObject.h
	g++ -c editor/NodeMapWindow.cpp -o build/editor/NodeMapWindow.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
