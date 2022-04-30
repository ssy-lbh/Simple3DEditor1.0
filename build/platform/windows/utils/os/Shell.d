build/platform/windows/utils/os/Shell.o: \
 platform/windows/utils/os/Shell.cpp utils/os/Shell.h define.h \
 utils/os/AppFrame.h utils/String.h utils/os/Font.h utils/os/Log.h \
 utils/StringBuilder.h
	g++ -c platform/windows/utils/os/Shell.cpp -o build/platform/windows/utils/os/Shell.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
