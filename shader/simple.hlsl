struct VSInput
{
    float4 Position : POSITION;
    float4 Color : COLOR;
};

struct PSInput
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
};

PSInput MainVS(VSInput input)
{
    PSInput result;

    result.Position = input.Position;
    result.Color = input.Color;
    result.Position.z = 0.5;
    result.Position.w = 1.0;

    return result;
}

float4 MainPS(PSInput input) : SV_TARGET
{
    return input.Color;
}
