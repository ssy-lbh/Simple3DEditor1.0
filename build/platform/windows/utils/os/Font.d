build/platform/windows/utils/os/Font.o: \
 platform/windows/utils/os/Font.cpp utils/os/Font.h define.h \
 lib/glew/gl/glew.h lib/glew/gl/wglew.h lib/ftgl/FTGL/ftgl.h \
 lib/freetype/ft2build.h lib/freetype/freetype/config/ftheader.h \
 lib/freetype/freetype/freetype.h lib/freetype/freetype/config/ftconfig.h \
 lib/freetype/freetype/config/ftoption.h \
 lib/freetype/freetype/config/ftstdlib.h \
 lib/freetype/freetype/config/integer-types.h \
 lib/freetype/freetype/config/public-macros.h \
 lib/freetype/freetype/config/mac-support.h \
 lib/freetype/freetype/fttypes.h lib/freetype/freetype/ftsystem.h \
 lib/freetype/freetype/ftimage.h lib/freetype/freetype/fterrors.h \
 lib/freetype/freetype/ftmoderr.h lib/freetype/freetype/fterrdef.h \
 lib/freetype/freetype/ftglyph.h lib/freetype/freetype/ftoutln.h \
 lib/ftgl/FTGL/FTPoint.h lib/ftgl/FTGL/FTBBox.h lib/ftgl/FTGL/FTBuffer.h \
 lib/ftgl/FTGL/FTGlyph.h lib/ftgl/FTGL/FTBitmapGlyph.h \
 lib/ftgl/FTGL/FTBufferGlyph.h lib/ftgl/FTGL/FTExtrdGlyph.h \
 lib/ftgl/FTGL/FTOutlineGlyph.h lib/ftgl/FTGL/FTPixmapGlyph.h \
 lib/ftgl/FTGL/FTPolyGlyph.h lib/ftgl/FTGL/FTTextureGlyph.h \
 lib/ftgl/FTGL/FTFont.h lib/ftgl/FTGL/FTGLBitmapFont.h \
 lib/ftgl/FTGL/FTBufferFont.h lib/ftgl/FTGL/FTGLExtrdFont.h \
 lib/ftgl/FTGL/FTGLOutlineFont.h lib/ftgl/FTGL/FTGLPixmapFont.h \
 lib/ftgl/FTGL/FTGLPolygonFont.h lib/ftgl/FTGL/FTGLTextureFont.h \
 lib/ftgl/FTGL/FTLayout.h lib/ftgl/FTGL/FTSimpleLayout.h utils/String.h \
 utils/os/Log.h utils/os/AppFrame.h utils/os/Thread.h utils/math3d/Math.h \
 utils/math3d/LinearAlgebra.h
	g++ -c platform/windows/utils/os/Font.cpp -o build/platform/windows/utils/os/Font.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
