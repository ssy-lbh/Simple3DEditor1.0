build/utils/physics3d/components/CollisionBodyComponents.o: \
 utils/physics3d/components/CollisionBodyComponents.cpp \
 utils/physics3d/components/CollisionBodyComponents.h \
 utils/physics3d/mathematics/Transform.h \
 utils/physics3d/mathematics/Vector3.h \
 utils/physics3d/mathematics/mathematics_functions.h \
 utils/physics3d/configuration.h utils/physics3d/decimal.h \
 utils/physics3d/containers/Pair.h \
 utils/physics3d/memory/MemoryAllocator.h \
 utils/physics3d/containers/containers_common.h \
 utils/physics3d/containers/List.h \
 utils/physics3d/mathematics/Quaternion.h utils/physics3d/engine/Entity.h \
 utils/physics3d/components/Components.h utils/physics3d/containers/Map.h \
 utils/physics3d/engine/EntityManager.h \
 utils/physics3d/containers/Deque.h
	g++ -c utils/physics3d/components/CollisionBodyComponents.cpp -o build/utils/physics3d/components/CollisionBodyComponents.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
