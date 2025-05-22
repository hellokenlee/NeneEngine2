/* Copyright reserved by KenLee@hellokenlee@163.com */

// ReSharper disable CppClangTidyClangDiagnosticFormatNonliteral

#pragma once

#include <ctime>
#include <string>
#include <iomanip>

#include "platform.h"
#include "template/pointer.h"

/**
 * Usage:
 *		LOG(engine, info, TXT("something wrong: %s"), str);
 */
#define LOG(cat, level, fmt, ...) SLOG(cat, level, fmt, __VA_ARGS__)

#define SLOG(cat, level, fmt, ...) zznn_log_category_instance_##cat.slog(level, fmt, __VA_ARGS__)
#define WLOG(cat, level, fmt, ...) zznn_log_category_instance_##cat.wlog(level, fmt, __VA_ARGS__)

/**
 * Usage:
 *		DECLARE_LOG_CATEGORY(engine)
 */
#define DECLARE_LOG_CATEGORY(cat)											\
	class NENE_API zznn_log_category_##cat : public i::log_category			\
	{																		\
	public:																	\
		zznn_log_category_##cat(): i::log_category() {}						\
		virtual std::wstring get_wname() override { return (L#cat); }			\
		virtual std::string get_sname() override { return (#cat); }				\
	};																		\

/**
 * Usage:
 *		DEFINE_LOG_CATEGORY(engine)
 */
#define DEFINE_LOG_CATEGORY(cat) \
	zznn_log_category_##cat zznn_log_category_instance_##cat;

/**
 * Usage:
 *		EXTERN_LOG_CATEGORY(engine)
 */
#define EXTERN_LOG_CATEGORY(cat) \
	extern zznn_log_category_##cat zznn_log_category_instance_##cat;

/**
 * Usage:
 *		LOG(engine, info, TXT("something wrong: %s"), str);
 */
enum log_level
{
	info,
	warning,
	error,
	fatal,

	MAX_COUNT,
};

/**
 * Log implementations
 */
namespace i
{
class NENE_API log_category
{
public:
	log_category();
	log_category(const log_category&) = delete;
	virtual ~log_category();

	/** Single char log */
	void slog(const log_level& level, const char* fmt, ...);

	/** Wide char log */
	void wlog(const log_level& level, const wchar_t* fmt, ...);

	/** If consumes log and outputs to stdout */
	static bool get_consume_log_to_stdout();
	static void set_consume_log_to_stdout(const bool& sw);

	/** Consumer methods from outside */
	static bool consume_slog(std::string& log);
	static bool consume_wlog(std::wstring& log);

	/** Category name interfaces */
	virtual std::wstring get_wname() = 0;
	virtual std::string get_sname() = 0;
	
protected:
	static std::queue<std::wstring> m_wlog_queue;
	static std::queue<std::string> m_slog_queue;
	static bool m_consume_log_to_stdout;
	static std::vector<log_category*> m_all_log_categories;
};
}
