build/platform/windows/utils/File.o: platform/windows/utils/File.cpp \
 utils/File.h define.h utils/String.h utils/os/Log.h
	g++ -c platform/windows/utils/File.cpp -o build/platform/windows/utils/File.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
