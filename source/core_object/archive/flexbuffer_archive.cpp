/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "flexbuffer_archive.h"


namespace nene::g
{
	// -------------------------------------------------------------------------
	// flexbuffer_writer
	// -------------------------------------------------------------------------

	flexbuffer_writer::flexbuffer_writer()
	{
		m_stack.emplace_back(container_type::map, m_builder.StartMap());
	}

	bool flexbuffer_writer::current_is_vector() const
	{
		CHECK(!m_finished);
		CHECK(!m_stack.empty());
		return m_stack.back().m_type == container_type::vector;
	}

	void flexbuffer_writer::write_uint(const char* name, uint64_t value)
	{
		if (current_is_vector())
		{
			m_builder.UInt(value);
		}
		else
		{
			CHECK(name != nullptr);
			m_builder.UInt(name, value);
		}
	}

	void flexbuffer_writer::write_int(const char* name, int64_t value)
	{
		if (current_is_vector())
		{
			m_builder.Int(value);
		}
		else
		{
			CHECK(name != nullptr);
			m_builder.Int(name, value);
		}
	}

	void flexbuffer_writer::write_float(const char* name, float value)
	{
		if (current_is_vector())
		{
			m_builder.Float(value);
		}
		else
		{
			CHECK(name != nullptr);
			m_builder.Float(name, value);
		}
	}

	void flexbuffer_writer::write_string(const char* name, const std::string& value)
	{
		if (current_is_vector())
		{
			m_builder.String(value);
		}
		else
		{
			CHECK(name != nullptr);
			m_builder.String(name, value);
		}
	}

	void flexbuffer_writer::write_uint_vector(const char* name, const uint32_t* values, size_t size)
	{
		if (current_is_vector())
		{
			m_builder.FixedTypedVector(values, size);
		}
		else
		{
			CHECK(name != nullptr);
			m_builder.FixedTypedVector(name, values, size);
		}
	}

	void flexbuffer_writer::write_float_vector(const char* name, const float* values, size_t size)
	{
		if (current_is_vector())
		{
			m_builder.FixedTypedVector(values, size);
		}
		else
		{
			CHECK(name != nullptr);
			m_builder.FixedTypedVector(name, values, size);
		}
	}

	void flexbuffer_writer::write_blob(const char* name, const uint8_t* data, size_t bytes)
	{
		if (current_is_vector())
		{
			m_builder.Blob(data, bytes);
		}
		else
		{
			CHECK(name != nullptr);
			m_builder.Blob(name, data, bytes);
		}
	}

	archive& flexbuffer_writer::operator<<(const nvp<uint8_t>& kv)
	{
		write_uint(kv.m_name, kv.m_data);
		return *this;
	}

	archive& flexbuffer_writer::operator<<(const nvp<uint16_t>& kv)
	{
		write_uint(kv.m_name, kv.m_data);
		return *this;
	}

	archive& flexbuffer_writer::operator<<(const nvp<uint32_t>& kv)
	{
		write_uint(kv.m_name, kv.m_data);
		return *this;
	}

	archive& flexbuffer_writer::operator<<(const nvp<int8_t>& kv)
	{
		write_int(kv.m_name, kv.m_data);
		return *this;
	}

	archive& flexbuffer_writer::operator<<(const nvp<int16_t>& kv)
	{
		write_int(kv.m_name, kv.m_data);
		return *this;
	}

	archive& flexbuffer_writer::operator<<(const nvp<int32_t>& kv)
	{
		write_int(kv.m_name, kv.m_data);
		return *this;
	}

	archive& flexbuffer_writer::operator<<(const nvp<float>& kv)
	{
		write_float(kv.m_name, kv.m_data);
		return *this;
	}

	archive& flexbuffer_writer::operator<<(const nvp<uint2>& kv)
	{
		const uint32_t values[] = {kv.m_data.x, kv.m_data.y};
		write_uint_vector(kv.m_name, values, std::size(values));
		return *this;
	}

	archive& flexbuffer_writer::operator<<(const nvp<uint3>& kv)
	{
		const uint32_t values[] = {kv.m_data.x, kv.m_data.y, kv.m_data.z};
		write_uint_vector(kv.m_name, values, std::size(values));
		return *this;
	}

	archive& flexbuffer_writer::operator<<(const nvp<float2>& kv)
	{
		const float values[] = {kv.m_data.x, kv.m_data.y};
		write_float_vector(kv.m_name, values, std::size(values));
		return *this;
	}

	archive& flexbuffer_writer::operator<<(const nvp<float3>& kv)
	{
		const float values[] = {kv.m_data.x, kv.m_data.y, kv.m_data.z};
		write_float_vector(kv.m_name, values, std::size(values));
		return *this;
	}

	archive& flexbuffer_writer::operator<<(const nvp<uuid>& kv)
	{
		write_string(kv.m_name, uuid_to_string(kv.m_data));
		return *this;
	}

	archive& flexbuffer_writer::operator<<(const nvp<std::string>& kv)
	{
		write_string(kv.m_name, kv.m_data);
		return *this;
	}

