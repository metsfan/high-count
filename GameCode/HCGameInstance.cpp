//
//  HCGameInstance.cpp
//  High Count
//
//  Created by Adam on 3/12/14.
//  Copyright (c) 2014 Adam. All rights reserved.
//

#include "HCGameInstance.h"
#include "Controllers/Controller.h"

namespace highcount
{
    static const int kWindowWidth = 400;
    
    HCGameInstance::HCGameInstance(std::shared_ptr<IApplication> app, const citymaps::Size &windowSize) :
        mApp(app),
        mTopController(NULL),
        mCamera(nullptr)
    {
        Logger::Log("Creating Instance");
        
        float aspectRatio = windowSize.height / windowSize.width;
        mContentScale = windowSize.width / kWindowWidth;
        mWindowSize.width = windowSize.width;
        mWindowSize.height = windowSize.height;
        
        this->OnResize(mWindowSize.width, mWindowSize.height);
        
        mApp->LoadShaderConfig("shaders.xml");
        mApp->LoadShaderConfig("hc_shaders.xml");
        
        mApp->LoadEffectsConfig("effects.xml");
        mApp->LoadEffectsConfig("hc_effects.xml");
    }
    
    void HCGameInstance::PushController(IController *controller)
    {
        mControllers.push(controller);
        mTopController = mControllers.top();
        
        mApp->Invalidate();
    }
    
    void HCGameInstance::PopController()
    {
        mControllers.pop();
        
        delete mTopController;
        
        mTopController = mControllers.top();
        
        mApp->Invalidate();
    }
    
    void HCGameInstance::OnResize(int width, int height)
    {
        mWindowSize.width = width;
        mWindowSize.height = height;
        
        mCamera = std::unique_ptr<OrthoCamera>(new OrthoCamera(0, width, height, 0, -1, 1, mWindowSize));
    }
    
    void HCGameInstance::OnUpdate(IApplication* application, int deltaMS)
    {
        if (mTopController) {
            mTopController->Update(deltaMS);
        }
        
        mApp->Invalidate();
    }
    
    void HCGameInstance::OnRender(IApplication* application, int deltaMS)
    {
        if (mTopController) {
            RenderState state;
            
            state.SetProjection(mCamera->GetProjectionMatrix());
            state.PushTransform(mCamera->GetViewMatrix());
            
            mTopController->Render(application->GetGraphicsDevice(), state, mContentScale);
            
            state.PopTransform();
        }
        
        mApp->Invalidate();
    }
    
    void HCGameInstance::OnTouchEvent(const TouchEvent &e)
    {
        if (mTopController) {
            mTopController->HandleTouchEvent(e);
        }
    }
}