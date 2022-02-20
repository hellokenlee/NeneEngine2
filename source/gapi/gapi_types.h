/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once


enum class gapi_cmd_type
{
	graphics	= 0,
	compute		= 1,
	copy		= 2,
};


enum class gapi_shader_type
{
	vertex_shader			= 0,
	hull_shader				= 1,
	domain_shader			= 2,
	pixel_shader			= 3,
	geometry_shader			= 4,

	compute_shader			= 5,

	ray_gen_shader			= 6,
	ray_miss_shader			= 7,
	ray_hit_group_shader	= 8,
	ray_callable_shader		= 9,
};


enum class gapi_shader_feature_level
{
	shading_model_5_0,
	shading_model_5_1,
	shading_model_6_0,
};
