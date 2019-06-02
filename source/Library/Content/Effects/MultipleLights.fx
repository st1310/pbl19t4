#include "include\\Common.fxh"

/************* Resources *************/

#define NUM_LIGHTS 10

cbuffer CBufferPerFrame
{
	DIRECTIONAL_LIGHT DirectLights[NUM_LIGHTS];
    POINT_LIGHT PointLights[NUM_LIGHTS];
	SPOT_LIGHT SpotLights[NUM_LIGHTS];
    
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
Texture2D NormalTexture;

SamplerState TrilinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

RasterizerState DisableCulling
{
    CullMode = NONE;
};

BlendState EnableAlphaBlending
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};

/************* Data Structures *************/

struct VS_INPUT
{
    float4 ObjectPosition : POSITION;
    float2 TextureCoordinate : TEXCOORD;
    float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
};

struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float3 WorldPosition : POSITION;
    float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float3 Binormal : BINORMAL;
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
	OUT.Tangent = normalize(mul(float4(IN.Tangent, 0), World).xyz);
	OUT.Binormal = cross(OUT.Normal, OUT.Tangent);

    return OUT;
}

/************* Pixel Shader *************/

float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 OUT = (float4)0;
        
    float3 normal = normalize(IN.Normal);
	float3 tangent = normalize(IN.Tangent);
	float3 binormal = normalize(IN.Binormal);
    float3 viewDirection = normalize(CameraPosition - IN.WorldPosition);	
    float4 color = ColorTexture.Sample(TrilinearSampler, IN.TextureCoordinate);
    float3 ambient = get_vector_color_contribution(AmbientColor, color.rgb);

	float3 sampledNormal = (2 * NormalTexture.Sample(TrilinearSampler, IN.TextureCoordinate).xyz) - 1.0; // Map normal from [0..1] to [-1..1]
	float3x3 tbn = float3x3(tangent, binormal, normal);
	sampledNormal = mul(sampledNormal, tbn); // Transform normal to world space
        
    LIGHT_CONTRIBUTION_DATA lightContributionData;
    lightContributionData.Color = color;
	lightContributionData.Normal = sampledNormal; //sampledNormal;
    lightContributionData.ViewDirection = viewDirection;
    lightContributionData.SpecularColor = SpecularColor;
    lightContributionData.SpecularPower = SpecularPower;
    
    float3 totalLightContribution = (float3)0;

	for (int i = 0; i < NUM_LIGHTS && DirectLights[i].Enabled; i++)
	{
		lightContributionData.LightDirection.rgb = normalize(-DirectLights[i].Direction);
		lightContributionData.LightDirection.a = 1.0f;
		lightContributionData.LightColor = DirectLights[i].Color;
		totalLightContribution += get_light_contribution(lightContributionData);
	}
    
    for (i = 0; i < NUM_LIGHTS && PointLights[i].Enabled; i++)
    {
        lightContributionData.LightDirection = get_point_light_data(PointLights[i].Position, IN.WorldPosition, PointLights[i].LightRadius);
        lightContributionData.LightColor = PointLights[i].Color;
        totalLightContribution += get_light_contribution(lightContributionData);
    }

	float spotFactor = 0.0f;
	for (i = 0; i < NUM_LIGHTS && SpotLights[i].Enabled; i++)
	{
		lightContributionData.LightDirection = get_spot_light_data(SpotLights[i].Position, IN.WorldPosition, SpotLights[i].OuterAngle, SpotLights[i].InnerAngle, SpotLights[i].LightRadius);
		lightContributionData.LightColor = SpotLights[i].Color;

		spotFactor = get_spot_factor(dot(-SpotLights[i].Direction, lightContributionData.LightDirection.rgb), SpotLights[i].OuterAngle, SpotLights[i].InnerAngle);
		totalLightContribution += (spotFactor * get_light_contribution(lightContributionData));
	}

    OUT.rgb = ambient + totalLightContribution;
	OUT.a = 1.0f; //OUT.a = color.a;
        
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
		//SetBlendState(EnableAlphaBlending, (float4)0, 0xFFFFFFFF);
    }
}