build/main.o: main.cpp main.h define.h utils/List.h utils/os/Log.h \
 utils/os/AppFrame.h utils/String.h utils/os/Font.h \
 utils/math3d/LinearAlgebra.h utils/math3d/Math.h lib/opengl/gl/gl.h \
 lib/glut/glut.h res.h editor/gui/Container.h editor/main/Window.h \
 editor/gui/Menu.h utils/Value.h utils/Delegate.h editor/MainWindow.h \
 utils/math3d/Camera.h editor/main/Tool.h editor/main/Operation.h \
 editor/dialog/Tips.h utils/AudioUtils.h utils/StringBuilder.h \
 utils/os/Time.h utils/os/Thread.h utils/os/Shell.h utils/os/Resource.h \
 utils/gl/GLUtils.h editor/main/ViewObject.h utils/math3d/Geometry.h \
 editor/main/Transform.h utils/math3d/Property.h \
 editor/object/MeshObject.h editor/object/GUIMeshObject.h \
 utils/gl/GLRenderTexture2D.h utils/gl/GLTexture2D.h \
 utils/gl/GLFrameBuffer.h editor/object/GUIManagerObject.h \
 editor/object/GUIObject.h editor/object/CameraObject.h \
 editor/object/AudioListenerObject.h utils/os/System.h
	g++ -c main.cpp -o build/main.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
