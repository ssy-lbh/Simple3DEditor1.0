build/editor/main/Operation.o: editor/main/Operation.cpp \
 editor/main/Operation.h define.h
	g++ -c editor/main/Operation.cpp -o build/editor/main/Operation.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
