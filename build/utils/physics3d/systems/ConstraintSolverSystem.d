build/utils/physics3d/systems/ConstraintSolverSystem.o: \
 utils/physics3d/systems/ConstraintSolverSystem.cpp \
 utils/physics3d/systems/ConstraintSolverSystem.h \
 utils/physics3d/configuration.h utils/physics3d/decimal.h \
 utils/physics3d/containers/Pair.h \
 utils/physics3d/memory/MemoryAllocator.h \
 utils/physics3d/containers/containers_common.h \
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
 utils/physics3d/systems/SolveBallAndSocketJointSystem.h \
 utils/physics3d/utils/Profiler.h \
 utils/physics3d/components/RigidBodyComponents.h \
 utils/physics3d/engine/Entity.h utils/physics3d/components/Components.h \
 utils/physics3d/containers/Map.h \
 utils/physics3d/components/JointComponents.h \
 utils/physics3d/components/BallAndSocketJointComponents.h \
 utils/physics3d/components/TransformComponents.h \
 utils/physics3d/systems/SolveFixedJointSystem.h \
 utils/physics3d/components/FixedJointComponents.h \
 utils/physics3d/systems/SolveHingeJointSystem.h \
 utils/physics3d/components/HingeJointComponents.h \
 utils/physics3d/systems/SolveSliderJointSystem.h \
 utils/physics3d/components/SliderJointComponents.h \
 utils/physics3d/engine/Island.h utils/physics3d/constraint/Joint.h \
 utils/physics3d/body/RigidBody.h utils/physics3d/body/CollisionBody.h \
 utils/physics3d/collision/shapes/AABB.h
	g++ -c utils/physics3d/systems/ConstraintSolverSystem.cpp -o build/utils/physics3d/systems/ConstraintSolverSystem.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
