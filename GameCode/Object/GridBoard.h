//
//  GridBoard.h
//  High Count
//
//  Created by Adam on 3/12/14.
//  Copyright (c) 2014 Adam. All rights reserved.
//

#pragma once

#include "HC.h"
#include "Object/GridCell.h"

namespace highcount
{
    class GridBoard
    {
    public:
        GridBoard(const Size &size);
        ~GridBoard();
        
        GridCell& GetCell(const Point &position)
        {
            return this->GetCell(position.x, position.y);
        }
        
        GridCell& GetCell(int x, int y)
        {
            return mGrid[y * mSize.width + x];
        }
        
    private:
        std::vector<GridCell> mGrid;
        Size mSize;
    };
};