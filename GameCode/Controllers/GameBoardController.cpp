//
//  GameBoardController.cpp
//  High Count
//
//  Created by Adam on 3/12/14.
//  Copyright (c) 2014 Adam. All rights reserved.
//

#include "GameBoardController.h"

namespace highcount
{
    GameBoardController::GameBoardController(std::shared_ptr<HCGameInstance> game) :
        BaseController(game),
        mGridBoard(citymaps::Size(4, 4))
    {
    }
    
    GameBoardController::~GameBoardController()
    {
    }
    
    void GameBoardController::Update(int deltaMillis)
    {
        mGridBoard.Update(deltaMillis);
    }
    
    void GameBoardController::Render(IGraphicsDevice *device, RenderState &state, float contentScale)
    {
        mGridBoard.Render(device, state, contentScale);
    }
    
    void GameBoardController::HandleTouchEvent(const TouchEvent &e)
    {
        switch (e.type) {
            case TouchEventStart:
            {
                mStartTouch = e.touchPoints[0];
                break;
            }
                
            case TouchEventEnd:
            {
                citymaps::Point dif = e.touchPoints[0] - mStartTouch;
                    
                ShiftDirection shiftDirection;
                
                if (abs(dif.x) > abs(dif.y)) {
                    if (dif.x > 0) {
                        shiftDirection = ShiftDirectionRight;
                    } else {
                        shiftDirection = ShiftDirectionLeft;
                    }
                } else {
                    if (dif.y > 0) {
                        shiftDirection = ShiftDirectionDown;
                    } else {
                        shiftDirection = ShiftDirectionUp;
                    }
                }
                
                mGridBoard.ShiftBoard(shiftDirection);
                break;
            }
                
            default:
                break;
        }
    }
}
