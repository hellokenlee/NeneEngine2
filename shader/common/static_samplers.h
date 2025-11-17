/* Copyright reserved by KenLee@hellokenlee@163.com */

#ifndef STATIC_SAMPLERS_H
#define STATIC_SAMPLERS_H

#include "/engine/cppshared/d3d12.h"

SamplerState PointWrapSampler : register(SLOT_POINT_WRAP, SPACE_STATIC_SAMPLER);
SamplerState PointClampSampler : register(SLOT_POINT_CLAMP, SPACE_STATIC_SAMPLER);
SamplerState LinearWrapSampler : register(SLOT_LINEAR_WRAP, SPACE_STATIC_SAMPLER);
SamplerState LinearClampSampler : register(SLOT_LINEAR_CLAMP, SPACE_STATIC_SAMPLER);


#endif // STATIC_SAMPLERS_H