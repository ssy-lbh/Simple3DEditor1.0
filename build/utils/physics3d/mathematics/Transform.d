build/utils/physics3d/mathematics/Transform.o: \
 utils/physics3d/mathematics/Transform.cpp \
 utils/physics3d/mathematics/Transform.h \
 utils/physics3d/mathematics/Vector3.h \
 utils/physics3d/mathematics/mathematics_functions.h \
 utils/physics3d/configuration.h utils/physics3d/decimal.h \
 utils/physics3d/containers/Pair.h \
 utils/physics3d/memory/MemoryAllocator.h \
 utils/physics3d/containers/containers_common.h \
 utils/physics3d/containers/List.h \
 utils/physics3d/mathematics/Quaternion.h \
 utils/physics3d/mathematics/Matrix3x3.h
	g++ -c utils/physics3d/mathematics/Transform.cpp -o build/utils/physics3d/mathematics/Transform.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
