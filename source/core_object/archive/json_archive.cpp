/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "json_archive.h"
#include <cstdio>
#include <zstd.h>


namespace nene::g
{
	// -------------------------------------------------------------------------
	// json_writer
	// -------------------------------------------------------------------------

	json_writer::json_writer()
	{
		// 初始化根节点为空 object，栈底指向根
		m_root = nlohmann::json::object();
		m_stack.emplace_back(&m_root);
	}

	archive& json_writer::operator<<(const nvp<uint8_t>& kv)
	{
		nlohmann::json* parent = m_stack.back();
		if (parent->is_array())
			parent->emplace_back(kv.m_data);
		else
			(*parent)[kv.m_name] = kv.m_data;
		return *this;
	}

	archive& json_writer::operator<<(const nvp<uint16_t>& kv)
	{
		nlohmann::json* parent = m_stack.back();
		if (parent->is_array())
			parent->emplace_back(kv.m_data);
		else
			(*parent)[kv.m_name] = kv.m_data;
		return *this;
	}

	archive& json_writer::operator<<(const nvp<uint32_t>& kv)
	{
		nlohmann::json* parent = m_stack.back();
		if (parent->is_array())
			parent->emplace_back(kv.m_data);
		else
			(*parent)[kv.m_name] = kv.m_data;
		return *this;
	}

	archive& json_writer::operator<<(const nvp<int8_t>& kv)
	{
		nlohmann::json* parent = m_stack.back();
		if (parent->is_array())
			parent->emplace_back(kv.m_data);
		else
			(*parent)[kv.m_name] = kv.m_data;
		return *this;
	}

	archive& json_writer::operator<<(const nvp<int16_t>& kv)
	{
		nlohmann::json* parent = m_stack.back();
		if (parent->is_array())
			parent->emplace_back(kv.m_data);
		else
			(*parent)[kv.m_name] = kv.m_data;
		return *this;
	}

	archive& json_writer::operator<<(const nvp<int32_t>& kv)
	{
		nlohmann::json* parent = m_stack.back();
		if (parent->is_array())
			parent->emplace_back(kv.m_data);
		else
			(*parent)[kv.m_name] = kv.m_data;
		return *this;
	}

	archive& json_writer::operator<<(const nvp<float>& kv)
	{
		nlohmann::json* parent = m_stack.back();
		if (parent->is_array())
			parent->emplace_back(kv.m_data);
		else
			(*parent)[kv.m_name] = kv.m_data;
		return *this;
	}

	archive& json_writer::operator<<(const nvp<uint3>& kv)
	{
		nlohmann::json* parent = m_stack.back();
		auto arr = nlohmann::json::array({kv.m_data.x, kv.m_data.y, kv.m_data.z});
		if (parent->is_array())
			parent->emplace_back(std::move(arr));
		else
			(*parent)[kv.m_name] = std::move(arr);
		return *this;
	}

	archive& json_writer::operator<<(const nvp<float2>& kv)
	{
		nlohmann::json* parent = m_stack.back();
		auto arr = nlohmann::json::array({kv.m_data.x, kv.m_data.y});
		if (parent->is_array())
			parent->emplace_back(std::move(arr));
		else
			(*parent)[kv.m_name] = std::move(arr);
		return *this;
	}

	archive& json_writer::operator<<(const nvp<uuid>& kv)
	{
		nlohmann::json* parent = m_stack.back();
		auto str = uuid_to_string(kv.m_data);
		if (parent->is_array())
			parent->emplace_back(std::move(str));
		else
			(*parent)[kv.m_name] = std::move(str);
		return *this;
	}

	archive& json_writer::operator<<(const nvp<std::string>& kv)
	{
		nlohmann::json* parent = m_stack.back();
		if (parent->is_array())
			parent->emplace_back(kv.m_data);
		else
			(*parent)[kv.m_name] = kv.m_data;
		return *this;
	}

	void json_writer::enter_array(const char* name, size_t& size)
	{
		nlohmann::json* parent = m_stack.back();
		if (parent->is_array())
		{
			parent->emplace_back(nlohmann::json::array());
			m_stack.emplace_back(&parent->back());
		}
		else
		{
			(*parent)[name] = nlohmann::json::array();
			m_stack.emplace_back(&(*parent)[name]);
		}
	}

