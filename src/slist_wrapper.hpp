#pragma once


#include <string>

#include <curl/curl.h>

namespace yuki::http
{

class curl_slist_wrapper
{
public:
	curl_slist_wrapper() = default;
	curl_slist_wrapper(const curl_slist_wrapper &) = delete;
	curl_slist_wrapper& operator=(const curl_slist_wrapper &) = delete;
	curl_slist_wrapper(curl_slist_wrapper &&) = default;
	curl_slist_wrapper& operator=(curl_slist_wrapper && ) = default;
	~curl_slist_wrapper();

	void append(const std::string & str);
	curl_slist const * get_list() const;

private:
	curl_slist* m_slist = nullptr;
};


}
