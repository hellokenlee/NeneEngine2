/* Copyright reserved by KenLee@hellokenlee@163.com */

#include "gapi/gapi.h"


class NENE_API gapi_manager
{
public:
	// >>> Static Methods >>>
	static void create(void* window);

	static void destroy();

	static shared_ptr<gapi> get();
	// <<< Static Methods <<<


	// >>> Disallow Instancing >>>
	gapi_manager() = delete;

	virtual ~gapi_manager() = delete;

	gapi_manager(gapi_manager& other) = delete;

	gapi_manager& operator =(const gapi_manager& other) = delete;
	// <<< Disallow Instancing <<<

protected:
	static shared_ptr<gapi> m_instance;
};
