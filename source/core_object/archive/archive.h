/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "asset_abstract.h"
#include "core/core.h"
#include <boost/pfr.hpp>


namespace nene
{
	/** Name-Value pair */
	template<typename data_t>
	struct nvp
	{
		const char* m_name;
		data_t& m_data;  // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
	};
	// CDAT
	template<typename T>
	nvp(const char*, T&) -> nvp<T>;
	
	/***
	 *	The writer/reader abstract base class
	 */
	class NENE_API archive
	{
	public:
		//
		enum class direction: uint8_t { write, read };
		//
		virtual ~archive() = default;
		virtual direction direction() const = 0;
		virtual void load(const std::vector<uint8_t>& content) = 0;
		virtual std::vector<uint8_t> dump() const = 0;
		
		// basic types
		virtual archive& operator<<(const nvp<uint8_t>& kv) = 0;
		virtual archive& operator<<(const nvp<uint16_t>& kv) = 0;
		virtual archive& operator<<(const nvp<uint32_t>& kv) = 0;
		virtual archive& operator<<(const nvp<int8_t>& kv) = 0;
		virtual archive& operator<<(const nvp<int16_t>& kv) = 0;
		virtual archive& operator<<(const nvp<int32_t>& kv) = 0;
		virtual archive& operator<<(const nvp<float>& kv) = 0;
		
		virtual archive& operator<<(const nvp<uint3>& kv) = 0;
		virtual archive& operator<<(const nvp<float2>& kv) = 0;
		
		virtual archive& operator<<(const nvp<uuid>& kv) = 0;
		
		virtual archive& operator<<(const nvp<std::string>& kv) = 0;
		
		// native types
		/** enum specialization */
		template<typename enum_t>
		requires std::is_enum_v<enum_t>
		archive& operator<<(const nvp<enum_t>& kv)
		{
			using integer_t = std::underlying_type_t<enum_t>;
			if (direction() == direction::write)
			{
				integer_t temp = static_cast<integer_t>(kv.m_data);
				*this << nvp<integer_t>(kv.m_name, temp);
			} 
			else 
			{
				integer_t temp{};
				*this << nvp<integer_t>(kv.m_name, temp);
				kv.m_data = static_cast<enum_t>(temp);
			}
			return *this;
		}
		
		/** aggregate specialization */
		template<typename struct_t>
		requires std::is_aggregate_v<struct_t>
		archive& operator<<(const nvp<struct_t>& kv)
		{
			enter_object(kv.m_name);
			boost::pfr::for_each_field(
				kv.m_data, 
				[&](auto& field, auto index) 
				{
					constexpr auto field_name = boost::pfr::get_name<index, struct_t>();
					*this << nvp(field_name.data(), field); 
				}
			);
			leave_object();
			return *this;
		}
		
		/** adl ( friend operator ) fall back */
		template<typename struct_t>
		requires (!std::is_aggregate_v<struct_t>) && (!std::is_enum_v<struct_t>)
		archive& operator<<(const nvp<struct_t>& kv)
		{
			enter_object(kv.m_name);
			*this << kv.m_data;
			leave_object();
			return *this;
		}
		
		// stl's container types
		/** std::vector<> specialization */
		template<typename element_t>
		archive& operator<<(const nvp<std::vector<element_t>>& kvs)
		{
			//
			size_t len = kvs.m_data.size();
			enter_array(kvs.m_name, len);
			//
			for (auto i = 0; i < len; ++i)
			{
				(*this) << nvp<element_t>(nullptr, kvs.m_data[i]);
			}
			leave_array();
			//
			return *this;
		}
		
	protected:
		virtual void enter_array(const char* name, size_t& size) = 0;
		virtual void leave_array() = 0;
		
		virtual void enter_object(const char* name) = 0;
		virtual void leave_object() = 0;
	};
}

/** auto nvp name constructor */
#define MAKE_NVP(instance, attrib) ::nene::nvp{#attrib, (instance).attrib}
#define MAKE_THIS_NVP(attrib) ::nene::nvp{#attrib, this->attrib}

#define AR(...) NENE_OVERLOAD_2_1(__VA_ARGS__, MAKE_NVP, MAKE_THIS_NVP)(__VA_ARGS__)
