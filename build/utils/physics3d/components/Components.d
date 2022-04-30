build/utils/physics3d/components/Components.o: \
 utils/physics3d/components/Components.cpp \
 utils/physics3d/components/Components.h utils/physics3d/configuration.h \
 utils/physics3d/decimal.h utils/physics3d/containers/Pair.h \
 utils/physics3d/memory/MemoryAllocator.h \
 utils/physics3d/containers/containers_common.h \
 utils/physics3d/engine/Entity.h utils/physics3d/containers/Map.h \
 utils/physics3d/mathematics/mathematics_functions.h \
 utils/physics3d/containers/List.h
	g++ -c utils/physics3d/components/Components.cpp -o build/utils/physics3d/components/Components.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
