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
    enum ShiftDirection
    {
        ShiftDirectionLeft,
        ShiftDirectionRight,
        ShiftDirectionUp,
        ShiftDirectionDown
    };
    
    class IGridBoardListener
    {
        virtual void OnScoreChange(int newScore) = 0;
        virtual void OnGameOver() = 0;
    };
    
    class GridBoard
    {
    public:
        GridBoard(const citymaps::Size &size);
        ~GridBoard();
        
        void SetListener(IGridBoardListener *listener)
        {
            mListener = listener;
        }
        
        GridCell& GetCell(const citymaps::Point &position)
        {
            return this->GetCell(position.x, position.y);
        }
        
        GridCell& GetCell(int x, int y)
        {
            return mGrid[y * mSize.width + x];
        }
        
        int GetIndex(const citymaps::Point &position)
        {
            return position.y * mSize.width + position.x;
        }
        
        void Update(int deltaMillis);
        void Render(IGraphicsDevice *device, RenderState &state, float contentScale);
        
        void ShiftBoard(ShiftDirection direction);
        
    private:
        std::vector<GridCell> mGrid;
        citymaps::Size mSize;
        citymaps::Point mPosition;
        IGridBoardListener *mListener;
        bool mAnimating;
        std::vector<int> mPendingNumbers;
        
        citymaps::Size mBoardRenderSize;
        
        void AddSquare();
        bool ValidateSet(const std::vector<GridCell *> &set);
        void ResolveSet(const std::vector<GridCell *> &set);
    };
};