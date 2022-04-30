build/utils/math3d/Mesh.o: utils/math3d/Mesh.cpp utils/math3d/Mesh.h \
 define.h utils/List.h utils/os/Log.h lib/opengl/gl/gl.h main.h \
 utils/os/AppFrame.h utils/String.h utils/os/Font.h \
 utils/math3d/LinearAlgebra.h utils/math3d/Math.h res.h utils/File.h \
 utils/gl/GLUtils.h utils/gl/GLTexture2D.h utils/math3d/Geometry.h \
 editor/main/ViewObject.h utils/Delegate.h editor/main/Transform.h \
 utils/math3d/Property.h
	g++ -c utils/math3d/Mesh.cpp -o build/utils/math3d/Mesh.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
