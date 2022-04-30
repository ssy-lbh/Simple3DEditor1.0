build/editor/main/Tool.o: editor/main/Tool.cpp editor/main/Tool.h \
 define.h
	g++ -c editor/main/Tool.cpp -o build/editor/main/Tool.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
