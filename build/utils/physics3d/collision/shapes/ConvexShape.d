build/utils/physics3d/collision/shapes/ConvexShape.o: \
 utils/physics3d/collision/shapes/ConvexShape.cpp \
 utils/physics3d/collision/shapes/ConvexShape.h \
 utils/physics3d/collision/shapes/CollisionShape.h \
 utils/physics3d/configuration.h utils/physics3d/decimal.h \
 utils/physics3d/containers/Pair.h \
 utils/physics3d/memory/MemoryAllocator.h \
 utils/physics3d/containers/containers_common.h \
 utils/physics3d/utils/Profiler.h utils/physics3d/containers/List.h \
 utils/physics3d/mathematics/Vector3.h \
 utils/physics3d/mathematics/mathematics_functions.h
	g++ -c utils/physics3d/collision/shapes/ConvexShape.cpp -o build/utils/physics3d/collision/shapes/ConvexShape.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64