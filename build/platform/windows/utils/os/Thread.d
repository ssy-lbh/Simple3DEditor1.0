build/platform/windows/utils/os/Thread.o: \
 platform/windows/utils/os/Thread.cpp utils/os/Thread.h define.h \
 utils/os/Log.h
	g++ -c platform/windows/utils/os/Thread.cpp -o build/platform/windows/utils/os/Thread.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
