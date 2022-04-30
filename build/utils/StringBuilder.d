build/utils/StringBuilder.o: utils/StringBuilder.cpp \
 utils/StringBuilder.h define.h utils/String.h utils/os/Log.h
	g++ -c utils/StringBuilder.cpp -o build/utils/StringBuilder.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
