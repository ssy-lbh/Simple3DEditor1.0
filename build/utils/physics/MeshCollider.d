build/utils/physics/MeshCollider.o: utils/physics/MeshCollider.cpp \
 utils/physics/MeshCollider.h define.h utils/physics/Collider.h main.h \
 utils/List.h utils/os/Log.h utils/os/AppFrame.h utils/String.h \
 utils/os/Font.h utils/math3d/LinearAlgebra.h utils/math3d/Math.h \
 utils/physics/Rigidbody.h utils/physics/AllColliders.h \
 utils/physics/CubeCollider.h utils/physics/SphereCollider.h
	g++ -c utils/physics/MeshCollider.cpp -o build/utils/physics/MeshCollider.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
