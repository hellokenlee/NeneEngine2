/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

/** template routers: a vertex factory must implement all routed methods */

template<typename SVertexShaderInput, typename SVertexFactoryInput>
SVertexShaderInput VertexFactoryGetVertexShaderInput(SVertexFactoryInput VertexFactoryInput)
{
	return VertexFactoryInput.GetVertexShaderInput();
}

template<typename SVertexFactoryInput, typename SVertexShaderInput>
float4 VertexFactoryGetWorldPosition(SVertexFactoryInput VertexFactoryInput, SVertexShaderInput VertexShaderInput)
{
	return VertexShaderInput.GetWorldPosition();
}
