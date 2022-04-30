build/lib/soundtouch/AAFilter.o: lib/soundtouch/AAFilter.cpp \
 lib/soundtouch/AAFilter.h lib/soundtouch/STTypes.h \
 lib/soundtouch/soundtouch_config.h lib/soundtouch/FIRFilter.h
	g++ -c lib/soundtouch/AAFilter.cpp -o build/lib/soundtouch/AAFilter.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
