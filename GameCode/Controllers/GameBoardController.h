//
//  GameBoardController.h
//  High Count
//
//  Created by Adam on 3/12/14.
//  Copyright (c) 2014 Adam. All rights reserved.
//

#pragma once

#include "HC.h"
#include "HCGameInstance.h"
#include "Controllers/Controller.h"
#include "Object/GridBoard.h"

namespace highcount
{
    class GameBoardController : public BaseController
    {
    public:
        GameBoardController(std::shared_ptr<HCGameInstance> game);
        ~GameBoardController();
        
        void Update(int deltaMillis);
        void Render(IGraphicsDevice *device, RenderState &state, float contentScale);
        
        void HandleTouchEvent(const TouchEvent &e);
        
    private:
        GridBoard mGridBoard;
        
        citymaps::Point mStartTouch;
    };
};
