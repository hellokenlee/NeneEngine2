/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "/engine/generated/vertex_factory.h"
#include "/vertex_factory/vertex_factory_common.h"
#include "/base_pass_common.h"
#include "/cppshared/view_uniform_buffer.h"

cbuffer ViewInfoConstantBuffer : register(b0, SPACE_VS) { SViewInfo ViewInfo; };


void Main(in SVertexFactoryInput VertexFactoryInput, out SVertexShaderOutput VertexShaderOutput)
{
	//
	SVertexShaderInput VertexShaderInput = VertexFactoryGetVertexShaderInput<SVertexShaderInput>(VertexFactoryInput);

	//
	float4 WorldPosition = VertexFactoryGetWorldPosition(VertexFactoryInput, VertexShaderInput);
	VertexShaderOutput.NdcPosition = mul(mul(WorldPosition, ViewInfo.ViewMatrix), ViewInfo.ProjectionMatrix);
	VertexShaderOutput.UV = float2(1.0, 0.0);
}
