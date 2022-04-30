build/platform/windows/utils/os/Log.o: platform/windows/utils/os/Log.cpp \
 utils/os/Log.h
	g++ -c platform/windows/utils/os/Log.cpp -o build/platform/windows/utils/os/Log.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
