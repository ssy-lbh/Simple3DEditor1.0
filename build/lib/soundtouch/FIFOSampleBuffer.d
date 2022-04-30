build/lib/soundtouch/FIFOSampleBuffer.o: \
 lib/soundtouch/FIFOSampleBuffer.cpp lib/soundtouch/FIFOSampleBuffer.h \
 lib/soundtouch/FIFOSamplePipe.h lib/soundtouch/STTypes.h \
 lib/soundtouch/soundtouch_config.h
	g++ -c lib/soundtouch/FIFOSampleBuffer.cpp -o build/lib/soundtouch/FIFOSampleBuffer.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
