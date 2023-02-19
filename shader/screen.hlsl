struct PSInput
{
    float4 position : SV_POSITION;
};

Texture2D screen_texture : register(t0, space0);
SamplerState screen_sampler : register(s0, space100);

PSInput MainVS(float4 position : POSITION, float4 color : COLOR)
{
    PSInput result;
    result.position = position;
    return result;
}

float4 MainPS(PSInput input) : SV_TARGET
{
    float2 uv = input.position.xy;
    return screen_texture.Sample(screen_sampler, uv);
}
