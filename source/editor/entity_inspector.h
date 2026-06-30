/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include "core_object/py.h"


namespace nene
{
	struct entity_inspect_event : typed_event<event_id::entity_inspect_event> {};
	
	class NENE_API entity_inspector : public event_publisher
	{
	public:
		//
		static entity_inspector& get();
		//
		entity_inspector(const entity_inspector&) = delete;
		entity_inspector& operator=(const entity_inspector&) = delete;
		// inspect one entity
		void inspect(uint64_t eid);
		// mark the inspecting entity's components modified
		void modified() const;
		// get the inspecting entity's components, can only be used in scripts
		std::vector<g::reflection::variant> get_inspecting_components() const;
		
	protected:
		entity_inspector() = default;
		
		uint64_t m_inspecting_eid = 0;
	};
}
