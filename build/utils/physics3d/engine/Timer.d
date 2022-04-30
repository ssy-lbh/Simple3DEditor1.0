build/utils/physics3d/engine/Timer.o: utils/physics3d/engine/Timer.cpp \
 utils/physics3d/engine/Timer.h utils/physics3d/configuration.h \
 utils/physics3d/decimal.h utils/physics3d/containers/Pair.h \
 utils/physics3d/memory/MemoryAllocator.h \
 utils/physics3d/containers/containers_common.h
	g++ -c utils/physics3d/engine/Timer.cpp -o build/utils/physics3d/engine/Timer.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
