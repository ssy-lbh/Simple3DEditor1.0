build/editor/object/CubicBezierObject.o: \
 editor/object/CubicBezierObject.cpp editor/object/CubicBezierObject.h \
 define.h utils/math3d/Geometry.h utils/List.h utils/os/Log.h \
 utils/math3d/LinearAlgebra.h utils/math3d/Math.h \
 editor/main/ViewObject.h utils/Delegate.h utils/String.h \
 editor/main/Transform.h utils/math3d/Property.h main.h \
 utils/os/AppFrame.h utils/os/Font.h utils/math3d/Mesh.h \
 utils/gl/GLSimplified.h lib/opengl/gl/gl.h utils/gl/GLUtils.h
	g++ -c editor/object/CubicBezierObject.cpp -o build/editor/object/CubicBezierObject.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
