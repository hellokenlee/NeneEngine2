/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "common/common.hlsli"

struct SVertexShaderOutput
{
	float4 NdcPosition : SV_POSITION;
	float2 UV : TEXCOORD0;
};
