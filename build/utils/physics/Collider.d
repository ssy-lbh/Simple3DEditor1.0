build/utils/physics/Collider.o: utils/physics/Collider.cpp \
 utils/physics/Collider.h define.h utils/math3d/LinearAlgebra.h \
 utils/math3d/Math.h utils/physics/Rigidbody.h \
 utils/physics/AllColliders.h utils/physics/CubeCollider.h \
 utils/physics/SphereCollider.h utils/physics/MeshCollider.h \
 editor/main/ViewObject.h utils/List.h utils/os/Log.h utils/Delegate.h \
 utils/math3d/Geometry.h utils/String.h editor/main/Transform.h \
 utils/math3d/Property.h
	g++ -c utils/physics/Collider.cpp -o build/utils/physics/Collider.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
