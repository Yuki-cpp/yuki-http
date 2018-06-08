#pragma once

#include <curl/curl.h>

namespace yuki::http
{


class mime_wrapper
{
public:
	explicit mime_wrapper(CURL* handle);
	mime_wrapper(const mime_wrapper&) = delete;
	mime_wrapper& operator=(const mime_wrapper&) = delete;
	mime_wrapper(mime_wrapper&&) = default;
	mime_wrapper& operator=(mime_wrapper && ) = default;
	virtual ~mime_wrapper();

	curl_mime* get_handle() const;

private:
	curl_mime* m_handle;
};



}
