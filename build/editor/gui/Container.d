build/editor/gui/Container.o: editor/gui/Container.cpp \
 editor/gui/Container.h define.h editor/main/Window.h \
 utils/math3d/LinearAlgebra.h utils/math3d/Math.h main.h utils/List.h \
 utils/os/Log.h utils/os/AppFrame.h utils/String.h utils/os/Font.h res.h \
 editor/AudioPlayerWindow.h editor/object/AudioSourceObject.h \
 editor/main/ViewObject.h utils/Delegate.h utils/math3d/Geometry.h \
 editor/main/Transform.h utils/math3d/Property.h \
 editor/AudioCaptureWindow.h editor/AnimationWindow.h \
 editor/gui/UIManager.h editor/main/Tool.h editor/main/Operation.h \
 editor/gui/Menu.h utils/Value.h editor/MainWindow.h \
 utils/math3d/Camera.h editor/NodeMapWindow.h editor/object/GUIObject.h \
 editor/PaintWindow.h editor/TreeWindow.h editor/UVEditWindow.h \
 editor/RenderWindow.h editor/AttributeWindow.h editor/main/ViewManager.h \
 utils/gl/GLUtils.h
	g++ -c editor/gui/Container.cpp -o build/editor/gui/Container.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
