#include "mime_wrapper.hpp"

#include <stdexcept>

curl_mime * yuki::http::mime_wrapper::get_handle() const
{
	return m_handle;
}


yuki::http::mime_wrapper::mime_wrapper(CURL* parent):
m_handle(curl_mime_init(parent))
{
	if(!m_handle)
	{
		throw std::runtime_error("Can't initialize the curl mime handle");
	}
}


yuki::http::mime_wrapper::~mime_wrapper()
{
	if(m_handle)
	{
		curl_mime_free(m_handle);
	}
}

