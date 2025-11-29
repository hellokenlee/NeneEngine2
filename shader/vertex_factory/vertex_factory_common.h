/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

template<typename SVertexFactoryInput, typename SVertexShaderInput>
void VertexFactoryGetVertexShaderInput(in SVertexFactoryInput VertexFactoryInput, out SVertexShaderInput VertexShaderInput)
{
	VertexShaderInput = VertexFactoryInput.GetVertexShaderInput();
}

template<typename SVertexFactoryInput, typename SVertexShaderInput>
float3 VertexFactoryGetWorldPosition(in SVertexFactoryInput VertexFactoryInput, out SVertexShaderInput VertexShaderInput)
{
	return VertexShaderInput.GetWorldPosition();
}
