struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

PSInput MainVS(float4 position : POSITION, float4 color : COLOR)
{
    PSInput result;

    result.position = position;
    result.color = color;
    result.position.z = 0.5;
    result.position.w = 1.0;

    return result;
}

float4 MainPS(PSInput input) : SV_TARGET
{
    return input.color;
}
