#include "base_pass_common.hlsli"

Texture2D BaseColor : register(t0, SPACE_PS); 

void Main(in SVertexShaderOutput Input, out float4 OutColor : SV_TARGET0)
{
	OutColor = BaseColor.Sample(LinearWrapSampler, Input.UV);
}
