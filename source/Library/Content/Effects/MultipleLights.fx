#include "include\\Common.fxh"

/************* Resources *************/

#define NUM_LIGHTS 5

cbuffer CBufferPerFrame
{
    POINT_LIGHT PointLights[NUM_LIGHTS];
    
    float4 AmbientColor : AMBIENT = {1.0f, 1.0f, 1.0f, 0.0f};
    
    float3 CameraPosition : CAMERAPOSITION;
}

cbuffer CBufferPerObject
{
    float4x4 WorldViewProjection : WORLDVIEWPROJECTION;
    float4x4 World : WORLD;
    
    float4 SpecularColor : SPECULAR = {1.0f, 1.0f, 1.0f, 1.0f};

    float SpecularPower : SPECULARPOWER = {25.0f};
}

Texture2D ColorTexture;

SamplerState ColorSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

RasterizerState DisableCulling
{
    CullMode = NONE;
};

/************* Data Structures *************/

struct VS_INPUT
{
    float4 ObjectPosition : POSITION;
    float2 TextureCoordinate : TEXCOORD;
    float3 Normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float3 WorldPosition : POSITION;
    float3 Normal : NORMAL;
    float2 TextureCoordinate : TEXCOORD0;
};

/************* Vertex Shader *************/

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;
    
    OUT.Position = mul(IN.ObjectPosition, WorldViewProjection);
    OUT.WorldPosition = mul(IN.ObjectPosition, World).xyz;		
    OUT.TextureCoordinate = IN.TextureCoordinate;
    OUT.Normal = normalize(mul(float4(IN.Normal, 0), World).xyz);

    return OUT;
}

/************* Pixel Shader *************/

float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 OUT = (float4)0;
        
    float3 normal = normalize(IN.Normal);
    float3 viewDirection = normalize(CameraPosition - IN.WorldPosition);	
    float4 color = ColorTexture.Sample(ColorSampler, IN.TextureCoordinate);
    float3 ambient = get_vector_color_contribution(AmbientColor, color.rgb);
        
    LIGHT_CONTRIBUTION_DATA lightContributionData;
    lightContributionData.Color = color;
    lightContributionData.Normal = normal;
    lightContributionData.ViewDirection = viewDirection;
    lightContributionData.SpecularColor = SpecularColor;
    lightContributionData.SpecularPower = SpecularPower;
    
    float3 totalLightContribution = (float3)0;
    
    for (int i = 0; i < NUM_LIGHTS && PointLights[i].Enabled; i++)
    {
        lightContributionData.LightDirection = get_light_data(PointLights[i].Position, IN.WorldPosition, PointLights[i].LightRadius);	
        lightContributionData.LightColor = PointLights[i].Color;
        totalLightContribution += get_light_contribution(lightContributionData);
    }

    OUT.rgb = ambient + totalLightContribution;
    OUT.a = 1.0f;
        
    return OUT;
}

/************* Techniques *************/

technique11 main11
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, pixel_shader()));
            
        SetRasterizerState(DisableCulling);
    }
}