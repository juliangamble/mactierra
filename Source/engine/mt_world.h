/*
 *  mt_world.h
 *  MacTierra
 *
 *  Created by Simon Fraser on 8/10/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef mt_world_h
#define mt_world_h

#include <map>
#include "Random.hpp"

#include "mt_engine.h"

#include "mt_reaper.h"
#include "mt_timeslicer.h"

namespace MacTierra {

class Creature;
class ExecutionUnit;
class Soup;
class CellMap;

class World
{
public:

    World();
    ~World();

    void        initializeSoup(u_int32_t inSoupSize);

    u_int32_t   soupSize() const    { return mSoupSize; }

    Soup*       soup() const        { return mSoup; }
    CellMap*    cellMap() const     { return mCellMap; }

    Creature*   createCreature();
    void        eradicateCreature(Creature* inCreature);
    
    void        addCreatureToSoup(Creature* inCreature);
    void        removeCreatureFromSoup(Creature* inCreature);
    
    void        iterate(uint32_t inNumCycles);
    
    RandomLib::Random&  RNG()   { return mRNG; }

    bool        copyErrorPending() const { return mCopyErrorPending; }

    enum EMutationType {
        kAddOrDec,
        kBitFlip,
        kRandomChoice
    };
    instruction_t   mutateInstruction(instruction_t inInst, EMutationType inMutationType) const;

    void            cosmicRay();

    // settings

    EMutationType   mutationType() const    { return mMutationType; }

    enum EDaughterAllocationStrategy {
        kRandomAlloc,
        kRandomPackedAlloc,
        kClosestAlloc,
        kPreferredAlloc
    };
    
    EDaughterAllocationStrategy daughterAllocationStrategy() const;
    
    bool            globalWritesAllowed() const;
    void            setGlobalWritesAllowed(bool inAllowed);

    bool            transferRegistersToOffspring() const;
    void            setTransferRegistersToOffspring(bool inTransfer);
    
    double          reapThreshold() const { return mReapThreshold; }
    void            setReapThreshold(double inThreshold) { mReapThreshold = inThreshold; }
    
protected:

    creature_id     uniqueCreatureID();

    void            destroyCreatures();

    void            handleBirth(Creature* inParent, Creature* inChild);
    void            handleDeath(Creature* inCreature);

    int32_t         instructionFlaw(u_int64_t inInstructionCount);
    bool            cosmicRay(u_int64_t inInstructionCount);

protected:

    RandomLib::Random   mRNG;

    u_int32_t           mSoupSize;

    Soup*               mSoup;
    CellMap*            mCellMap;

    // creature book keeping
    creature_id         mNextCreatureID;

    // creatures hashed by ID
    typedef std::map<creature_id, Creature*>    CreatureIDMap;
    CreatureIDMap       mCreatureIDMap;
    
    // creature space map
    
    
    // settings
    double          mSliceSizeVariance; // sigma of normal distribution
    
    ExecutionUnit*  mExecution;
    
    TimeSlicer      mTimeSlicer;

    Reaper          mReaper;


    // runtime
    u_int32_t       mCurCreatureCycles;         // fAlive
    u_int32_t       mCurCreatureSliceCycles;    // fCurCpuSliceSize

    // maybe package these up into a "flaws" object?
    double          mCopyErrorRate;
    bool            mCopyErrorPending;
    u_int32_t       mCopiesSinceLastError;
    u_int32_t       mNextCopyError;
    
    double          mFlawRate;
    u_int64_t       mNextFlawInstruction;

    double          mCosmicRate;
    u_int64_t       mCosmicRayInstruction;
    
    double          mSizeSelection;             // size selection
    bool            mLeannessSelection;         // select for "lean" creatures

    double          mReapThreshold;     // [0, 1)

    // settings
    EMutationType   mMutationType;
    
    bool            mGlobalWritesAllowed;
    bool            mTransferRegistersToOffspring;

};







} // namespace MacTierra

#endif // mt_world_h
