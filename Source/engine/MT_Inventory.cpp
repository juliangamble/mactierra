/*
 *  MT_Inventory.cpp
 *  MacTierra
 *
 *  Created by Simon Fraser on 8/18/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <iostream>
#include <sstream>

#include "MT_Inventory.h"

namespace MacTierra {

using namespace std;

InventoryGenotype::InventoryGenotype(const std::string& inName, const genotype_t& inGenotype)
: Genotype(inName, inGenotype)
, mNumAlive(0)
, mNumEverLived(0)
, mOriginInstructions(0)
, mOriginGenerations(0)
{
}


#pragma mark -

Inventory::Inventory()
: mNumSpeciesEver(0)
, mNumSpeciesCurrent(0)
, mSpeciationCount(0)
, mExtinctionCount(0)
{
}

Inventory::~Inventory()
{
    // FIXME: delete stuff
}

InventoryGenotype*
Inventory::findGenotype(const genotype_t& inGenotype) const
{
    InventoryMap::const_iterator it = mInventoryMap.find(inGenotype);
    return (it != mInventoryMap.end()) ? it->second  : NULL;
}

bool
Inventory::enterGenotype(const genotype_t& inGenotype, InventoryGenotype*& outGenotype)
{
    InventoryMap::const_iterator it = mInventoryMap.find(inGenotype);
    if (it == mInventoryMap.end())
    {
        // not found. make a new one.
        string name = uniqueNameForLength(inGenotype.length());

        InventoryGenotype* newGenotype = new InventoryGenotype(name, inGenotype);
        mInventoryMap[inGenotype] = newGenotype;
        mGenotypeSizeMap.insert(pair<u_int32_t, InventoryGenotype*>(inGenotype.length(), newGenotype));

        outGenotype = newGenotype;
        return true;
    }

    // it exists already
    outGenotype = it->second;
    return false;
}

static std::string incrementString(const std::string& inString)
{
    string tempString(inString);
    
    size_t len = tempString.length();
    size_t pos = len - 1;
    while (pos >= 0)
    {
        if (tempString[pos] <= 0)       // we hit the numbers
            break;

        if (tempString[pos] < 'z')
        {
            ++tempString[pos];
            break;
        }
        --pos;
    }

    return tempString;
}

void
Inventory::printCreatures() const
{
    cout << "Inventory" << endl;
    
    InventoryMap::const_iterator it, end;
    
    for (it = mInventoryMap.begin(), end = mInventoryMap.end();
         it != end;
         ++it)
    {
        const InventoryGenotype* curEntry = it->second;
        cout << curEntry->name() << " alive: " << curEntry->numberAlive() << " total: " << curEntry->numberEverLived() << endl;
    }

}

std::string
Inventory::uniqueNameForLength(u_int32_t inLength) const
{
    std::ostringstream formatter;
    formatter << inLength;
    pair<SizeMap::const_iterator, SizeMap::const_iterator> sizeRange = mGenotypeSizeMap.equal_range(inLength);
    
    if (sizeRange.first == sizeRange.second)
    {
        formatter << "aaaaa";
        return formatter.str();
    }
    
    SizeMap::const_iterator last = sizeRange.second;
    --last;
    const Genotype* lastGenotype = last->second;
    return incrementString(lastGenotype->name());
}


} // namespace MacTierra