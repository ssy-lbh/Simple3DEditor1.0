build/editor/RenderWindow.o: editor/RenderWindow.cpp \
 editor/RenderWindow.h define.h utils/math3d/Camera.h \
 utils/math3d/LinearAlgebra.h utils/math3d/Math.h editor/main/Window.h \
 lib/glew/gl/glew.h main.h utils/List.h utils/os/Log.h \
 utils/os/AppFrame.h utils/String.h utils/os/Font.h res.h utils/File.h \
 utils/DataBuffer.h utils/os/Shell.h utils/os/Resource.h \
 utils/gl/GLUtils.h utils/gl/GLShader.h utils/gl/GLProgram.h \
 editor/gui/Menu.h utils/Value.h utils/Delegate.h \
 editor/main/ViewManager.h editor/main/ViewObject.h \
 utils/math3d/Geometry.h editor/main/Transform.h utils/math3d/Property.h \
 editor/object/GUIManagerObject.h editor/object/GUIObject.h
	g++ -c editor/RenderWindow.cpp -o build/editor/RenderWindow.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
