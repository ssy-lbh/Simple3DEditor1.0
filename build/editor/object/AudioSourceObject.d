build/editor/object/AudioSourceObject.o: \
 editor/object/AudioSourceObject.cpp editor/object/AudioSourceObject.h \
 define.h utils/math3d/LinearAlgebra.h utils/math3d/Math.h \
 editor/main/ViewObject.h utils/List.h utils/os/Log.h utils/Delegate.h \
 utils/math3d/Geometry.h utils/String.h editor/main/Transform.h \
 utils/math3d/Property.h lib/openal/al.h main.h utils/os/AppFrame.h \
 utils/os/Font.h utils/gl/GLSimplified.h lib/opengl/gl/gl.h \
 utils/AudioUtils.h utils/os/Time.h editor/AudioPlayerWindow.h \
 editor/main/Window.h
	g++ -c editor/object/AudioSourceObject.cpp -o build/editor/object/AudioSourceObject.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
