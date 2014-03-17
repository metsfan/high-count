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
#include <CitymapsEngine/Core/Graphics/Camera/OrthoCamera.h>

namespace highcount
{
    class IController;
    
    class HCGameInstance : public std::enable_shared_from_this<HCGameInstance>,
                           public IApplicationListener,
                           public ITouchListener
    {
    public:
        HCGameInstance(std::shared_ptr<IApplication> app, const citymaps::Size &windowSize);
        
        ~HCGameInstance()
        {
        }
        
        float GetContentScale() { return mContentScale; }
        
        void OnResize(int width, int height);
        void OnUpdate(IApplication* application, int deltaMS);
        void OnRender(IApplication* application, int deltaMS);
        
        void OnNetworkStatusChanged(IApplication* application, NetworkStatus status)
        {
        }
        
        void OnTouchEvent(const TouchEvent &e);
        
        void PushController(IController *controller);
        void PopController();
        
    private:
        std::shared_ptr<IApplication> mApp;
        citymaps::Size mWindowSize;
        std::unique_ptr<OrthoCamera> mCamera;
        
        std::stack<IController *> mControllers;
        IController *mTopController;
        
        float mContentScale;
        
    };
};