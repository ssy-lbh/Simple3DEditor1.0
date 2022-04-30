build/editor/main/ViewManager.o: editor/main/ViewManager.cpp \
 editor/main/ViewManager.h define.h utils/List.h utils/os/Log.h \
 utils/math3d/LinearAlgebra.h utils/math3d/Math.h utils/os/Thread.h \
 utils/os/AppFrame.h utils/String.h utils/os/Font.h
	g++ -c editor/main/ViewManager.cpp -o build/editor/main/ViewManager.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
