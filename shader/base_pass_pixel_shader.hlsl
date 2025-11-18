#include "base_pass_common.h"

void Main(in SVertexShaderOutput Input, out float4 OutColor : SV_TARGET0)
{
	OutColor = float4(Input.UV.x, Input.UV.y, 0.0, 0.0);
}