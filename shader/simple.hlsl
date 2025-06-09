struct VSInput
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
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
