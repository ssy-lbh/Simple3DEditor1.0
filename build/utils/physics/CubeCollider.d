build/utils/physics/CubeCollider.o: utils/physics/CubeCollider.cpp \
 utils/physics/CubeCollider.h define.h utils/math3d/LinearAlgebra.h \
 utils/math3d/Math.h utils/physics/Collider.h utils/os/Time.h \
 utils/physics/Rigidbody.h utils/physics/AllColliders.h \
 utils/physics/SphereCollider.h utils/physics/MeshCollider.h
	g++ -c utils/physics/CubeCollider.cpp -o build/utils/physics/CubeCollider.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
