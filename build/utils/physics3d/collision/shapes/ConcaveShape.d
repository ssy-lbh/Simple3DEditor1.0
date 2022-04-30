build/utils/physics3d/collision/shapes/ConcaveShape.o: \
 utils/physics3d/collision/shapes/ConcaveShape.cpp \
 utils/physics3d/collision/shapes/ConcaveShape.h \
 utils/physics3d/collision/shapes/CollisionShape.h \
 utils/physics3d/configuration.h utils/physics3d/decimal.h \
 utils/physics3d/containers/Pair.h \
 utils/physics3d/memory/MemoryAllocator.h \
 utils/physics3d/containers/containers_common.h \
 utils/physics3d/utils/Profiler.h utils/physics3d/containers/List.h \
 utils/physics3d/collision/shapes/TriangleShape.h \
 utils/physics3d/mathematics/mathematics.h \
 utils/physics3d/mathematics/Matrix3x3.h \
 utils/physics3d/mathematics/Vector3.h \
 utils/physics3d/mathematics/mathematics_functions.h \
 utils/physics3d/mathematics/Matrix2x2.h \
 utils/physics3d/mathematics/Vector2.h \
 utils/physics3d/mathematics/Quaternion.h \
 utils/physics3d/mathematics/Transform.h \
 utils/physics3d/mathematics/Ray.h \
 utils/physics3d/collision/shapes/ConvexPolyhedronShape.h \
 utils/physics3d/collision/shapes/ConvexShape.h \
 utils/physics3d/collision/HalfEdgeStructure.h
	g++ -c utils/physics3d/collision/shapes/ConcaveShape.cpp -o build/utils/physics3d/collision/shapes/ConcaveShape.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
