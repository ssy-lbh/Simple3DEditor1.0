build/utils/AudioUtils.o: utils/AudioUtils.cpp utils/AudioUtils.h \
 define.h lib/openal/al.h lib/openal/alc.h utils/os/Time.h \
 utils/math3d/Math.h utils/math3d/LinearAlgebra.h utils/os/Log.h
	g++ -c utils/AudioUtils.cpp -o build/utils/AudioUtils.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
