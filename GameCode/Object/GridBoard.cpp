//
//  GridBoard.cpp
//  High Count
//
//  Created by Adam on 3/12/14.
//  Copyright (c) 2014 Adam. All rights reserved.
//

#include "GridBoard.h"
#include "Render/RoundRectRenderer.h"

#include <random>

namespace highcount
{
    static const int kInitialSquares = 1;
    
    GridBoard::GridBoard(const Size &size) :
        mSize(size),
        mPosition(50, 50),
        mListener(NULL)
    {
        for (int y = 0; y < size.width; y++) {
            for (int x = 0; x < size.height; x++) {
                GridCell cell(x, y);
                cell.SetScreenOffset(mPosition);
                mGrid.push_back(cell);
            }
        }
        
        mBoardRenderSize.width = (size.width * kGridCellSize) + ((size.width + 1) * kGridCellPadding);
        mBoardRenderSize.height = (size.height * kGridCellSize) + ((size.height + 1) * kGridCellPadding);
        
        for (int i = 0; i < kInitialSquares; i++) {
            this->AddSquare();
        }
    }
    
    GridBoard::~GridBoard()
    {
    }
    
    void GridBoard::Update(int deltaMillis)
    {
        mAnimating = false;
        
        for (GridCell &cell : mGrid) {
            cell.Update(deltaMillis);
            
            mAnimating = mAnimating || cell.IsAnimating();
        }
        
        if (!mAnimating && mPendingNumbers.size() > 0) {
            for (int i = 0; i < mPendingNumbers.size(); i++) {
                mGrid[i].SetNumber(mPendingNumbers[i]);
            }
            
            mPendingNumbers.clear();
            
            this->AddSquare();
        }
    }
    
    void GridBoard::Render(IGraphicsDevice *device, RenderState &state, float contentScale)
    {
        ROUND_RECT_DESC rectDesc;
        rectDesc.Color = Vector4f(0, 0, 0, 1);
        rectDesc.Dimensions = mBoardRenderSize * contentScale;
        rectDesc.Position = citymaps::Point(50 * contentScale, 50 * contentScale);
        
        RoundRectRenderer::DrawRect(device, state, rectDesc);
        
        for (GridCell &cell : mGrid) {
            cell.RenderBackground(device, state, contentScale);
        }
        
        for (GridCell &cell : mGrid) {
            cell.RenderNumber(device, state, contentScale);
        }
    }
    
    void GridBoard::ShiftBoard(ShiftDirection direction)
    {
        std::vector<std::vector<GridCell *>> cellSets;
        
        switch (direction) {
            case ShiftDirectionDown:
                Logger::Log("Shift Down");
                cellSets.resize(mSize.width);
                
                for (int x = 0; x < mSize.width; x++) {
                    for (int y = mSize.height - 1; y >= 0; y--) {
                        cellSets[x].push_back(&this->GetCell(x, y));
                    }
                }
                
                break;
                
            case ShiftDirectionUp:
                Logger::Log("Shift Up");
                cellSets.resize(mSize.width);
                
                for (int x = 0; x < mSize.width; x++) {
                    for (int y = 0; y < mSize.height; y++) {
                        cellSets[x].push_back(&this->GetCell(x, y));
                    }
                }
                
                break;
                
            case ShiftDirectionLeft:
                Logger::Log("Shift Left");
                cellSets.resize(mSize.height);
                
                for (int y = 0; y < mSize.height; y++) {
                    for (int x = 0; x < mSize.width; x++) {
                        cellSets[y].push_back(&this->GetCell(x, y));
                    }
                }
                
                break;
                
            case ShiftDirectionRight:
                Logger::Log("Shift Right");
                cellSets.resize(mSize.height);
                
                for (int y = 0; y < mSize.height; y++) {
                    for (int x = mSize.width - 1; x >= 0; x--) {
                        cellSets[y].push_back(&this->GetCell(x, y));
                    }
                }
                
                break;
                
            default:
                break;
        }
        
        bool moveValid = false;
        for (int i = 0; i < cellSets.size(); i++) {
            moveValid = moveValid || this->ValidateSet(cellSets[i]);
        }
        
        if (moveValid) {
            mPendingNumbers.resize(mGrid.size());
            for (int i = 0; i < mGrid.size(); i++) {
                mPendingNumbers[i] = mGrid[i].GetNumber();
            }
            
            for (int i = 0; i < cellSets.size(); i++) {
                this->ResolveSet(cellSets[i]);
            }
            
        }
        
    }
    
    bool GridBoard::ValidateSet(const std::vector<GridCell *> &set)
    {
        for (int i = 0; i < set.size() - 1; i++) {
            if (!set[i]->IsEmpty()) {
                for (int j = i + 1; j < set.size(); j++) {
                    if (set[i]->HasSameNumber(*set[j])) {
                        return true;
                    }
                }
            }
        }
        
        bool hasEmpty = false;
        
        for (int i = 0; i < set.size(); i++) {
            if (hasEmpty && !set[i]->IsEmpty()) {
                return true;
            }
            
            if (set[i]->IsEmpty()) {
                hasEmpty = true;
            }
        }
        
        return false;
    }
    
    void GridBoard::ResolveSet(const std::vector<GridCell *> &set)
    {
        int cursor = 0;
        
        
        for (int i = 0; i < set.size() - 1; i++) {
            int indexI = (set[i]->GetPosition().y * mSize.width) + set[i]->GetPosition().x;
            if (mPendingNumbers[indexI] > 0) {
                for (int j = i + 1; j < set.size(); j++) {
                    if (set[i]->HasSameNumber(*set[j])) {
                        
                        int indexJ = (set[j]->GetPosition().y * mSize.width) + set[j]->GetPosition().x;
                        
                        mPendingNumbers[indexI] = mPendingNumbers[indexI] * 2;
                        mPendingNumbers[indexJ] = -1;
                        set[j]->AnimateTo(set[i]->GetPosition());
                        i = j;
                        break;
                    }
                }
            }
        }
        
        for (int i = 0; i < set.size(); i++) {
            int indexI = (set[i]->GetPosition().y * mSize.width) + set[i]->GetPosition().x;
            if (mPendingNumbers[indexI] > 0) {
                int indexCursor = (set[cursor]->GetPosition().y * mSize.width) + set[cursor]->GetPosition().x;
                if (mPendingNumbers[indexCursor] < 0) {
                    mPendingNumbers[indexCursor] = mPendingNumbers[indexI];
                    mPendingNumbers[indexI] = -1;
                    set[i]->AnimateTo(set[cursor]->GetPosition());
                }
                
                cursor++;
            }
        }
    }
    
    void GridBoard::AddSquare()
    {
        std::vector<GridCell *> emptyCells;
        
        for (auto &cell : mGrid) {
            if (cell.IsEmpty()) {
                emptyCells.push_back(&cell);
            }
        }
        
        if (emptyCells.size() == 0) {
            // Game Over
        } else {
            std::default_random_engine generator;
            generator.seed(time(NULL));
            std::uniform_int_distribution<int> distribution(0, emptyCells.size() - 1);
            int cell = distribution(generator);
            
            emptyCells[cell]->SetNumber(2);
        }
    }
}