#include "yukihttp/request.hpp"

#include <iostream>
#include <string>
#include <stdexcept>

#include <curl/curl.h>

#include "yukihttp/mime.hpp"

#include "slist_wrapper.hpp"
#include "utils.hpp"
#include "mime_part.hpp"



namespace
{

int set_progress_callback_impl(void *clientp,   curl_off_t dltotal,   curl_off_t dlnow,   curl_off_t ultotal,   curl_off_t ulnow)
{
	yuki::http::progress_callback* callback = reinterpret_cast<yuki::http::progress_callback*>(clientp);
	(*callback)(dlnow, dltotal, ulnow, ultotal);
	return 0;
}


int set_verbose_output_impl(CURL*, curl_infotype, char *data, size_t size, void *userptr)
{
	std::ostream* out = reinterpret_cast<std::ostream*>(userptr);
	const std::string out_str(data, size);
	(*out) << out_str;
	return 0;
}



}





yuki::http::request::request(const std::string & url):
	m_handle(curl_easy_init())
{
	if(!m_handle)
	{
		throw std::runtime_error("Can't init cURL");
	}


	if(curl_easy_setopt(m_handle, CURLOPT_URL, url.c_str()) != CURLE_OK)
	{
		throw std::runtime_error("Can't set URL");
	}
	curl_easy_setopt(m_handle, CURLOPT_FOLLOWLOCATION, 1L);
}

yuki::http::request::~request()
{
	if(m_handle != nullptr)
	{
		curl_easy_cleanup(m_handle);
	}
}




void yuki::http::request::set_verbose(bool is_verbose) const
{
	const long value = is_verbose ? 1l : 0l;
	curl_easy_setopt(m_handle, CURLOPT_VERBOSE, value);
}


void yuki::http::request::set_verbose_output(std::ostream& out) const
{
	curl_easy_setopt(m_handle, CURLOPT_DEBUGFUNCTION, set_verbose_output_impl);
	curl_easy_setopt(m_handle, CURLOPT_DEBUGDATA, &out);
}









void yuki::http::request::enable_progress_meter(bool is_enabled) const
{
	const long value = is_enabled ? 0l : 1l;
	curl_easy_setopt(m_handle, CURLOPT_NOPROGRESS, value);
}


void yuki::http::request::set_progress_callback(yuki::http::progress_callback& callback) const
{
	curl_easy_setopt(m_handle, CURLOPT_XFERINFOFUNCTION, set_progress_callback_impl);
	curl_easy_setopt(m_handle, CURLOPT_XFERINFODATA, &callback);
}



void yuki::http::request::set_timeout(unsigned int ms) const
{
	curl_easy_setopt(m_handle, CURLOPT_TIMEOUT_MS, ms);
}




void yuki::http::request::set_user_agent(const std::string& ua) const
{
	curl_easy_setopt(m_handle, CURLOPT_USERAGENT, ua.c_str());
}



void yuki::http::request::set_header(const std::string& name, const std::string& value)
{
	m_headers_map[name] = value;
}




yuki::http::response yuki::http::request::execute_request() const
{
	curl_slist_wrapper m_curl_headers;
	for(auto & it : m_headers_map)
	{
		const std::string header = it.first + ": " + it.second;
		m_curl_headers.append(header);
	}
	auto header_list = m_curl_headers.get_list();
	if(header_list)
	{
		curl_easy_setopt(m_handle, CURLOPT_HTTPHEADER, header_list);
	}
	yuki::http::response rep;

	curl_easy_setopt(m_handle, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(m_handle, CURLOPT_WRITEDATA, &rep);

	curl_easy_setopt(m_handle, CURLOPT_HEADERFUNCTION, header_callback);
	curl_easy_setopt(m_handle, CURLOPT_HEADERDATA, &rep);


	const auto code = curl_easy_perform(m_handle);
	if(code != CURLE_OK)
	{
		rep.code = -1;
	}
	else
	{
		curl_easy_getinfo(m_handle, CURLINFO_RESPONSE_CODE , &rep.code);
	}

	return rep;
}


yuki::http::response yuki::http::request::GET() const
{
	curl_easy_setopt(m_handle, CURLOPT_HTTPGET, 1L);
	return execute_request();
}


yuki::http::response yuki::http::request::HEAD() const
{
	curl_easy_setopt(m_handle, CURLOPT_NOBODY, 1L);
	return execute_request();
}


yuki::http::response yuki::http::request::POST(const std::string & data ) const
{
	curl_easy_setopt(m_handle, CURLOPT_POST, 1L);

	if(data != "")
	{
		curl_easy_setopt(m_handle, CURLOPT_POSTFIELDS, data.c_str());
		curl_easy_setopt(m_handle, CURLOPT_POSTFIELDSIZE, data.size());
	}
	return execute_request();
}


yuki::http::response yuki::http::request::DELETE() const
{
	curl_easy_setopt(m_handle, CURLOPT_CUSTOMREQUEST, "DELETE");
	return execute_request();
}


yuki::http::response yuki::http::request::POST(const yuki::http::mime& mime_data) const
{
	curl_easy_setopt(m_handle, CURLOPT_POST, 1L);
	auto curl_mime_data = curl_mime_init(m_handle);
	for(auto& part : mime_data.m_parts)
	{
		part->set_up_handle(curl_mime_data);
	}
	curl_easy_setopt(m_handle, CURLOPT_MIMEPOST, curl_mime_data);
	return execute_request();
}















































