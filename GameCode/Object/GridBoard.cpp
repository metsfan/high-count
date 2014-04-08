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
        
        this->GetCell(0, 0).SetNumber(2);
        this->GetCell(0, 1).SetNumber(2);
        this->GetCell(0, 2).SetNumber(2);
        this->GetCell(0, 3).SetNumber(2);
        
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
        
        std::vector<GridAnimFrame> newGrid(16);
        for (int i = 0; i < mGrid.size(); i++) {
            newGrid[i].newNumber = mGrid[i].GetNumber();
            newGrid[i].position = newGrid[i].newPosition = mGrid[i].GetPosition();
        }
        
        if (moveValid) {
            mPendingNumbers.resize(mGrid.size());
            for (int i = 0; i < mGrid.size(); i++) {
                mPendingNumbers[i] = mGrid[i].GetNumber();
            }
            
            for (int i = 0; i < cellSets.size(); i++) {
                this->ResolveSet(cellSets[i], newGrid);
            }
            
            for (int i = 0; i < newGrid.size(); i++) {
                mGrid[i].AnimateTo(newGrid[i].newPosition);
                mPendingNumbers[i] = newGrid[i].newNumber;
                //mGrid[i].SetNumber(newGrid[i].newNumber);
            }
            
            //this->AddSquare();
        }
        
    }
    
    bool GridBoard::ValidateSet(const std::vector<GridCell *> &set)
    {
        for (int i = 0; i < set.size() - 1; i++) {
            if (!set[i]->IsEmpty()) {
                for (int j = i + 1; j < set.size(); j++) {
                    if (!set[j]->IsEmpty()) {
                        if (set[i]->HasSameNumber(*set[j])) {
                            return true;
                        }
                        
                        break;
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
    
    void GridBoard::ResolveSet(const std::vector<GridCell *> &set, std::vector<GridAnimFrame> &newGrid)
    {
        std::vector<int32_t> newNumbers(4);
        
        int cursor = 0;
        for (int i = 0; i < 4; i++) {
            if (!set[i]->IsEmpty()) {
                int index = this->GetIndex(set[i]->GetPosition());
                newNumbers[cursor] = set[i]->GetNumber();
                
                newGrid[index].newPosition = set[cursor]->GetPosition();
                
                
                cursor++;
            }
        }
        
        int lastNum = -1;
        for (int i = 0; i < 4; i++) {
            if (!newNumbers[i] || newNumbers[i] < 0) {
                continue;
            }
            
            if (lastNum > 0) {
                if (newNumbers[i] == newNumbers[i-1]) {
                    newNumbers[i-1] *= 2;
                    
                    int index = this->GetIndex(set[i]->GetPosition());
                    newGrid[index].newPosition = set[i-1]->GetPosition();
                    
                    for (int j = i+1; j < 4; j++) {
                        newNumbers[j-1] = newNumbers[j];
                        
                        int index = this->GetIndex(set[j]->GetPosition());
                        newGrid[index].newPosition = set[j-1]->GetPosition();
                    }
                    newNumbers[3] = 0;
                    lastNum = -1;
                    i--;
                } else {
                    lastNum = newNumbers[i];
                }
            } else {
                lastNum = newNumbers[i];
            }
        }
        
        for (int i = 0; i < 4; i++) {
            int index = this->GetIndex(set[i]->GetPosition());
            newGrid[index].newNumber = newNumbers[i];
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