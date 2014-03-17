//
//  RoundRectRenderer.cpp
//  High Count
//
//  Created by Adam on 3/16/14.
//  Copyright (c) 2014 Adam. All rights reserved.
//

#include "RoundRectRenderer.h"

namespace highcount
{
    MeshShape *RoundRectRenderer::msRenderShape = NULL;
    
    int RoundRectRenderer::msColorUniform;
    int RoundRectRenderer::msSizeUniform;
    int RoundRectRenderer::msCornerRadiusUniform;
    
    void RoundRectRenderer::DrawRect(IGraphicsDevice *device, RenderState &state, ROUND_RECT_DESC &desc)
    {
        if (!msRenderShape) {
            float data[] = {
                0, 0,
                1, 0,
                0, 1,
                1, 1
            };
            auto effect = device->GetContext()->GetEffect("round_rect");
            msRenderShape = new MeshShape(device, "round_rect");
            msRenderShape->UpdateData(data, sizeof(data), 4);
            msColorUniform = device->GetContext()->GetGlobalDataIndex("u_color");
            msSizeUniform = device->GetContext()->GetGlobalDataIndex("u_size");
            msCornerRadiusUniform = device->GetContext()->GetGlobalDataIndex("u_cornerRadius");
            
            
        }
        
        ShaderDataVector4f colorData(desc.Color);
        state.SetShaderData(msColorUniform, &colorData);
        
        ShaderDataVector2f sizeData(Vector2f(desc.Dimensions.width, desc.Dimensions.height));
        state.SetShaderData(msSizeUniform, &sizeData);
        
        ShaderDataFloat cornerRadiusData(desc.CornerRadius);
        state.SetShaderData(msCornerRadiusUniform, &cornerRadiusData);
        
        Matrix4 viewMatrix;
        viewMatrix = glm::translate(viewMatrix, Vector3(desc.Position.x, desc.Position.y, 0));
        viewMatrix = glm::scale(viewMatrix, Vector3(desc.Dimensions.width, desc.Dimensions.height, 1.0f));
        
        state.PushTransform(viewMatrix);
        msRenderShape->Draw(device, state, PrimitiveTypeTriangleStrip);
        state.PopTransform();
    }
}