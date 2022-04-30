build/editor/AudioCaptureWindow.o: editor/AudioCaptureWindow.cpp \
 editor/AudioCaptureWindow.h define.h utils/String.h \
 utils/math3d/LinearAlgebra.h utils/math3d/Math.h editor/main/Window.h \
 lib/opengl/gl/gl.h lib/openal/al.h lib/openal/alc.h main.h utils/List.h \
 utils/os/Log.h utils/os/AppFrame.h utils/os/Font.h res.h \
 utils/AudioUtils.h utils/os/Shell.h utils/gl/GLUtils.h editor/gui/Menu.h \
 utils/Value.h utils/Delegate.h editor/gui/GUIUtils.h \
 utils/StringBuilder.h editor/object/GUIObject.h editor/main/ViewObject.h \
 utils/math3d/Geometry.h editor/main/Transform.h utils/math3d/Property.h \
 editor/object/GUIManagerObject.h lib/soundtouch/SoundTouch.h \
 lib/soundtouch/FIFOSamplePipe.h lib/soundtouch/STTypes.h \
 lib/soundtouch/soundtouch_config.h
	g++ -c editor/AudioCaptureWindow.cpp -o build/editor/AudioCaptureWindow.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
