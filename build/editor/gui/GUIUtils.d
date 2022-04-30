build/editor/gui/GUIUtils.o: editor/gui/GUIUtils.cpp \
 editor/gui/GUIUtils.h define.h utils/List.h utils/os/Log.h \
 utils/String.h utils/StringBuilder.h utils/Delegate.h \
 utils/math3d/LinearAlgebra.h utils/math3d/Math.h \
 editor/object/GUIObject.h editor/main/ViewObject.h \
 utils/math3d/Geometry.h editor/main/Transform.h utils/math3d/Property.h \
 lib/glew/gl/glew.h main.h utils/os/AppFrame.h utils/os/Font.h \
 utils/gl/GLSimplified.h utils/gl/GLUtils.h utils/gl/GLTexture2D.h \
 editor/main/ViewManager.h
	g++ -c editor/gui/GUIUtils.cpp -o build/editor/gui/GUIUtils.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
