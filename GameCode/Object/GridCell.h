//
//  GridCell.h
//  High Count
//
//  Created by Adam on 3/12/14.
//  Copyright (c) 2014 Adam. All rights reserved.
//

#pragma once

#include "HC.h"
#include "Animation/LinearAnimation.h"

#include <CitymapsEngine/Core/Graphics/Shape/Label.h>

namespace highcount
{
    static const int kGridCellSize = 65;
    static const int kGridCellPadding = 10;
    
    struct GRID_CELL_ANIM_DESC
    {
        citymaps::Point endCell;
        int newNumber;
    };
    
    class GridCell : public ILinearAnimationListener
    {
    public:
        GridCell(int x, int y);
        ~GridCell()
        {
            delete mLabel;
        }
        
        const citymaps::Point& GetPosition() const { return mPosition; }
        
        void SetScreenOffset(const citymaps::Point &offset)
        {
            mOffset = offset;
        }
    
        void SetNumber(int number)
        {
            if (number != mNumber) {
                mNumber = number;
                mNumberChanged = true;
            }
        }
        
        int GetNumber()
        {
            return mNumber;
        }
        
        void DoubleNumber()
        {
            this->SetNumber(mNumber * 2);
        }
        
        bool HasSameNumber(const GridCell &other) const
        {
            return other.mNumber == mNumber;
        }
        
        bool IsAdjacent(const GridCell &other) const
        {
            return abs(mPosition.x - other.mPosition.x) == 1 ||
            abs(mPosition.y - other.mPosition.y) == 1;
        }
        
        bool IsEmpty() const
        {
            return mNumber < 0;
        }
        
        void SetPendingNumber(int newNumber)
        {
            mPendingNumber = newNumber;
        }
        
        void Update(int deltaMillis);
        void RenderBackground(IGraphicsDevice *device, RenderState &state, float contentScale);
        void RenderNumber(IGraphicsDevice *device, RenderState &state, float contentScale);
        
        void AnimateTo(const citymaps::Point &end, float duration = 0.1f);
    
        bool IsAnimating() { return mAnimator.IsAnimating(); }
        
        /* ILinearAnimationListener */
        void AnimationStart();
        void AnimationStep(const citymaps::Point &position);
        void AnimationEnd();
        
    private:
        citymaps::Point mPosition;
        citymaps::Point mOffset;
        int mNumber;
        Label *mLabel;
        bool mNumberChanged;
        
        int mPendingNumber;
        LinearAnimation mAnimator;
        
        citymaps::Point mScreenPosition;
        citymaps::Point mNumberScreenPosition;
        
        static std::map<int, Vector4f> msColorMap;
        
        static citymaps::Point CalculateScreenPosition(const citymaps::Point &position, const citymaps::Point &offset)
        {
            citymaps::Point point;
            int cellSize = kGridCellSize + kGridCellPadding;
            point.x = (((cellSize * position.x) + kGridCellPadding) + offset.x);
            point.y = (((cellSize * position.y) + kGridCellPadding) + offset.y);
            
            return point;
        }
    };
}
