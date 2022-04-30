build/utils/physics3d/collision/TriangleMesh.o: \
 utils/physics3d/collision/TriangleMesh.cpp \
 utils/physics3d/collision/TriangleMesh.h \
 utils/physics3d/containers/List.h utils/physics3d/configuration.h \
 utils/physics3d/decimal.h utils/physics3d/containers/Pair.h \
 utils/physics3d/memory/MemoryAllocator.h \
 utils/physics3d/containers/containers_common.h
	g++ -c utils/physics3d/collision/TriangleMesh.cpp -o build/utils/physics3d/collision/TriangleMesh.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
