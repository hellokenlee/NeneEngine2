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

    result.position = input.position;
    result.color = input.color;
    result.position.z = 0.5;
    result.position.w = 1.0;

    return result;
}

float4 MainPS(PSInput input) : SV_TARGET
{
    return input.color;
}
