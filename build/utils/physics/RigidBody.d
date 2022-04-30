build/utils/physics/RigidBody.o: utils/physics/RigidBody.cpp \
 utils/physics/Rigidbody.h define.h utils/math3d/LinearAlgebra.h \
 utils/math3d/Math.h editor/main/ViewObject.h utils/List.h utils/os/Log.h \
 utils/Delegate.h utils/math3d/Geometry.h utils/String.h \
 editor/main/Transform.h utils/math3d/Property.h
	g++ -c utils/physics/RigidBody.cpp -o build/utils/physics/RigidBody.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
