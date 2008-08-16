/*
 *  mt_cellmap.cpp
 *  MacTierra
 *
 *  Created by Simon Fraser on 8/12/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <iostream>

#include "mt_cellmap.h"

#include "mt_creature.h"

namespace MacTierra {

using namespace std;

CellMap::CellMap(u_int32_t inSize)
: mSize(inSize)
{
}

CellMap::~CellMap()
{
}

Creature*
CellMap::creatureAtAddress(address_t inAddress) const
{
    size_t index;
    if (indexOfCreatureAtAddress(inAddress, index))
        return mCells[index].mData;
    
    return NULL;
}

bool
CellMap::spaceAtAddress(address_t inAddress, u_int32_t inLength) const
{
    size_t index;
    return spaceAtAddress(inAddress, inLength, index);
}

bool
CellMap::spaceAtAddress(address_t inAddress, u_int32_t inLength, size_t& outIndex) const
{
    outIndex = -1;
    assert(inAddress < mSize);
    if (mCells.empty() && inLength <= mSize)
    {
        outIndex = 0;
        return true;
    }
    
    const size_t numCreatures = mCells.size();

    // if request wraps
    if (inAddress + inLength > mSize)
    {
        const CreatureCell& lastCreature = mCells[numCreatures - 1];
        if (lastCreature.wraps(mSize))
            return false;

        address_t endAddr = (inAddress + inLength) % mSize;
        if (!(inAddress >= lastCreature.end() && endAddr <= mCells[0].start()))
            return false;
        
        outIndex = numCreatures;
        return true;
    }
    
    address_t requestEnd = inAddress + inLength;
    
    size_t index = indexAtOrBefore(inAddress);
    assert(index < mCells.size());
    
    // check range at index
    if (mCells[index].containsOffset(inAddress, mSize))
        return false;

    if (index == 0)  // index = 0 case is special because it covers the range from 0 to the start of the second creature
    {
        // first check tail of the last (maybe wrapping) creature
        if (mCells[numCreatures - 1].containsOffset(inAddress, mSize))
            return false;
        
        
        // make sure we're in the gap before the first, or between the first and second
        bool canFit = requestEnd <= mCells[0].start() ||
                      (inAddress >= mCells[0].end() && ((numCreatures == 1) || requestEnd <= mCells[1].start()));
        if (!canFit)
            return false;
    }
    else
    {
        // check end of range with range after index
        if ((index < numCreatures - 1) && requestEnd > mCells[index + 1].start())
            return false;
    }
    
    outIndex = index + 1;
    return true;
}

bool
CellMap::insertCreature(Creature* inCreature)
{
    size_t insertionIndex;
    if (!spaceAtAddress(inCreature->location(), inCreature->length(), insertionIndex))
        return false;

    // insert at the given index
    CreatureList::iterator insertPos = mCells.begin() + insertionIndex;
    mCells.insert(insertPos, CreatureCell(inCreature->location(), inCreature->length(), inCreature));
    return true;
}

void
CellMap::removeCreature(Creature* inCreature)
{
    size_t index;
    if (indexOfCreatureAtAddress(inCreature->location(), index) && mCells[index].mData == inCreature)
    {
        CreatureList::iterator it = mCells.begin() + index;
        mCells.erase(it);
    }
    
    assert(0);
}

bool
CellMap::searchForSpace(address_t inAddress, u_int32_t inLength, ESearchDirection inSearchDirection) const
{
    return false;
}

void
CellMap::printCreatures() const
{
    cout << "Cell Map Creatures" << endl;

    for (CreatureList::const_iterator it = mCells.begin();
         it != mCells.end();
         ++it)
    {
        CreatureCell    cell = (*it);
        Creature*       creature = cell.mData;

        cout << cell.mStart << " creature (" << creature->location() << ", " << creature->length() << ")" << endl;
    }
    
    cout << endl;
}

// return the index at or before the given address
bool
CellMap::indexOfCreatureAtAddress(address_t inAddress, size_t& outIndex) const
{
    outIndex = 0;

    if (mCells.empty())
        return false;

    int32_t  mid, low = 0, high = mCells.size() - 1;

    while (low <= high)
    {
        mid = (low + high) / 2;
        if (inAddress < mCells[mid].start())
            high = mid - 1;
        else if (inAddress >= mCells[mid].end())
            low = mid + 1;
        else
        {
            outIndex = mid;
            return true;
        }
    }
    
    // check for wrapped creature
    if (inAddress < mCells[0].start())
    {
        size_t  lastIndex = mCells.size() - 1;
        if (mCells[lastIndex].containsOffset(inAddress, mSize))
        {
            outIndex = lastIndex;
            return true;
        }
    }
    return false;
}

size_t
CellMap::indexAtOrBefore(address_t inAddress) const
{
    if (mCells.empty())
        return 0;

    const int32_t numCreatures = mCells.size();
    int32_t  mid, low = 0, high = numCreatures - 1;
    int32_t foundIndex = 0;
    
    while (low <= high)
    {
        mid = (low + high) / 2;
        if (inAddress < mCells[mid].start())
        {
            high = mid - 1;
            foundIndex = high;
        }
        else if (inAddress > mCells[mid].start())
        {
            low = mid + 1;
            foundIndex = mid;
        }
        else
        {
            foundIndex = mid;
            return true;
        }
    }
    
    // check for wrapped creature
    if (inAddress < mCells[0].start())
    {
        size_t  lastIndex = numCreatures - 1;
        if (mCells[lastIndex].containsOffset(inAddress, mSize))
            foundIndex = lastIndex;
    }
    return min(max(foundIndex, 0), numCreatures - 1);
}


} // namespace MacTierra
