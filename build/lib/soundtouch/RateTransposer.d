build/lib/soundtouch/RateTransposer.o: lib/soundtouch/RateTransposer.cpp \
 lib/soundtouch/RateTransposer.h lib/soundtouch/AAFilter.h \
 lib/soundtouch/STTypes.h lib/soundtouch/soundtouch_config.h \
 lib/soundtouch/FIFOSamplePipe.h lib/soundtouch/FIFOSampleBuffer.h
	g++ -c lib/soundtouch/RateTransposer.cpp -o build/lib/soundtouch/RateTransposer.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
