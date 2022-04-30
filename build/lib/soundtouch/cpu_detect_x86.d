build/lib/soundtouch/cpu_detect_x86.o: lib/soundtouch/cpu_detect_x86.cpp \
 lib/soundtouch/cpu_detect.h lib/soundtouch/STTypes.h \
 lib/soundtouch/soundtouch_config.h
	g++ -c lib/soundtouch/cpu_detect_x86.cpp -o build/lib/soundtouch/cpu_detect_x86.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
