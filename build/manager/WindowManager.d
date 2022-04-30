build/manager/WindowManager.o: manager/WindowManager.cpp \
 manager/WindowManager.h define.h utils/List.h utils/os/Log.h \
 utils/String.h
	g++ -c manager/WindowManager.cpp -o build/manager/WindowManager.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
