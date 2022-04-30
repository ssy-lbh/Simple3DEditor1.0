build/lib/soundtouch/PeakFinder.o: lib/soundtouch/PeakFinder.cpp \
 lib/soundtouch/PeakFinder.h
	g++ -c lib/soundtouch/PeakFinder.cpp -o build/lib/soundtouch/PeakFinder.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
