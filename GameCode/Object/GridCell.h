//
//  GridCell.h
//  High Count
//
//  Created by Adam on 3/12/14.
//  Copyright (c) 2014 Adam. All rights reserved.
//

#pragma once

#include "HC.h"

namespace highcount
{
    class GridCell
    {
    public:
        GridCell(const Point &position) :
            mPosition(position),
            mNumber(-1)
        {
        }
        
        ~GridCell()
        {
        }
    
        void SetNumber(int number)
        {
            mNumber = number;
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
            return mNumber > 0;
        }
        
    private:
        Point mPosition;
        int mNumber;
    };
}
