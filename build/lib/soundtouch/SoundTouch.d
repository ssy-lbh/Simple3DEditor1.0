build/lib/soundtouch/SoundTouch.o: lib/soundtouch/SoundTouch.cpp \
 lib/soundtouch/SoundTouch.h lib/soundtouch/FIFOSamplePipe.h \
 lib/soundtouch/STTypes.h lib/soundtouch/soundtouch_config.h \
 lib/soundtouch/TDStretch.h lib/soundtouch/RateTransposer.h \
 lib/soundtouch/AAFilter.h lib/soundtouch/FIFOSampleBuffer.h \
 lib/soundtouch/cpu_detect.h
	g++ -c lib/soundtouch/SoundTouch.cpp -o build/lib/soundtouch/SoundTouch.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
