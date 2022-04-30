build/editor/AttributeWindow.o: editor/AttributeWindow.cpp \
 editor/AttributeWindow.h define.h editor/main/Window.h \
 utils/math3d/LinearAlgebra.h utils/math3d/Math.h lib/glew/gl/glew.h \
 utils/gl/GLUtils.h editor/gui/GUIUtils.h utils/List.h utils/os/Log.h \
 utils/String.h utils/StringBuilder.h utils/Delegate.h \
 editor/object/GUIObject.h editor/main/ViewObject.h \
 utils/math3d/Geometry.h editor/main/Transform.h utils/math3d/Property.h \
 editor/object/GUIManagerObject.h
	g++ -c editor/AttributeWindow.cpp -o build/editor/AttributeWindow.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
