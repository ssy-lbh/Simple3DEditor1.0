build/utils/DataBuffer.o: utils/DataBuffer.cpp utils/DataBuffer.h \
 define.h utils/os/Log.h
	g++ -c utils/DataBuffer.cpp -o build/utils/DataBuffer.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
