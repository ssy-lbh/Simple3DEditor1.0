build/utils/physics3d/systems/DynamicsSystem.o: \
 utils/physics3d/systems/DynamicsSystem.cpp \
 utils/physics3d/systems/DynamicsSystem.h \
 utils/physics3d/utils/Profiler.h \
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
 utils/physics3d/components/RigidBodyComponents.h \
 utils/physics3d/mathematics/Matrix3x3.h \
 utils/physics3d/components/TransformComponents.h \
 utils/physics3d/components/ColliderComponents.h \
 utils/physics3d/collision/shapes/AABB.h \
 utils/physics3d/mathematics/mathematics.h \
 utils/physics3d/mathematics/Matrix2x2.h \
 utils/physics3d/mathematics/Vector2.h utils/physics3d/mathematics/Ray.h \
 utils/physics3d/body/RigidBody.h utils/physics3d/body/CollisionBody.h \
 utils/physics3d/engine/PhysicsWorld.h utils/physics3d/constraint/Joint.h \
 utils/physics3d/memory/MemoryManager.h \
 utils/physics3d/memory/DefaultAllocator.h \
 utils/physics3d/memory/PoolAllocator.h \
 utils/physics3d/memory/HeapAllocator.h \
 utils/physics3d/memory/SingleFrameAllocator.h \
 utils/physics3d/engine/EntityManager.h \
 utils/physics3d/containers/Deque.h \
 utils/physics3d/components/JointComponents.h \
 utils/physics3d/components/BallAndSocketJointComponents.h \
 utils/physics3d/components/FixedJointComponents.h \
 utils/physics3d/components/HingeJointComponents.h \
 utils/physics3d/components/SliderJointComponents.h \
 utils/physics3d/collision/CollisionCallback.h \
 utils/physics3d/collision/ContactPair.h \
 utils/physics3d/engine/OverlappingPairs.h \
 utils/physics3d/collision/Collider.h \
 utils/physics3d/collision/shapes/CollisionShape.h \
 utils/physics3d/engine/Material.h utils/physics3d/utils/Logger.h \
 utils/physics3d/containers/Set.h \
 utils/physics3d/constraint/ContactPoint.h \
 utils/physics3d/collision/ContactPointInfo.h \
 utils/physics3d/collision/OverlapCallback.h \
 utils/physics3d/systems/ConstraintSolverSystem.h \
 utils/physics3d/systems/SolveBallAndSocketJointSystem.h \
 utils/physics3d/systems/SolveFixedJointSystem.h \
 utils/physics3d/systems/SolveHingeJointSystem.h \
 utils/physics3d/systems/SolveSliderJointSystem.h \
 utils/physics3d/systems/CollisionDetectionSystem.h \
 utils/physics3d/systems/BroadPhaseSystem.h \
 utils/physics3d/collision/broadphase/DynamicAABBTree.h \
 utils/physics3d/containers/LinkedList.h \
 utils/physics3d/collision/ContactManifoldInfo.h \
 utils/physics3d/collision/ContactManifold.h \
 utils/physics3d/collision/narrowphase/NarrowPhaseInput.h \
 utils/physics3d/collision/narrowphase/NarrowPhaseInfoBatch.h \
 utils/physics3d/collision/narrowphase/SphereVsSphereNarrowPhaseInfoBatch.h \
 utils/physics3d/collision/narrowphase/CapsuleVsCapsuleNarrowPhaseInfoBatch.h \
 utils/physics3d/collision/narrowphase/SphereVsCapsuleNarrowPhaseInfoBatch.h \
 utils/physics3d/collision/narrowphase/CollisionDispatch.h \
 utils/physics3d/collision/narrowphase/SphereVsSphereAlgorithm.h \
 utils/physics3d/collision/narrowphase/NarrowPhaseAlgorithm.h \
 utils/physics3d/collision/narrowphase/SphereVsConvexPolyhedronAlgorithm.h \
 utils/physics3d/collision/narrowphase/SphereVsCapsuleAlgorithm.h \
 utils/physics3d/collision/narrowphase/CapsuleVsCapsuleAlgorithm.h \
 utils/physics3d/collision/narrowphase/CapsuleVsConvexPolyhedronAlgorithm.h \
 utils/physics3d/collision/narrowphase/ConvexPolyhedronVsConvexPolyhedronAlgorithm.h \
 utils/physics3d/systems/ContactSolverSystem.h \
 utils/physics3d/engine/Islands.h utils/physics3d/utils/DebugRenderer.h \
 utils/physics3d/engine/EventListener.h
	g++ -c utils/physics3d/systems/DynamicsSystem.cpp -o build/utils/physics3d/systems/DynamicsSystem.o -I. -m64 -O3 -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -DPLATFORM_WINDOWS -DARCH_AMD64
