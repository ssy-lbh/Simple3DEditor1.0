build/lib/soundtouch/BPMDetect.o: lib/soundtouch/BPMDetect.cpp \
 lib/soundtouch/FIFOSampleBuffer.h lib/soundtouch/FIFOSamplePipe.h \
 lib/soundtouch/STTypes.h lib/soundtouch/soundtouch_config.h \
 lib/soundtouch/PeakFinder.h lib/soundtouch/BPMDetect.h
	g++ -c lib/soundtouch/BPMDetect.cpp -o build/lib/soundtouch/BPMDetect.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
