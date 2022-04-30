build/editor/windows/AudioPlayerWindowObject.o: \
 editor/windows/AudioPlayerWindowObject.cpp \
 editor/windows/AudioPlayerWindowObject.h define.h \
 editor/object/WindowObject.h editor/object/GUIObject.h \
 editor/main/ViewObject.h utils/List.h utils/os/Log.h utils/Delegate.h \
 utils/math3d/Geometry.h utils/math3d/LinearAlgebra.h utils/math3d/Math.h \
 utils/String.h editor/main/Transform.h utils/math3d/Property.h \
 editor/object/AudioSourceObject.h lib/openal/al.h main.h \
 utils/os/AppFrame.h utils/os/Font.h res.h utils/os/Shell.h \
 utils/os/Resource.h utils/File.h utils/gl/GLUtils.h \
 utils/gl/GLSimplified.h lib/opengl/gl/gl.h utils/AudioUtils.h \
 utils/StringBuilder.h editor/gui/Menu.h utils/Value.h \
 editor/gui/GUIUtils.h editor/object/GUIManagerObject.h
	g++ -c editor/windows/AudioPlayerWindowObject.cpp -o build/editor/windows/AudioPlayerWindowObject.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
