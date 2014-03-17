//
//  GridCell.cpp
//  High Count
//
//  Created by Adam on 3/12/14.
//  Copyright (c) 2014 Adam. All rights reserved.
//

#include "GridCell.h"
#include "Render/RoundRectRenderer.h"

#include <CitymapsEngine/Core/Util/UnicodeUtil.h>

namespace highcount
{
    std::map<int, Vector4f> GridCell::msColorMap;
    
    GridCell::GridCell(int x, int y) :
        mPosition(x, y),
        mNumber(-1),
        mLabel(NULL),
        mNumberChanged(false),
        mScreenPosition(-1, -1)
    {
        if (msColorMap.size() == 0) {
            msColorMap[-1] = Vector4f(0.5, 0.5, 0.5, 1);
            msColorMap[2] = Vector4f(0.1, 0.1, 0.1, 1);
            msColorMap[4] = Vector4f(0.2, 0.2, 0.2, 1);
            msColorMap[8] = Vector4f(0.3, 0.3, 0.3, 1);
            msColorMap[16] = Vector4f(0.4, 0.4, 0.4, 1);
            msColorMap[32] = Vector4f(0.5, 0.5, 0.5, 1);
            msColorMap[64] = Vector4f(0.6, 0.6, 0.6, 1);
            msColorMap[128] = Vector4f(0.7, 0.7, 0.7, 1);
            msColorMap[256] = Vector4f(0.7, 0.7, 0.7, 1);
            msColorMap[512] = Vector4f(0.7, 0.7, 0.7, 1);
            msColorMap[1024] = Vector4f(0.7, 0.7, 0.7, 1);
            msColorMap[2048] = Vector4f(0.7, 0.7, 0.7, 1);
            msColorMap[INFINITY] = Vector4f(0.7, 0.7, 0.7, 1);
        }
    }
    
    void GridCell::Update(int deltaMillis)
    {
        if (mScreenPosition.x < 0 && mScreenPosition.y < 0) {
            mScreenPosition = GridCell::CalculateScreenPosition(mPosition, mOffset);
        }
        
        if (mAnimator.IsAnimating()) {
            mAnimator.Update(deltaMillis);
        } else {
            mNumberScreenPosition = mScreenPosition;
        }
    }
    
    void GridCell::RenderBackground(IGraphicsDevice *device, RenderState &state, float contentScale)
    {
        ROUND_RECT_DESC rectDesc;
        rectDesc.Color = msColorMap[-1];
        rectDesc.Dimensions.width = kGridCellSize * contentScale;
        rectDesc.Dimensions.height = kGridCellSize * contentScale;
        rectDesc.Position = mScreenPosition * contentScale;
        
        RoundRectRenderer::DrawRect(device, state, rectDesc);
    }
    
    void GridCell::RenderNumber(IGraphicsDevice *device, RenderState &state, float contentScale)
    {
        if (!mLabel) {
            mLabel = new Label(device);
            
            mLabel->SetFont(FontManager::GetFont("_system", 36));
            mLabel->SetTextColor(0, 1, 0, 1);
            mLabel->SetAlignment(TextAlignCenter, TextAlignMiddle);
        }
        
        if (mNumber > 0) {
            ROUND_RECT_DESC rectDesc;
            rectDesc.Color = msColorMap[-1];
            rectDesc.Dimensions.width = kGridCellSize * contentScale;
            rectDesc.Dimensions.height = kGridCellSize * contentScale;
            rectDesc.Position = mNumberScreenPosition * contentScale;
            rectDesc.Color = mNumber <= 2048 ? msColorMap[mNumber] : msColorMap[INFINITY];
            RoundRectRenderer::DrawRect(device, state, rectDesc);
            
            if (mNumberChanged && !mAnimator.IsAnimating()) {
                mLabel->SetText(UnicodeUtil::ToUTF16(std::to_string(mNumber)));
            }
            
            mLabel->SetPosition(rectDesc.Position.x + kGridCellSize * 0.5 * contentScale, rectDesc.Position.y + kGridCellSize * 0.5 * contentScale);
            //mLabel->SetSize(rectDesc.Dimensions.width, rectDesc.Dimensions.height);
            
            mLabel->Draw(device, state);
        }
    }
    
    void GridCell::AnimateTo(const citymaps::Point &end, float duration)
    {
        mAnimator.SetListener(this);
        
        citymaps::Point startPos = GridCell::CalculateScreenPosition(mPosition, mOffset);
        citymaps::Point endPos = GridCell::CalculateScreenPosition(end, mOffset);
        
        mAnimator.Animate(startPos, endPos, duration);
    }

    /* ILinearAnimationListener methods */
    
    void GridCell::AnimationStart()
    {
    }
    
    void GridCell::AnimationStep(const citymaps::Point &position)
    {
        mNumberScreenPosition = position;
    }
    
    void GridCell::AnimationEnd()
    {
        //if (mPendingNumber > 0) {
        mNumber = mPendingNumber;
        mPendingNumber = -1;
        //}
    }
}