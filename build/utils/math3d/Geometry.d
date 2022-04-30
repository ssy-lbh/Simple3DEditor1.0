build/utils/math3d/Geometry.o: utils/math3d/Geometry.cpp \
 utils/math3d/Geometry.h define.h utils/List.h utils/os/Log.h \
 utils/math3d/LinearAlgebra.h utils/math3d/Math.h utils/gl/GLUtils.h \
 editor/main/ViewObject.h utils/Delegate.h utils/String.h \
 editor/main/Transform.h utils/math3d/Property.h
	g++ -c utils/math3d/Geometry.cpp -o build/utils/math3d/Geometry.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