	void json_writer::leave_array()
	{
		// 弹出数组节点，回到父节点
		CHECK(m_stack.size() > 1);
		m_stack.pop_back();
	}

	void json_writer::enter_object(const char* name)
	{
		// 进入子 object：若父是 array 则追加新 object；否则按 name 创建子 object
		nlohmann::json* parent = m_stack.back();
		if (parent->is_array())
		{
			parent->emplace_back(nlohmann::json::object());
			m_stack.emplace_back(&parent->back());
		}
		else
		{
			(*parent)[name] = nlohmann::json::object();
			m_stack.emplace_back(&(*parent)[name]);
		}
	}

	void json_writer::leave_object()
	{
		// 弹出 object 节点，回到父节点
		CHECK(m_stack.size() > 1);
		m_stack.pop_back();
	}

	void json_writer::write(const std::string& file_path) const
	{
		if (file_path.empty())
		{
			return;
		}

		std::vector<std::uint8_t> bjdata = nlohmann::json::to_bjdata(m_root);

		const size_t compress_bound = ZSTD_compressBound(bjdata.size());
		std::vector<std::uint8_t> compressed(compress_bound);
		const size_t compressed_size = ZSTD_compress(compressed.data(), compress_bound, bjdata.data(), bjdata.size(), 19);
		CHECK(!ZSTD_isError(compressed_size));

		// ReSharper disable once CppDeprecatedEntity
		FILE* fp = fopen(file_path.c_str(), "wb");
		if (!fp)
		{
			return;
		}

		const uint64_t original_size = bjdata.size();
		size_t written = fwrite(&original_size, sizeof(original_size), 1, fp);
		CHECK(written == 1);
		written = fwrite(compressed.data(), sizeof(uint8_t), compressed_size, fp);
		CHECK(written == compressed_size);
		ENSURE(fclose(fp) != -1);
	}

	// -------------------------------------------------------------------------
	// json_reader
	// -------------------------------------------------------------------------

	void json_reader::read(const std::string& file_path)
	{
		m_root = nlohmann::json::object();
		m_stack.clear();

		if (file_path.empty())
		{
			m_stack.emplace_back(&m_root);
			return;
		}

		// ReSharper disable once CppDeprecatedEntity
		FILE* fp = fopen(file_path.c_str(), "rb");
		if (!fp)
		{
			m_stack.emplace_back(&m_root);
			return;
		}

		(void)fseek(fp, 0, SEEK_END);
		long file_size = ftell(fp);
		(void)fseek(fp, 0, SEEK_SET);
		if (file_size > static_cast<long>(sizeof(uint64_t)))
		{
			uint64_t original_size = 0;
			size_t read_count = fread(&original_size, sizeof(original_size), 1, fp);
			CHECK(read_count == 1);

			const size_t compressed_size = static_cast<size_t>(file_size) - sizeof(uint64_t);
			std::vector<std::uint8_t> compressed(compressed_size);
			read_count = fread(compressed.data(), sizeof(std::uint8_t), compressed_size, fp);
			CHECK(read_count == compressed_size);

			std::vector<std::uint8_t> bjdata(static_cast<size_t>(original_size));
			const size_t decompressed_size = ZSTD_decompress(bjdata.data(), bjdata.size(), compressed.data(), compressed_size);
			CHECK(!ZSTD_isError(decompressed_size) && decompressed_size == original_size);

			m_root = nlohmann::json::from_bjdata(bjdata);
		}

		ENSURE(fclose(fp) != -1);
		m_stack.emplace_back(&m_root);
	}

	archive& json_reader::operator<<(const nvp<uint8_t>& kv)
	{
		const nlohmann::json* cur = m_stack.back();
		if (cur->is_array())
		{
			size_t idx = m_array_index_stack.back()++;
			if (idx < cur->size() && (*cur)[idx].is_number_unsigned())
				kv.m_data = (*cur)[idx].get<uint8_t>();
		}
		else if (cur->contains(kv.m_name) && (*cur)[kv.m_name].is_number_unsigned())
		{
			kv.m_data = (*cur)[kv.m_name].get<uint8_t>();
		}
		return *this;
	}

