//
//  HCGameInstance.h
//  High Count
//
//  Created by Adam on 3/12/14.
//  Copyright (c) 2014 Adam. All rights reserved.
//

#pragma once

#include "HC.h"

#include <CitymapsEngine/Core/Application.h>

namespace highcount
{
    class HCGameInstance : public std::enable_shared_from_this<HCGameInstance>
    {
    public:
        HCGameInstance(std::shared_ptr<IApplication> app) :
            mApp(app)
        {
            
        }
        
        ~HCGameInstance()
        {
            
        }
        
    private:
        std::shared_ptr<IApplication> mApp;
    };
};