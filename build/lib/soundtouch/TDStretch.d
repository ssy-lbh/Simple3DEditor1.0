build/lib/soundtouch/TDStretch.o: lib/soundtouch/TDStretch.cpp \
 lib/soundtouch/STTypes.h lib/soundtouch/soundtouch_config.h \
 lib/soundtouch/cpu_detect.h lib/soundtouch/TDStretch.h \
 lib/soundtouch/RateTransposer.h lib/soundtouch/AAFilter.h \
 lib/soundtouch/FIFOSamplePipe.h lib/soundtouch/FIFOSampleBuffer.h
	g++ -c lib/soundtouch/TDStretch.cpp -o build/lib/soundtouch/TDStretch.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