	void flexbuffer_writer::enter_array(const char* name, size_t& size)
	{
		CHECK(!m_finished);
		if (current_is_vector())
		{
			m_stack.emplace_back(container_type::vector, m_builder.StartVector());
		}
		else
		{
			CHECK(name != nullptr);
			m_stack.emplace_back(container_type::vector, m_builder.StartVector(name));
		}
	}

	void flexbuffer_writer::leave_array()
	{
		CHECK(!m_finished);
		CHECK(m_stack.size() > 1);
		CHECK(m_stack.back().m_type == container_type::vector);
		const auto start = m_stack.back().m_start;
		m_stack.pop_back();
		m_builder.EndVector(start, false, false);
	}

	void flexbuffer_writer::enter_object(const char* name)
	{
		CHECK(!m_finished);
		if (current_is_vector())
		{
			m_stack.emplace_back(container_type::map, m_builder.StartMap());
		}
		else
		{
			CHECK(name != nullptr);
			m_stack.emplace_back(container_type::map, m_builder.StartMap(name));
		}
	}

	void flexbuffer_writer::leave_object()
	{
		CHECK(!m_finished);
		CHECK(m_stack.size() > 1);
		CHECK(m_stack.back().m_type == container_type::map);
		const auto start = m_stack.back().m_start;
		m_stack.pop_back();
		m_builder.EndMap(start);
#ifndef NDEBUG
		CHECK(!m_builder.HasDuplicateKeys());
#endif
	}

	std::vector<uint8_t> flexbuffer_writer::dump() const
	{
		if (!m_finished)
		{
			CHECK(m_stack.size() == 1);
			CHECK(m_stack.back().m_type == container_type::map);
			const auto start = m_stack.back().m_start;
			m_stack.pop_back();
			m_builder.EndMap(start);
#ifndef NDEBUG
			CHECK(!m_builder.HasDuplicateKeys());
#endif
			m_builder.Finish();
			m_finished = true;
		}
		return m_builder.GetBuffer();
	}

	// -------------------------------------------------------------------------
	// flexbuffer_reader
	// -------------------------------------------------------------------------

	namespace
	{
		bool is_vector_like(const flexbuffers::Reference& ref)
		{
			return ref.IsUntypedVector() || ref.IsTypedVector() || ref.IsFixedTypedVector();
		}

		flexbuffers::Reference make_empty_map()
		{
			static const auto empty = [] {
				flexbuffers::Builder builder;
				const auto start = builder.StartMap();
				builder.EndMap(start);
				builder.Finish();
				return builder.GetBuffer();
			}();

			return flexbuffers::GetRoot(empty);
		}
	}

	void flexbuffer_reader::load(const std::vector<uint8_t>& content)
	{
		NENE_PROFILER_ZONE();

		m_content = content;
		load_impl();
	}

	void flexbuffer_reader::load(std::vector<uint8_t>&& content)
	{
		NENE_PROFILER_ZONE();

		m_content = std::move(content);
		load_impl();
	}

	void flexbuffer_reader::load_impl()
	{
		m_stack.clear();
		m_array_index_stack.clear();

		if (m_content.empty())
		{
			m_root = make_empty_map();
		}
		else
		{
			CHECK(flexbuffers::VerifyBuffer(m_content.data(), m_content.size()));
			m_root = flexbuffers::GetRoot(m_content);
		}

		CHECK(m_root.IsMap());
		m_stack.emplace_back(m_root);
	}

	size_t flexbuffer_reader::vector_size(const flexbuffers::Reference& ref)
	{
		if (ref.IsUntypedVector())
		{
			return ref.AsVector().size();
		}
		if (ref.IsTypedVector())
		{
			return ref.AsTypedVector().size();
		}
		if (ref.IsFixedTypedVector())
		{
			return ref.AsFixedTypedVector().size();
		}
		return 0;
	}

	flexbuffers::Reference flexbuffer_reader::vector_at(const flexbuffers::Reference& ref, size_t index)
	{
		if (ref.IsUntypedVector())
		{
			return ref.AsVector()[index];
		}
		if (ref.IsTypedVector())
		{
			return ref.AsTypedVector()[index];
		}
		if (ref.IsFixedTypedVector())
		{
			return ref.AsFixedTypedVector()[index];
		}
		return {};
	}

	flexbuffers::Reference flexbuffer_reader::next_value(const char* name)
	{
		CHECK(!m_stack.empty());
		const auto cur = m_stack.back();
		if (cur.IsMap())
		{
			CHECK(name != nullptr);
			return cur.AsMap()[name];
		}

		CHECK(is_vector_like(cur));
		CHECK(!m_array_index_stack.empty());
		const size_t idx = m_array_index_stack.back()++;
		return vector_at(cur, idx);
	}

	archive& flexbuffer_reader::operator<<(const nvp<uint8_t>& kv)
	{
		const auto ref = next_value(kv.m_name);
		if (ref.IsIntOrUint())
		{
			kv.m_data = ref.AsUInt8();
		}
		return *this;
	}

