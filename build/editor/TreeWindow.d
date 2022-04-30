build/editor/TreeWindow.o: editor/TreeWindow.cpp editor/TreeWindow.h \
 define.h utils/List.h utils/os/Log.h editor/main/Window.h \
 utils/math3d/LinearAlgebra.h utils/math3d/Math.h lib/opengl/gl/gl.h \
 main.h utils/os/AppFrame.h utils/String.h utils/os/Font.h res.h \
 editor/gui/Menu.h utils/Value.h utils/Delegate.h editor/gui/GUIUtils.h \
 utils/StringBuilder.h editor/object/GUIObject.h editor/main/ViewObject.h \
 utils/math3d/Geometry.h editor/main/Transform.h utils/math3d/Property.h \
 editor/dialog/Tips.h utils/gl/GLUtils.h editor/object/AllObjects.h \
 editor/object/AudioListenerObject.h editor/object/AudioSourceObject.h \
 editor/object/CameraObject.h editor/object/CubicBezierObject.h \
 editor/object/MeshObject.h editor/object/PointLightObject.h \
 editor/object/GUIManagerObject.h editor/object/GUIMeshObject.h \
 utils/gl/GLRenderTexture2D.h utils/gl/GLTexture2D.h \
 utils/gl/GLFrameBuffer.h editor/windows/AllWindows.h \
 editor/windows/AudioPlayerWindowObject.h editor/object/WindowObject.h \
 editor/windows/TreeWindowObject.h
	g++ -c editor/TreeWindow.cpp -o build/editor/TreeWindow.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
