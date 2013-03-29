//
//  NBUMockHelper.m
//  NBUKit
//
//  Created by 利辺羅 on 2012/08/09.
//  Copyright (c) 2012年 CyberAgent Inc. All rights reserved.
//

#import "NBUMockHelper.h"
#import <CoreData/CoreData.h>
#import "LoremIpsum.h"

// Define module
#undef  NBUKIT_MODULE
#define NBUKIT_MODULE   NBUKIT_MODULE_HELPERS

static NBUMockHelper * _sharedHelper = nil;

@implementation NBUMockHelper
{
    NSMutableDictionary * _classBlocks, * _entityBlocks;
}
@synthesize managedObjectContext = _managedObjectContext;

+ (NBUMockHelper *)sharedHelper
{
    if (!_sharedHelper)
    {
        [self new];
    }
    return _sharedHelper;
}

+ (void)setSharedHelper:(NBUMockHelper *)helper
{
    _sharedHelper = helper;
}

- (id)init
{
    self = [super init];
    if (self)
    {
        _classBlocks = [NSMutableDictionary dictionary];
        _entityBlocks = [NSMutableDictionary dictionary];
        
        if (!_sharedHelper)
        {
            _sharedHelper = self;
        }
    }
    return self;
}

#pragma mark - Methods

- (void)setConfigurationBlock:(NBUMockObjectConfigurationBlock)block
                forClassNamed:(NSString *)name
{
    [_classBlocks setValue:block
                    forKey:name];
}

- (void)setConfigurationBlock:(NBUMockObjectConfigurationBlock)block
               forEntityNamed:(NSString *)name
{
    [_entityBlocks setValue:block
                     forKey:name];
}

- (id)generateMockObject:(NSString *)nameOfClassOrEntity
{
    id object;
    
    // Check for configuration blocks
    NBUMockObjectConfigurationBlock classBlock, entityBlock;
    classBlock = [_classBlocks valueForKey:nameOfClassOrEntity];
    if (!classBlock)
    {
        entityBlock = [_entityBlocks valueForKey:nameOfClassOrEntity];
    }
    
    // Create a new object from a class
    if (!entityBlock)
    {
        object = [NSClassFromString(nameOfClassOrEntity) new];
        
        // And configure it
        if (classBlock)
        {
            classBlock(object, 0);
        }
    }
    
    // Or from an entity
    else
    {
        object = [[NSManagedObject alloc] initWithEntity:[NSEntityDescription entityForName:nameOfClassOrEntity
                                                                     inManagedObjectContext:_managedObjectContext]
                          insertIntoManagedObjectContext:_managedObjectContext];
        
        // And configure it
        entityBlock(object, 0);
    }
    
    return object;
}

- (NSArray *)generate:(NSUInteger)count
          mockObjects:(NSString *)nameOfClassOrEntity
{
    NSMutableArray * objects = [NSMutableArray array];
    for (NSUInteger i = 0; i < count; i++)
    {
        [objects addObject:[self generateMockObject:nameOfClassOrEntity]];
    }
    return objects;
}

- (NSArray *)generateFrom:(NSUInteger)minCount
                       to:(NSUInteger)maxCount
              mockObjects:(NSString *)nameOfClassOrEntity
{
    return [self generate:minCount + (arc4random() % (maxCount + 1 - minCount))
              mockObjects:nameOfClassOrEntity];
}

#pragma mark - Utilities

+ (NSString *)mockTextWithLenght:(NSUInteger)length
{
    NSString * lorem = [[LoremIpsum new] words:length / 4];
    return [lorem substringWithRange:NSMakeRange(0, length)]; // TODO: Check better the length!
}

@end