	archive& json_reader::operator<<(const nvp<uint16_t>& kv)
	{
		const nlohmann::json* cur = m_stack.back();
		if (cur->is_array())
		{
			size_t idx = m_array_index_stack.back()++;
			if (idx < cur->size() && (*cur)[idx].is_number_unsigned())
				kv.m_data = (*cur)[idx].get<uint16_t>();
		}
		else if (cur->contains(kv.m_name) && (*cur)[kv.m_name].is_number_unsigned())
		{
			kv.m_data = (*cur)[kv.m_name].get<uint16_t>();
		}
		return *this;
	}

	archive& json_reader::operator<<(const nvp<uint32_t>& kv)
	{
		const nlohmann::json* cur = m_stack.back();
		if (cur->is_array())
		{
			size_t idx = m_array_index_stack.back()++;
			if (idx < cur->size() && (*cur)[idx].is_number_unsigned())
				kv.m_data = (*cur)[idx].get<uint32_t>();
		}
		else if (cur->contains(kv.m_name) && (*cur)[kv.m_name].is_number_unsigned())
		{
			kv.m_data = (*cur)[kv.m_name].get<uint32_t>();
		}
		return *this;
	}

	archive& json_reader::operator<<(const nvp<int8_t>& kv)
	{
		const nlohmann::json* cur = m_stack.back();
		if (cur->is_array())
		{
			size_t idx = m_array_index_stack.back()++;
			if (idx < cur->size() && (*cur)[idx].is_number_integer())
				kv.m_data = (*cur)[idx].get<int8_t>();
		}
		else if (cur->contains(kv.m_name) && (*cur)[kv.m_name].is_number_integer())
		{
			kv.m_data = (*cur)[kv.m_name].get<int8_t>();
		}
		return *this;
	}

	archive& json_reader::operator<<(const nvp<int16_t>& kv)
	{
		const nlohmann::json* cur = m_stack.back();
		if (cur->is_array())
		{
			size_t idx = m_array_index_stack.back()++;
			if (idx < cur->size() && (*cur)[idx].is_number_integer())
				kv.m_data = (*cur)[idx].get<int16_t>();
		}
		else if (cur->contains(kv.m_name) && (*cur)[kv.m_name].is_number_integer())
		{
			kv.m_data = (*cur)[kv.m_name].get<int16_t>();
		}
		return *this;
	}

	archive& json_reader::operator<<(const nvp<int32_t>& kv)
	{
		const nlohmann::json* cur = m_stack.back();
		if (cur->is_array())
		{
			size_t idx = m_array_index_stack.back()++;
			if (idx < cur->size() && (*cur)[idx].is_number_integer())
				kv.m_data = (*cur)[idx].get<int32_t>();
		}
		else if (cur->contains(kv.m_name) && (*cur)[kv.m_name].is_number_integer())
		{
			kv.m_data = (*cur)[kv.m_name].get<int32_t>();
		}
		return *this;
	}

	archive& json_reader::operator<<(const nvp<float>& kv)
	{
		const nlohmann::json* cur = m_stack.back();
		if (cur->is_array())
		{
			size_t idx = m_array_index_stack.back()++;
			if (idx < cur->size() && (*cur)[idx].is_number())
				kv.m_data = (*cur)[idx].get<float>();
		}
		else if (cur->contains(kv.m_name) && (*cur)[kv.m_name].is_number())
		{
			kv.m_data = (*cur)[kv.m_name].get<float>();
		}
		return *this;
	}

