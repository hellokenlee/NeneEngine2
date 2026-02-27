/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once


namespace nene::g
{
	class NENE_API scene_component
	{
	public:
		virtual ~scene_component() = default;
		virtual void on_add_to_scene() {}
		virtual void on_remove_from_scene() {}
	};
}
