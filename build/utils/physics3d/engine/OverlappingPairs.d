build/utils/physics3d/engine/OverlappingPairs.o: \
 utils/physics3d/engine/OverlappingPairs.cpp \
 utils/physics3d/engine/OverlappingPairs.h \
 utils/physics3d/collision/Collider.h \
 utils/physics3d/body/CollisionBody.h utils/physics3d/engine/Entity.h \
 utils/physics3d/configuration.h utils/physics3d/decimal.h \
 utils/physics3d/containers/Pair.h \
 utils/physics3d/memory/MemoryAllocator.h \
 utils/physics3d/containers/containers_common.h \
 utils/physics3d/collision/shapes/AABB.h \
 utils/physics3d/mathematics/mathematics.h \
 utils/physics3d/mathematics/Matrix3x3.h \
 utils/physics3d/mathematics/Vector3.h \
 utils/physics3d/mathematics/mathematics_functions.h \
 utils/physics3d/containers/List.h \
 utils/physics3d/mathematics/Matrix2x2.h \
 utils/physics3d/mathematics/Vector2.h \
 utils/physics3d/mathematics/Quaternion.h \
 utils/physics3d/mathematics/Transform.h \
 utils/physics3d/mathematics/Ray.h \
 utils/physics3d/collision/shapes/CollisionShape.h \
 utils/physics3d/utils/Profiler.h utils/physics3d/engine/Material.h \
 utils/physics3d/utils/Logger.h utils/physics3d/containers/Map.h \
 utils/physics3d/containers/Set.h \
 utils/physics3d/components/ColliderComponents.h \
 utils/physics3d/components/Components.h \
 utils/physics3d/components/CollisionBodyComponents.h \
 utils/physics3d/components/RigidBodyComponents.h \
 utils/physics3d/collision/ContactPointInfo.h \
 utils/physics3d/collision/narrowphase/NarrowPhaseAlgorithm.h \
 utils/physics3d/collision/narrowphase/CollisionDispatch.h \
 utils/physics3d/collision/narrowphase/SphereVsSphereAlgorithm.h \
 utils/physics3d/collision/narrowphase/SphereVsConvexPolyhedronAlgorithm.h \
 utils/physics3d/collision/narrowphase/SphereVsCapsuleAlgorithm.h \
 utils/physics3d/collision/narrowphase/CapsuleVsCapsuleAlgorithm.h \
 utils/physics3d/collision/narrowphase/CapsuleVsConvexPolyhedronAlgorithm.h \
 utils/physics3d/collision/narrowphase/ConvexPolyhedronVsConvexPolyhedronAlgorithm.h
	g++ -c utils/physics3d/engine/OverlappingPairs.cpp -o build/utils/physics3d/engine/OverlappingPairs.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
