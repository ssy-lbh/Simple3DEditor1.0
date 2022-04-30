build/lib/soundtouch/FIRFilter.o: lib/soundtouch/FIRFilter.cpp \
 lib/soundtouch/FIRFilter.h lib/soundtouch/STTypes.h \
 lib/soundtouch/soundtouch_config.h lib/soundtouch/cpu_detect.h
	g++ -c lib/soundtouch/FIRFilter.cpp -o build/lib/soundtouch/FIRFilter.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
