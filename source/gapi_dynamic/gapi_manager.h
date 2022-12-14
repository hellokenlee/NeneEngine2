/* Copyright reserved by KenLee@hellokenlee@163.com */

#pragma once

#include "gapi/gapi.h"

enum class gapi_platform
{
	direct3d12,
	vulkan,
	metal,
};

class NENE_API gapi_manager
{
public:
	// >>> Static Methods >>>
	/** Create adapter and device from a window handler. */
	static void create(const gapi_platform& platform, void* window);

	/** Init from existing device and adapter. */
	static void initialize(const gapi_platform& platform, void* device);

	static void destroy();

	static t::shared_ptr<gapi> get();
	// <<< Static Methods <<<


	// >>> Disallow Instancing >>>
	gapi_manager() = delete;

	virtual ~gapi_manager() = delete;

	gapi_manager(gapi_manager& other) = delete;

	gapi_manager& operator =(const gapi_manager& other) = delete;
	// <<< Disallow Instancing <<<

protected:
	static t::shared_ptr<gapi> m_instance;
};
