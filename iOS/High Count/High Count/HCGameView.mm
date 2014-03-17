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
#import "Controllers/GameBoardController.h"
#import <CitymapsEngine/Core/Platform/iOS/CEApplication.h>
using namespace highcount;
using namespace citymaps;

@interface HCGameView()

@property (assign, nonatomic) std::shared_ptr<HCGameInstance> instance;

@end

@implementation HCGameView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        self.backgroundColor = [UIColor colorWithRed:230/255.0f green:221/255.0f blue:195/255.0f alpha: 1.0];
    }
    return self;
}

- (void)applicationInitialized:(CEApplication *)app size:(CGSize)size
{
    _instance = std::shared_ptr<HCGameInstance>(new HCGameInstance(app.appInstance, citymaps::Size(size.width, size.height)));
    app.appInstance->SetListener(_instance);
    app.appInstance->AddTouchListener(_instance);
    
    app.appInstance->RunMainLoop(true);
    
    GameBoardController *gameBoard = new GameBoardController(_instance);
    _instance->PushController(gameBoard);
}

@end
