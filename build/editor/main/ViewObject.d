build/editor/main/ViewObject.o: editor/main/ViewObject.cpp \
 editor/main/ViewObject.h define.h utils/List.h utils/os/Log.h \
 utils/Delegate.h utils/math3d/Geometry.h utils/math3d/LinearAlgebra.h \
 utils/math3d/Math.h utils/String.h editor/main/Transform.h \
 utils/math3d/Property.h lib/openal/al.h main.h utils/os/AppFrame.h \
 utils/os/Font.h res.h editor/MainWindow.h utils/math3d/Camera.h \
 editor/main/Window.h editor/main/Tool.h editor/main/Operation.h \
 editor/gui/Menu.h utils/Value.h editor/AudioPlayerWindow.h \
 editor/object/AudioSourceObject.h utils/AudioUtils.h utils/os/Time.h \
 utils/math3d/Mesh.h utils/gl/GLSimplified.h lib/opengl/gl/gl.h \
 utils/gl/GLLights.h utils/gl/GLUtils.h
	g++ -c editor/main/ViewObject.cpp -o build/editor/main/ViewObject.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
