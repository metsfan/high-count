//
//  GridCellAnimation.h
//  High Count
//
//  Created by Adam on 3/16/14.
//  Copyright (c) 2014 Adam. All rights reserved.
//

#pragma once

#include "HC.h"

namespace highcount
{
    class ILinearAnimationListener
    {
    public:
        virtual void AnimationStart() = 0;
        virtual void AnimationStep(const citymaps::Point &position) = 0;
        virtual void AnimationEnd() = 0;
    };
    
    class LinearAnimation
    {
    public:
        LinearAnimation();
        ~LinearAnimation();
        
        void SetListener(ILinearAnimationListener *listener)
        {
            mListener = listener;
        }
    
        void Animate(const citymaps::Point &start, const citymaps::Point &end, float duration);
        void Update(int deltaMillis);
        
        bool IsAnimating() { return mAnimating; }
        
    private:
        citymaps::Point mStart;
        citymaps::Point mEnd;
        citymaps::Point mCurrentPoint;
        
        int mCurrentFrame;
        int mTotalFrames;
        int mDuration;
        int mRemaining;
        
        bool mAnimating;
        
        ILinearAnimationListener *mListener;
    };
};
