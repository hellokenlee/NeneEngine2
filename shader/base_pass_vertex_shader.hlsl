/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "generated/vertex_factory.hlsli"
#include "vertex_factory/vertex_factory_common.hlsli"
#include "base_pass_common.hlsli"
#include "cppshared/view_info.hlsli"

ConstantBuffer<SViewInfo> ViewInfo : register(b0, SPACE_VS);


void Main(in SVertexFactoryInput VertexFactoryInput, out SVertexShaderOutput VertexShaderOutput)
{
	//
	SVertexShaderInput VertexShaderInput = VertexFactoryGetVertexShaderInput<SVertexShaderInput>(VertexFactoryInput);

	//
	float4 WorldPosition = VertexFactoryGetWorldPosition(VertexFactoryInput, VertexShaderInput);
	VertexShaderOutput.NdcPosition = mul(mul(WorldPosition, ViewInfo.ViewMatrix), ViewInfo.ProjectionMatrix);
	VertexShaderOutput.UV = VertexFactoryGetUV(VertexFactoryInput, VertexShaderInput);
}
