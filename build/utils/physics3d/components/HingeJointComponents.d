build/utils/physics3d/components/HingeJointComponents.o: \
 utils/physics3d/components/HingeJointComponents.cpp \
 utils/physics3d/components/HingeJointComponents.h \
 utils/physics3d/mathematics/Transform.h \
 utils/physics3d/mathematics/Vector3.h \
 utils/physics3d/mathematics/mathematics_functions.h \
 utils/physics3d/configuration.h utils/physics3d/decimal.h \
 utils/physics3d/containers/Pair.h \
 utils/physics3d/memory/MemoryAllocator.h \
 utils/physics3d/containers/containers_common.h \
 utils/physics3d/containers/List.h \
 utils/physics3d/mathematics/Quaternion.h \
 utils/physics3d/mathematics/Matrix3x3.h \
 utils/physics3d/mathematics/Matrix2x2.h \
 utils/physics3d/mathematics/Vector2.h utils/physics3d/engine/Entity.h \
 utils/physics3d/components/Components.h utils/physics3d/containers/Map.h \
 utils/physics3d/engine/EntityManager.h \
 utils/physics3d/containers/Deque.h
	g++ -c utils/physics3d/components/HingeJointComponents.cpp -o build/utils/physics3d/components/HingeJointComponents.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
