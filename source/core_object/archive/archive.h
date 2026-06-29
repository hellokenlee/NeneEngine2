/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "core/core.h"
#include <boost/pfr.hpp>
#include <unordered_map>
#include <type_traits>
#include <cstring>


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
		/** move overload, defaults to copying via the const-ref version */
		virtual void load(std::vector<uint8_t>&& content) { load(static_cast<const std::vector<uint8_t>&>(content)); }
		virtual std::vector<uint8_t> dump() const = 0;
		
		// basic types
		virtual archive& operator<<(const nvp<uint8_t>& kv) = 0;
		virtual archive& operator<<(const nvp<uint16_t>& kv) = 0;
		virtual archive& operator<<(const nvp<uint32_t>& kv) = 0;
		virtual archive& operator<<(const nvp<int8_t>& kv) = 0;
		virtual archive& operator<<(const nvp<int16_t>& kv) = 0;
		virtual archive& operator<<(const nvp<int32_t>& kv) = 0;
		virtual archive& operator<<(const nvp<float>& kv) = 0;
		
		virtual archive& operator<<(const nvp<uint2>& kv) = 0;
		virtual archive& operator<<(const nvp<uint3>& kv) = 0;
		virtual archive& operator<<(const nvp<float2>& kv) = 0;
		virtual archive& operator<<(const nvp<float3>& kv) = 0;
		
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
		/** std::unordered_map<> specialization */
		template<typename key_t, typename value_t>
		archive& operator<<(const nvp<std::unordered_map<key_t, value_t>>& kvs)
		{
			//
			size_t len = kvs.m_data.size();
			enter_array(kvs.m_name, len);
			if (direction() == direction::write)
			{
				for (auto& [key, value] : kvs.m_data)
				{
					key_t k = key;
					size_t pair_len = 2;
					enter_array(nullptr, pair_len);
					*this << nvp<key_t>(nullptr, k);
					*this << nvp<value_t>(nullptr, value);
					leave_array();
				}
			}
			else
			{
				kvs.m_data.clear();
				for (size_t i = 0; i < len; ++i)
				{
					key_t k{};
					value_t v{};
					size_t pair_len = 2;
					enter_array(nullptr, pair_len);
					*this << nvp<key_t>(nullptr, k);
					*this << nvp<value_t>(nullptr, v);
					leave_array();
					kvs.m_data.emplace(std::move(k), std::move(v));
				}
			}
			leave_array();
			//
			return *this;
		}
		
		/** std::vector<> specialization */
		template<typename element_t>
		archive& operator<<(const nvp<std::vector<element_t>>& kvs)
		{
			// fast path: vectors of trivially-copyable scalar types are stored as a
			// single Blob (one builder op + one memcpy) instead of element-by-element.
			// std::vector<bool> is excluded (bit-packed, no contiguous data()).
			if constexpr (std::is_arithmetic_v<element_t> &&
			              std::is_trivially_copyable_v<element_t> &&
			              !std::is_same_v<element_t, bool>)
			{
				if (direction() == direction::write)
				{
					write_blob(
						kvs.m_name,
						reinterpret_cast<const uint8_t*>(kvs.m_data.data()),
						kvs.m_data.size() * sizeof(element_t)
					);
				}
				else
				{
					std::vector<uint8_t> bytes;
					read_blob(kvs.m_name, bytes);
					kvs.m_data.resize(bytes.size() / sizeof(element_t));
					if (!bytes.empty())
					{
						std::memcpy(kvs.m_data.data(), bytes.data(), bytes.size());
					}
				}
				return *this;
			}
			// slow path:
			else
			{
				//
				size_t len = kvs.m_data.size();
				enter_array(kvs.m_name, len);
				if (direction() == direction::read)
				{
					kvs.m_data.resize(len);
				}
				//
				for (auto i = 0; i < len; ++i)
				{
					(*this) << nvp<element_t>(nullptr, kvs.m_data[i]);
				}
				leave_array();
				//
				return *this;
			}
		}
		
		virtual void enter_array(const char* name, size_t& size) = 0;
		virtual void leave_array() = 0;
		
		virtual void enter_object(const char* name) = 0;
		virtual void leave_object() = 0;
		
		/** raw byte block (Blob) read/write, shared fast path for all scalar vectors */
		virtual void write_blob(const char* name, const uint8_t* data, size_t bytes) { CHECK(false); }
		virtual void read_blob(const char* name, std::vector<uint8_t>& bytes) { CHECK(false); }
	};
}

/** auto nvp name constructor */
#define MAKE_NVP(instance, attrib) ::nene::nvp{#attrib, (instance).attrib}
#define MAKE_THIS_NVP(attrib) ::nene::nvp{#attrib, this->attrib}

#define AR(...) NENE_OVERLOAD_2_1(__VA_ARGS__, MAKE_NVP, MAKE_THIS_NVP)(__VA_ARGS__)
