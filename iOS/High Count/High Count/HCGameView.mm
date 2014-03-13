//
//  HCGameView.m
//  High Count
//
//  Created by Adam on 3/12/14.
//  Copyright (c) 2014 Adam. All rights reserved.
//

#import "HCGameView.h"
#import "HC.h"
#import "HCGameInstance.h"
using namespace highcount;

@interface HCGameView()

@property (assign, nonatomic) std::shared_ptr<HCGameInstance> instance;

@end

@implementation HCGameView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        self.backgroundColor = [UIColor redColor];
    }
    return self;
}

- (void)applicationInitialized:(CEApplication *)app size:(CGSize)size
{
    
}

@end
