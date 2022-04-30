build/editor/UVEditWindow.o: editor/UVEditWindow.cpp \
 editor/UVEditWindow.h define.h utils/List.h utils/os/Log.h \
 editor/main/Window.h utils/math3d/LinearAlgebra.h utils/math3d/Math.h \
 editor/main/Tool.h editor/main/Operation.h lib/opengl/gl/gl.h main.h \
 utils/os/AppFrame.h utils/String.h utils/os/Font.h res.h \
 editor/gui/Menu.h utils/Value.h utils/Delegate.h utils/gl/GLTexture2D.h \
 utils/gl/GLUtils.h utils/math3d/Mesh.h utils/math3d/Geometry.h \
 editor/main/ViewObject.h editor/main/Transform.h utils/math3d/Property.h
	g++ -c editor/UVEditWindow.cpp -o build/editor/UVEditWindow.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
