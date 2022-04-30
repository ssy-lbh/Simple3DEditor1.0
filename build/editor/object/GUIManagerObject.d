build/editor/object/GUIManagerObject.o: \
 editor/object/GUIManagerObject.cpp editor/object/GUIManagerObject.h \
 define.h editor/main/ViewObject.h utils/List.h utils/os/Log.h \
 utils/Delegate.h utils/math3d/Geometry.h utils/math3d/LinearAlgebra.h \
 utils/math3d/Math.h utils/String.h editor/main/Transform.h \
 utils/math3d/Property.h editor/object/GUIObject.h lib/glew/gl/glew.h \
 main.h utils/os/AppFrame.h utils/os/Font.h utils/gl/GLSimplified.h \
 utils/gl/GLUtils.h utils/gl/GLTexture2D.h editor/main/ViewManager.h
	g++ -c editor/object/GUIManagerObject.cpp -o build/editor/object/GUIManagerObject.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