	archive& flexbuffer_reader::operator<<(const nvp<uint16_t>& kv)
	{
		const auto ref = next_value(kv.m_name);
		if (ref.IsIntOrUint())
		{
			kv.m_data = ref.AsUInt16();
		}
		return *this;
	}

	archive& flexbuffer_reader::operator<<(const nvp<uint32_t>& kv)
	{
		const auto ref = next_value(kv.m_name);
		if (ref.IsIntOrUint())
		{
			kv.m_data = ref.AsUInt32();
		}
		return *this;
	}

	archive& flexbuffer_reader::operator<<(const nvp<int8_t>& kv)
	{
		const auto ref = next_value(kv.m_name);
		if (ref.IsIntOrUint())
		{
			kv.m_data = ref.AsInt8();
		}
		return *this;
	}

	archive& flexbuffer_reader::operator<<(const nvp<int16_t>& kv)
	{
		const auto ref = next_value(kv.m_name);
		if (ref.IsIntOrUint())
		{
			kv.m_data = ref.AsInt16();
		}
		return *this;
	}

	archive& flexbuffer_reader::operator<<(const nvp<int32_t>& kv)
	{
		const auto ref = next_value(kv.m_name);
		if (ref.IsIntOrUint())
		{
			kv.m_data = ref.AsInt32();
		}
		return *this;
	}

	archive& flexbuffer_reader::operator<<(const nvp<float>& kv)
	{
		const auto ref = next_value(kv.m_name);
		if (ref.IsNumeric())
		{
			kv.m_data = ref.AsFloat();
		}
		return *this;
	}

	archive& flexbuffer_reader::operator<<(const nvp<uint2>& kv)
	{
		const auto ref = next_value(kv.m_name);
		if (is_vector_like(ref) && vector_size(ref) >= 2)
		{
			kv.m_data.x = vector_at(ref, 0).AsUInt32();
			kv.m_data.y = vector_at(ref, 1).AsUInt32();
		}
		return *this;
	}

	archive& flexbuffer_reader::operator<<(const nvp<uint3>& kv)
	{
		const auto ref = next_value(kv.m_name);
		if (is_vector_like(ref) && vector_size(ref) >= 3)
		{
			kv.m_data.x = vector_at(ref, 0).AsUInt32();
			kv.m_data.y = vector_at(ref, 1).AsUInt32();
			kv.m_data.z = vector_at(ref, 2).AsUInt32();
		}
		return *this;
	}

	archive& flexbuffer_reader::operator<<(const nvp<float2>& kv)
	{
		const auto ref = next_value(kv.m_name);
		if (is_vector_like(ref) && vector_size(ref) >= 2)
		{
			kv.m_data.x = vector_at(ref, 0).AsFloat();
			kv.m_data.y = vector_at(ref, 1).AsFloat();
		}
		return *this;
	}

	archive& flexbuffer_reader::operator<<(const nvp<float3>& kv)
	{
		const auto ref = next_value(kv.m_name);
		if (is_vector_like(ref) && vector_size(ref) >= 3)
		{
			kv.m_data.x = vector_at(ref, 0).AsFloat();
			kv.m_data.y = vector_at(ref, 1).AsFloat();
			kv.m_data.z = vector_at(ref, 2).AsFloat();
		}
		return *this;
	}

	archive& flexbuffer_reader::operator<<(const nvp<uuid>& kv)
	{
		const auto ref = next_value(kv.m_name);
		if (ref.IsString())
		{
			kv.m_data = string_to_uuid(ref.AsString().str());
		}
		return *this;
	}

	archive& flexbuffer_reader::operator<<(const nvp<std::string>& kv)
	{
		const auto ref = next_value(kv.m_name);
		if (ref.IsString())
		{
			kv.m_data = ref.AsString().str();
		}
		return *this;
	}

	void flexbuffer_reader::read_blob(const char* name, std::vector<uint8_t>& bytes)
	{
		const auto ref = next_value(name);
		if (ref.IsBlob())
		{
			const auto blob = ref.AsBlob();
			bytes.assign(blob.data(), blob.data() + blob.size());
		}
		else
		{
			bytes.clear();
		}
	}

	void flexbuffer_reader::enter_array(const char* name, size_t& size)
	{
		const auto ref = next_value(name);
		CHECK(is_vector_like(ref));
		m_stack.emplace_back(ref);
		size = vector_size(ref);
		m_array_index_stack.emplace_back(0);
	}

	void flexbuffer_reader::leave_array()
	{
		CHECK(m_stack.size() > 1);
		CHECK(!m_array_index_stack.empty());
		m_stack.pop_back();
		m_array_index_stack.pop_back();
	}

	void flexbuffer_reader::enter_object(const char* name)
	{
		const auto ref = next_value(name);
		CHECK(ref.IsMap());
		m_stack.emplace_back(ref);
	}

	void flexbuffer_reader::leave_object()
	{
		CHECK(m_stack.size() > 1);
		m_stack.pop_back();
	}
}
