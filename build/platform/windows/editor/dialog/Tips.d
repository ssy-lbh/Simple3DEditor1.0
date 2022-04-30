build/platform/windows/editor/dialog/Tips.o: \
 platform/windows/editor/dialog/Tips.cpp editor/dialog/Tips.h define.h \
 res.h utils/os/Log.h utils/os/System.h
	g++ -c platform/windows/editor/dialog/Tips.cpp -o build/platform/windows/editor/dialog/Tips.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
