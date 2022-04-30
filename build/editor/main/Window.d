build/editor/main/Window.o: editor/main/Window.cpp editor/main/Window.h \
 define.h utils/math3d/LinearAlgebra.h utils/math3d/Math.h \
 editor/object/GUIManagerObject.h editor/main/ViewObject.h utils/List.h \
 utils/os/Log.h utils/Delegate.h utils/math3d/Geometry.h utils/String.h \
 editor/main/Transform.h utils/math3d/Property.h \
 editor/object/GUIObject.h
	g++ -c editor/main/Window.cpp -o build/editor/main/Window.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
