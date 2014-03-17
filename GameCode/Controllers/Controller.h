//
//  Controller.h
//  High Count
//
//  Created by Adam on 3/15/14.
//  Copyright (c) 2014 Adam. All rights reserved.
//

#include "HC.h"

namespace highcount
{
    class IController
    {
    public:
        virtual void Update(int deltaMillis) = 0;
        virtual void Render(IGraphicsDevice *device, RenderState &state, float contentScale) = 0;
        virtual void HandleTouchEvent(const TouchEvent &e) = 0;
        
        virtual ~IController() {}
    };
    
    class BaseController : public IController
    {
    public:
        BaseController(std::shared_ptr<HCGameInstance> game) :
            mGame(game)
        {
        }
        
        virtual ~BaseController()
        {
        }
        
    protected:
        std::shared_ptr<HCGameInstance> GetGameInstance() { return mGame; }
        
    private:
        std::shared_ptr<HCGameInstance> mGame;
    };
}
