//*******************************************************************
// Copyright Frank Luna (C) 2015 All Rights Reserved.
//
// Shadows.hlsl:
//
// Simple shader that renders the scene from the perspective of the 
// light and builds the shadow map.
//*******************************************************************

// Include common HLSL code.
#include "Common.hlsl"

struct VertexIn
{
    float3 PosL : POSITION;
    float2 TexC : TEXCOORD;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float2 TexC : TEXCOORD;
    
    // nointerpolation is used so the index is not interpolated across the
    // triangle.
    nointerpolation uint MatIndex : MATINDEX;
};

VertexOut VS(VertexIn vin, uint instanceID : SV_InstanceID)
{
    VertexOut vout = (VertexOut) 0.0f;
  
    // Fetch the instance data.
    // Direct3D provides the system value identifier SV_InstanceID to tell
    // which instance is being drawn in the vS.
    InstanceData instData = gInstanceData[instanceID];
    float4x4 world = instData.World;
    float4x4 texTransform = instData.TexTransform;
    uint matIndex = instData.MaterialIndex;
    
    vout.MatIndex = matIndex;
    
    // Fetch the material data.
    MaterialData matData = gMaterialData[matIndex];
	
    // Transform to world space.
    float4 posW = mul(float4(vin.PosL, 1.0f), world);

    // Transform to homogeneous clip space.
    vout.PosH = mul(posW, gViewProj);
	
	// Output vertex attributes for interpolation across triangle.
    float4 texC = mul(float4(vin.TexC, 0.0f, 1.0f), texTransform);
    vout.TexC = mul(texC, matData.MatTransform).xy;
	
    return vout;
}

// This is only used for alpha cut out geometry, so that shadows 
// show up correctly.  Geometry that does not need to sample a
// texture can use a NULL pixel shader for depth pass.
void PS(VertexOut pin)
{
	// Fetch the material data.
    MaterialData matData = gMaterialData[pin.MatIndex];
    float4 diffuseAlbedo = matData.DiffuseAlbedo;
    uint diffuseTexIndex = matData.DiffuseMapIndex;
	
	// Dynamically look up the texture in the array.
    diffuseAlbedo *= gTextureMaps[diffuseTexIndex].Sample(gsamAnisotropicWrap, pin.TexC);

#ifdef ALPHA_TEST
    // Discard pixel if texture alpha < 0.1.  We do this test as soon 
    // as possible in the shader so that we can potentially exit the
    // shader early, thereby skipping the rest of the shader code.
    clip(diffuseAlbedo.a - 0.1f);
#endif
}
