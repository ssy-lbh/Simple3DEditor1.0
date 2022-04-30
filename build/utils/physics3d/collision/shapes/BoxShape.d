build/utils/physics3d/collision/shapes/BoxShape.o: \
 utils/physics3d/collision/shapes/BoxShape.cpp \
 utils/physics3d/collision/shapes/BoxShape.h \
 utils/physics3d/collision/shapes/ConvexPolyhedronShape.h \
 utils/physics3d/collision/shapes/ConvexShape.h \
 utils/physics3d/collision/shapes/CollisionShape.h \
 utils/physics3d/configuration.h utils/physics3d/decimal.h \
 utils/physics3d/containers/Pair.h \
 utils/physics3d/memory/MemoryAllocator.h \
 utils/physics3d/containers/containers_common.h \
 utils/physics3d/utils/Profiler.h utils/physics3d/containers/List.h \
 utils/physics3d/collision/HalfEdgeStructure.h \
 utils/physics3d/mathematics/mathematics.h \
 utils/physics3d/mathematics/Matrix3x3.h \
 utils/physics3d/mathematics/Vector3.h \
 utils/physics3d/mathematics/mathematics_functions.h \
 utils/physics3d/mathematics/Matrix2x2.h \
 utils/physics3d/mathematics/Vector2.h \
 utils/physics3d/mathematics/Quaternion.h \
 utils/physics3d/mathematics/Transform.h \
 utils/physics3d/mathematics/Ray.h utils/physics3d/collision/Collider.h \
 utils/physics3d/body/CollisionBody.h utils/physics3d/engine/Entity.h \
 utils/physics3d/collision/shapes/AABB.h \
 utils/physics3d/engine/Material.h utils/physics3d/utils/Logger.h \
 utils/physics3d/containers/Map.h utils/physics3d/memory/MemoryManager.h \
 utils/physics3d/memory/DefaultAllocator.h \
 utils/physics3d/memory/PoolAllocator.h \
 utils/physics3d/memory/HeapAllocator.h \
 utils/physics3d/memory/SingleFrameAllocator.h \
 utils/physics3d/collision/RaycastInfo.h
	g++ -c utils/physics3d/collision/shapes/BoxShape.cpp -o build/utils/physics3d/collision/shapes/BoxShape.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64