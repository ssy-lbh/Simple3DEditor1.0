build/platform/windows/utils/os/System.o: \
 platform/windows/utils/os/System.cpp utils/os/System.h
	g++ -c platform/windows/utils/os/System.cpp -o build/platform/windows/utils/os/System.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
