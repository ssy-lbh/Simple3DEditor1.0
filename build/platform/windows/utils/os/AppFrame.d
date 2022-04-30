build/platform/windows/utils/os/AppFrame.o: \
 platform/windows/utils/os/AppFrame.cpp utils/os/AppFrame.h define.h \
 utils/String.h utils/os/Font.h main.h utils/List.h utils/os/Log.h \
 utils/math3d/LinearAlgebra.h utils/math3d/Math.h res.h utils/os/Time.h \
 utils/os/Thread.h utils/os/System.h utils/gl/GLEW.h lib/glew/gl/glew.h \
 editor/main/ViewManager.h editor/main/ViewObject.h utils/Delegate.h \
 utils/math3d/Geometry.h editor/main/Transform.h utils/math3d/Property.h \
 editor/gui/UIManager.h editor/main/Window.h editor/main/Tool.h \
 editor/main/Operation.h
	g++ -c platform/windows/utils/os/AppFrame.cpp -o build/platform/windows/utils/os/AppFrame.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
