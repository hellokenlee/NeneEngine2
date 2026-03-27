/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "editor_command/editor_command.h"
#include <memory>
#include <vector>


namespace nene
{
	class NENE_API editor_command_center
	{
	public:
		//
		static editor_command_center& get();
		//
		editor_command_center(const editor_command_center&) = delete;
		editor_command_center& operator=(const editor_command_center&) = delete;
		
		//
		void invoke(const std::shared_ptr<editor_command>& cmd);
		//
		bool undo();
		//
		bool redo();
		//
		bool can_undo() const;
		//
		bool can_redo() const;
		//
		void clear_history();
		
	protected:
		editor_command_center() = default;
		
	private:
		std::vector<std::shared_ptr<editor_command>> m_undo_stack;
		std::vector<std::shared_ptr<editor_command>> m_redo_stack;
	};
}
