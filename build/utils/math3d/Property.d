build/utils/math3d/Property.o: utils/math3d/Property.cpp \
 utils/math3d/Property.h define.h utils/math3d/LinearAlgebra.h \
 utils/math3d/Math.h editor/gui/AnimationCurve.h utils/List.h \
 utils/os/Log.h editor/gui/UIManager.h utils/String.h \
 editor/main/Window.h editor/main/Tool.h editor/main/Operation.h
	g++ -c utils/math3d/Property.cpp -o build/utils/math3d/Property.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
