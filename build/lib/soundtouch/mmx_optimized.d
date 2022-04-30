build/lib/soundtouch/mmx_optimized.o: lib/soundtouch/mmx_optimized.cpp \
 lib/soundtouch/cpu_detect.h lib/soundtouch/STTypes.h \
 lib/soundtouch/soundtouch_config.h lib/soundtouch/TDStretch.h \
 lib/soundtouch/RateTransposer.h lib/soundtouch/AAFilter.h \
 lib/soundtouch/FIFOSamplePipe.h lib/soundtouch/FIFOSampleBuffer.h \
 lib/soundtouch/FIRFilter.h
	g++ -c lib/soundtouch/mmx_optimized.cpp -o build/lib/soundtouch/mmx_optimized.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
