/********************************************************************************
* ReactPhysics3D physics library, http://www.reactphysics3d.com                 *
* Copyright (c) 2010-2020 Daniel Chappuis                                       *
*********************************************************************************
*                                                                               *
* This software is provided 'as-is', without any express or implied warranty.   *
* In no event will the authors be held liable for any damages arising from the  *
* use of this software.                                                         *
*                                                                               *
* Permission is granted to anyone to use this software for any purpose,         *
* including commercial applications, and to alter it and redistribute it        *
* freely, subject to the following restrictions:                                *
*                                                                               *
* 1. The origin of this software must not be misrepresented; you must not claim *
*    that you wrote the original software. If you use this software in a        *
*    product, an acknowledgment in the product documentation would be           *
*    appreciated but is not required.                                           *
*                                                                               *
* 2. Altered source versions must be plainly marked as such, and must not be    *
*    misrepresented as being the original software.                             *
*                                                                               *
* 3. This notice may not be removed or altered from any source distribution.    *
*                                                                               *
********************************************************************************/

#ifndef REACTPHYSICS3D_OVERLAPPING_PAIR_CONTACT_H
#define REACTPHYSICS3D_OVERLAPPING_PAIR_CONTACT_H

// Libraries
#include <utils/physics3d/mathematics/mathematics.h>
#include <utils/physics3d/configuration.h>
#include <utils/physics3d/engine/OverlappingPairs.h>

/// ReactPhysics3D namespace
namespace reactphysics3d {

// Structure ContactPair
/**
 * This structure represents a pair of shapes that are in contact during narrow-phase.
 */
struct ContactPair {

    public:

        // -------------------- Attributes -------------------- //

        /// Overlapping pair Id
        uint64 pairId;

        /// Indices of the potential contact manifolds
        List<uint> potentialContactManifoldsIndices;

        /// Entity of the first body of the contact
        Entity body1Entity;

        /// Entity of the second body of the contact
        Entity body2Entity;

        /// Entity of the first collider of the contact
        Entity collider1Entity;

        /// Entity of the second collider of the contact
        Entity collider2Entity;

        /// True if the manifold is already in an island
        bool isAlreadyInIsland;

        /// Index of the contact pair in the array of pairs
        uint contactPairIndex;

        /// Index of the first contact manifold in the array
        uint contactManifoldsIndex;

        /// Number of contact manifolds
        int8 nbContactManifolds;

        /// Index of the first contact point in the array of contact points
        uint contactPointsIndex;

        /// Total number of contact points in all the manifolds of the contact pair
        uint nbToTalContactPoints;

        /// True if the colliders of the pair were already colliding in the previous frame
        bool collidingInPreviousFrame;

        /// True if one of the two involved colliders is a trigger
        bool isTrigger;

        // -------------------- Methods -------------------- //

        /// Constructor
        ContactPair(uint64 pairId, Entity body1Entity, Entity body2Entity, Entity collider1Entity,
                    Entity collider2Entity, uint contactPairIndex, bool collidingInPreviousFrame, bool isTrigger, MemoryAllocator& allocator)
            : pairId(pairId), potentialContactManifoldsIndices(allocator), body1Entity(body1Entity), body2Entity(body2Entity),
              collider1Entity(collider1Entity), collider2Entity(collider2Entity),
              isAlreadyInIsland(false), contactPairIndex(contactPairIndex), contactManifoldsIndex(0), nbContactManifolds(0),
              contactPointsIndex(0), nbToTalContactPoints(0), collidingInPreviousFrame(collidingInPreviousFrame), isTrigger(isTrigger) {

        }
};

}

#endif
