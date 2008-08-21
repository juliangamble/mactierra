//
//  MTInventoryGenotype.mm
//  MacTierra
//
//  Created by Simon Fraser on 8/20/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "MTInventoryGenotype.h"

#import "MT_Inventory.h"

using namespace MacTierra;


@implementation MTInventoryGenotype

@synthesize genotype;
@synthesize name;
@synthesize numAlive;
@synthesize numEverLived;
@synthesize originInstructions;
@synthesize originGenerations;
@synthesize genotypeString;

- (id)initWithGenotype:(MacTierra::InventoryGenotype*)inGenotype
{
    if ((self = [super init]))
    {
        genotype = inGenotype;
    }
    return self;
}

- (NSString*)name
{
    return [NSString stringWithUTF8String:genotype->name().c_str()];
}

- (NSInteger)length
{
    return genotype->length();
}

- (NSInteger)numAlive
{
    return genotype->numberAlive();
}

- (NSInteger)numEverLived
{
    return genotype->numberEverLived();
}

- (u_int64_t)originInstructions
{
    return genotype->originInstructions();
}

- (NSInteger)originGenerations
{
    return genotype->originGenerations();
}

- (NSString*)genotypeString
{
    return [NSString stringWithUTF8String:genotype->printableGenotype().c_str()];
}

@end