	archive& json_reader::operator<<(const nvp<uint3>& kv)
	{
		const nlohmann::json* cur = m_stack.back();
		if (cur->is_array())
		{
			size_t idx = m_array_index_stack.back()++;
			if (idx < cur->size() && (*cur)[idx].is_array() && (*cur)[idx].size() >= 3)
			{
				const auto& arr = (*cur)[idx];
				kv.m_data.x = arr[0].get<uint32_t>();
				kv.m_data.y = arr[1].get<uint32_t>();
				kv.m_data.z = arr[2].get<uint32_t>();
			}
		}
		else if (cur->contains(kv.m_name) && (*cur)[kv.m_name].is_array() && (*cur)[kv.m_name].size() >= 3)
		{
			const auto& arr = (*cur)[kv.m_name];
			kv.m_data.x = arr[0].get<uint32_t>();
			kv.m_data.y = arr[1].get<uint32_t>();
			kv.m_data.z = arr[2].get<uint32_t>();
		}
		return *this;
	}

	archive& json_reader::operator<<(const nvp<float2>& kv)
	{
		const nlohmann::json* cur = m_stack.back();
		if (cur->is_array())
		{
			size_t idx = m_array_index_stack.back()++;
			if (idx < cur->size() && (*cur)[idx].is_array() && (*cur)[idx].size() >= 2)
			{
				const auto& arr = (*cur)[idx];
				kv.m_data.x = arr[0].get<float>();
				kv.m_data.y = arr[1].get<float>();
			}
		}
		else if (cur->contains(kv.m_name) && (*cur)[kv.m_name].is_array() && (*cur)[kv.m_name].size() >= 2)
		{
			const auto& arr = (*cur)[kv.m_name];
			kv.m_data.x = arr[0].get<float>();
			kv.m_data.y = arr[1].get<float>();
		}
		return *this;
	}

	archive& json_reader::operator<<(const nvp<uuid>& kv)
	{
		const nlohmann::json* cur = m_stack.back();
		if (cur->is_array())
		{
			size_t idx = m_array_index_stack.back()++;
			if (idx < cur->size() && (*cur)[idx].is_string())
				kv.m_data = string_to_uuid((*cur)[idx].get<std::string>());
		}
		else if (cur->contains(kv.m_name) && (*cur)[kv.m_name].is_string())
		{
			kv.m_data = string_to_uuid((*cur)[kv.m_name].get<std::string>());
		}
		return *this;
	}

	archive& json_reader::operator<<(const nvp<std::string>& kv)
	{
		const nlohmann::json* cur = m_stack.back();
		if (cur->is_array())
		{
			size_t idx = m_array_index_stack.back()++;
			if (idx < cur->size() && (*cur)[idx].is_string())
				kv.m_data = (*cur)[idx].get<std::string>();
		}
		else if (cur->contains(kv.m_name) && (*cur)[kv.m_name].is_string())
		{
			kv.m_data = (*cur)[kv.m_name].get<std::string>();
		}
		return *this;
	}

	void json_reader::enter_array(const char* name, size_t& size)
	{
		nlohmann::json* cur = m_stack.back();
		if (cur->is_array())
		{
			size_t idx = m_array_index_stack.back()++;
			CHECK(idx < cur->size() && (*cur)[idx].is_array());
			m_stack.emplace_back(&(*cur)[idx]);
		}
		else
		{
			CHECK(cur->contains(name) && (*cur)[name].is_array());
			m_stack.emplace_back(&(*cur)[name]);
		}
		size = m_stack.back()->size();
		m_array_index_stack.emplace_back(0);
	}

	void json_reader::leave_array()
	{
		// 弹出数组节点和对应的元素索引
		CHECK(m_stack.size() > 1);
		m_stack.pop_back();
		m_array_index_stack.pop_back();
	}

	void json_reader::enter_object(const char* name)
	{
		// 进入子 object：若父是 array 则按索引顺序取下一个元素；否则按 name 导航
		nlohmann::json* cur = m_stack.back();
		if (cur->is_array())
		{
			size_t idx = m_array_index_stack.back()++;
			CHECK(idx < cur->size());
			m_stack.emplace_back(&(*cur)[idx]);
		}
		else
		{
			CHECK(cur->contains(name) && (*cur)[name].is_object());
			m_stack.emplace_back(&(*cur)[name]);
		}
	}

	void json_reader::leave_object()
	{
		// 弹出 object 节点，回到父节点
		CHECK(m_stack.size() > 1);
		m_stack.pop_back();
	}
}
