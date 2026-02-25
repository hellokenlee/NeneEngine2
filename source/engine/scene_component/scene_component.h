/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once


namespace nene::g
{
	class scene_component
	{
	public:
		virtual ~scene_component();
		virtual void on_add_to_scene() {}
		virtual void on_remove_from_scene() {}
	};
}
