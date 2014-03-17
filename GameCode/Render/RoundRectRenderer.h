//
//  RoundRectRenderer.h
//  High Count
//
//  Created by Adam on 3/16/14.
//  Copyright (c) 2014 Adam. All rights reserved.
//

#pragma once

#include "HC.h"

namespace highcount
{
    struct ROUND_RECT_DESC
    {
        Vector4f Color;
        citymaps::Point Position;
        citymaps::Size Dimensions;
        float CornerRadius = 0;
    };
    
    class RoundRectRenderer
    {
    public:
        static void DrawRect(IGraphicsDevice *device, RenderState &state, ROUND_RECT_DESC &desc);
        
    private:
        static MeshShape *msRenderShape;
        
        static int msColorUniform;
        static int msSizeUniform;
        static int msCornerRadiusUniform;
    };
}
