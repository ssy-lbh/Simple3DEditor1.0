build/editor/main/Transform.o: editor/main/Transform.cpp \
 editor/main/Transform.h define.h utils/math3d/LinearAlgebra.h \
 utils/math3d/Math.h utils/math3d/Property.h utils/os/Log.h \
 utils/gl/GLUtils.h
	g++ -c editor/main/Transform.cpp -o build/editor/main/Transform.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
