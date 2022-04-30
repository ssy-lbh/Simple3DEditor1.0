build/utils/physics3d/mathematics/Matrix2x2.o: \
 utils/physics3d/mathematics/Matrix2x2.cpp \
 utils/physics3d/mathematics/Matrix2x2.h \
 utils/physics3d/mathematics/Vector2.h \
 utils/physics3d/mathematics/mathematics_functions.h \
 utils/physics3d/configuration.h utils/physics3d/decimal.h \
 utils/physics3d/containers/Pair.h \
 utils/physics3d/memory/MemoryAllocator.h \
 utils/physics3d/containers/containers_common.h \
 utils/physics3d/containers/List.h
	g++ -c utils/physics3d/mathematics/Matrix2x2.cpp -o build/utils/physics3d/mathematics/Matrix2x2.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
