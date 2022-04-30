build/utils/String.o: utils/String.cpp utils/String.h define.h \
 utils/os/Log.h utils/os/System.h utils/os/Font.h
	g++ -c utils/String.cpp -o build/utils/String.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
