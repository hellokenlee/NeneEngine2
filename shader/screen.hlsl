
#include "./share/common.h"

struct VSInput
{
    float4 Position : POSITION;
    float4 Color : COLOR;
};

struct PSInput
{
    float4 SvPosition : SV_POSITION;
    float2 ScreenUV : TEXCOORD0;
};

Texture2D ScreenTexture : register(t0, SPACE);

float2 ScreenNdcToUv(float2 Ndc)
{
    float2 NormalizedNdc = (Ndc + float2(1.0, 1.0)) * 0.5;
    float2 ScreenUV = float2(NormalizedNdc.x, 1.0 - NormalizedNdc.y);
    return ScreenUV;
}

PSInput MainVS(VSInput input)
{
    PSInput Result;
    Result.SvPosition = input.Position;
    Result.SvPosition.z = 0.5;
    Result.SvPosition.w = 1.0;
    Result.ScreenUV = ScreenNdcToUv(input.Position.xy);
    return Result;
}

float4 MainPS(PSInput Input) : SV_TARGET
{
    return ScreenTexture.Sample(PointWrapSampler, Input.ScreenUV);
}
