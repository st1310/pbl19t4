#define MaxBones 60

/************* Resources *************/

cbuffer CBufferPerObject
{
    float4x4 WorldViewProjection : WORLDVIEWPROJECTION;
}

cbuffer CBufferSkinning
{
    float4x4 BoneTransforms[MaxBones];
}

Texture2D ColorTexture;

SamplerState ColorSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

/************* Data Structures *************/

struct VS_INPUT
{
    float4 ObjectPosition : POSITION;
    float2 TextureCoordinate : TEXCOORD;
    uint4 BoneIndices : BONEINDICES;
    float4 BoneWeights : WEIGHTS;	
};

struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float2 TextureCoordinate : TEXCOORD0;
};

/************* Vertex Shader *************/

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;      

    float4x4 skinTransform = (float4x4)0;
    skinTransform += BoneTransforms[IN.BoneIndices.x] * IN.BoneWeights.x;
    skinTransform += BoneTransforms[IN.BoneIndices.y] * IN.BoneWeights.y;
    skinTransform += BoneTransforms[IN.BoneIndices.z] * IN.BoneWeights.z;
    skinTransform += BoneTransforms[IN.BoneIndices.w] * IN.BoneWeights.w;
    
    float4 position = mul(IN.ObjectPosition, skinTransform);	
    OUT.Position = mul(position, WorldViewProjection);
    
    OUT.TextureCoordinate = IN.TextureCoordinate;

    return OUT;
}

/************* Pixel Shaders *************/

float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
    return ColorTexture.Sample(ColorSampler, IN.TextureCoordinate);
}

/************* Techniques *************/

technique11 main11
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, pixel_shader()));
    }
}