build/editor/object/GUIObject.o: editor/object/GUIObject.cpp \
 editor/object/GUIObject.h define.h editor/main/ViewObject.h utils/List.h \
 utils/os/Log.h utils/Delegate.h utils/math3d/Geometry.h \
 utils/math3d/LinearAlgebra.h utils/math3d/Math.h utils/String.h \
 editor/main/Transform.h utils/math3d/Property.h
	g++ -c editor/object/GUIObject.cpp -o build/editor/object/GUIObject.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
