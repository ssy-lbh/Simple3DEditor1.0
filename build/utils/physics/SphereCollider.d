build/utils/physics/SphereCollider.o: utils/physics/SphereCollider.cpp \
 utils/physics/SphereCollider.h define.h utils/physics/Collider.h \
 utils/os/Time.h utils/math3d/LinearAlgebra.h utils/math3d/Math.h \
 utils/physics/Rigidbody.h utils/physics/AllColliders.h \
 utils/physics/CubeCollider.h utils/physics/MeshCollider.h
	g++ -c utils/physics/SphereCollider.cpp -o build/utils/physics/SphereCollider.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
