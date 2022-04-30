build/editor/MainWindow.o: editor/MainWindow.cpp editor/MainWindow.h \
 define.h utils/math3d/LinearAlgebra.h utils/math3d/Math.h \
 utils/math3d/Camera.h editor/main/Window.h editor/main/Tool.h \
 editor/main/Operation.h editor/gui/Menu.h utils/List.h utils/os/Log.h \
 utils/Value.h utils/Delegate.h lib/opengl/gl/gl.h lib/opengl/gl/glu.h \
 main.h utils/os/AppFrame.h utils/String.h utils/os/Font.h res.h \
 utils/File.h utils/AudioUtils.h utils/math3d/Mesh.h utils/os/Shell.h \
 utils/os/Resource.h utils/gl/GLTexture2D.h utils/gl/GLSkyBox.h \
 utils/gl/GLSimplified.h editor/gui/GUIUtils.h utils/StringBuilder.h \
 editor/object/GUIObject.h editor/main/ViewObject.h \
 utils/math3d/Geometry.h editor/main/Transform.h utils/math3d/Property.h \
 editor/dialog/ColorBoard.h utils/gl/GLUtils.h editor/dialog/Tips.h \
 editor/main/ViewManager.h editor/object/AllObjects.h \
 editor/object/AudioListenerObject.h editor/object/AudioSourceObject.h \
 editor/object/CameraObject.h editor/object/CubicBezierObject.h \
 editor/object/MeshObject.h editor/object/PointLightObject.h \
 editor/object/GUIManagerObject.h editor/object/GUIMeshObject.h \
 utils/gl/GLRenderTexture2D.h utils/gl/GLFrameBuffer.h
	g++ -c editor/MainWindow.cpp -o build/editor/MainWindow.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
