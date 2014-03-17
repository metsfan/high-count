//
//  GridCellAnimation.cpp
//  High Count
//
//  Created by Adam on 3/16/14.
//  Copyright (c) 2014 Adam. All rights reserved.
//

#include "LinearAnimation.h"

namespace highcount
{
    LinearAnimation::LinearAnimation() :
        mAnimating(false),
        mListener(NULL)
    {
    }
    
    LinearAnimation::~LinearAnimation()
    {
    }
    
    void LinearAnimation::Animate(const citymaps::Point &start, const citymaps::Point &end, float duration)
    {
        mStart = start;
        mEnd = end;
        mDuration = mRemaining = duration * 1000;
        mCurrentPoint = mStart;
        
        printf("Start: %f, %f\n", mStart.x, mStart.y);
        printf("End: %f, %f\n", mEnd.x, mEnd.y);
        
        mAnimating = true;
        
        if (mListener) {
            mListener->AnimationStart();
        }
    }
    
    void LinearAnimation::Update(int deltaMillis)
    {
        if (mAnimating) {
            if (mRemaining - deltaMillis < 0) {
                mAnimating = false;
                
                if (mListener) {
                    mListener->AnimationStep(mEnd);
                    mListener->AnimationEnd();
                }
                
                return;
            }
            
            mRemaining -= deltaMillis;
            
            double percent = 1.0 - ((double)mRemaining / (double)mDuration);
            printf("Percent: %f\n", percent);
            mCurrentPoint = mStart + ((mEnd - mStart) * percent);
            
            if (mListener) {
                //printf("Step: %f, %f\n", mCurrentPoint.x, mCurrentPoint.y);
                mListener->AnimationStep(mCurrentPoint);
            }
        }
    }
}