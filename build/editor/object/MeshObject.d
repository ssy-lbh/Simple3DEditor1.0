build/editor/object/MeshObject.o: editor/object/MeshObject.cpp \
 editor/object/MeshObject.h define.h editor/main/ViewObject.h \
 utils/List.h utils/os/Log.h utils/Delegate.h utils/math3d/Geometry.h \
 utils/math3d/LinearAlgebra.h utils/math3d/Math.h utils/String.h \
 editor/main/Transform.h utils/math3d/Property.h lib/glut/glut.h main.h \
 utils/os/AppFrame.h utils/os/Font.h utils/math3d/Mesh.h \
 utils/gl/GLSimplified.h
	g++ -c editor/object/MeshObject.cpp -o build/editor/object/MeshObject.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
