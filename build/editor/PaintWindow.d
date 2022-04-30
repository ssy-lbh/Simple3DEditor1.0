build/editor/PaintWindow.o: editor/PaintWindow.cpp editor/PaintWindow.h \
 define.h utils/math3d/LinearAlgebra.h utils/math3d/Math.h \
 editor/main/Tool.h editor/main/Operation.h editor/main/Window.h \
 lib/glew/gl/glew.h main.h utils/List.h utils/os/Log.h \
 utils/os/AppFrame.h utils/String.h utils/os/Font.h res.h \
 editor/gui/Menu.h utils/Value.h utils/Delegate.h \
 editor/dialog/ColorBoard.h utils/gl/GLUtils.h utils/math3d/Mesh.h \
 utils/gl/GLTexture2D.h utils/gl/GLRenderTexture2D.h \
 utils/gl/GLFrameBuffer.h utils/gl/GLComputeProgram.h
	g++ -c editor/PaintWindow.cpp -o build/editor/PaintWindow.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
