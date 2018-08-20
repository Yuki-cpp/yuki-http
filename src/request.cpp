#include "yukihttp/request.hpp"

#include <iostream>
#include <string>
#include <stdexcept>

#include <curl/curl.h>

#include "yukihttp/mime.hpp"

#include "slist_wrapper.hpp"
#include "utils.hpp"
#include "mime_part.hpp"
#include "mime_wrapper.hpp"


namespace
{


/**
* @brief ...
*
* @param provided_callback p_provided_callback:...
* @param expected_download p_expected_download:...
* @param downloaded p_downloaded:...
* @param expected_upload p_expected_upload:...
* @param uploaded p_uploaded:...
* @return int
*/
int set_progress_callback_impl(void * provided_callback,   curl_off_t expected_download,   curl_off_t downloaded,   curl_off_t expected_upload,   curl_off_t uploaded)
{
	if(provided_callback)
	{
		auto* callback = reinterpret_cast<yuki::http::progress_callback*>(provided_callback);
		(*callback)(downloaded, expected_download, uploaded, expected_upload);
		return 0;
	}

	return -1;
}


/**
* @brief ...
*
* @param handle p_handle:...
* @param type p_type:...
* @param data p_data:...
* @param size p_size:... Defaults to maybe_unused.
* @param debug_callback p_debug_callback:... Defaults to maybe_unused.
* @return int
*/
int set_verbose_output_impl(CURL* handle [[maybe_unused]], curl_infotype type [[maybe_unused]], char *data, size_t size, void *debug_callback)
{
	if(debug_callback and data)
	{
		auto* out = reinterpret_cast<std::ostream*>(debug_callback);
		const std::string out_str(data, size);
		(*out) << out_str;
	}
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

	if(curl_easy_setopt(m_handle, CURLOPT_FOLLOWLOCATION, 1L) != CURLE_OK)
	{
		throw std::runtime_error("HTTP not supported");
	}
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
	const auto value = is_verbose ? 1l : 0l;
	curl_easy_setopt(m_handle, CURLOPT_VERBOSE, value);
}


void yuki::http::request::set_verbose_output(std::ostream& out) const
{
	curl_easy_setopt(m_handle, CURLOPT_DEBUGFUNCTION, set_verbose_output_impl);
	curl_easy_setopt(m_handle, CURLOPT_DEBUGDATA, &out);
}





void yuki::http::request::enable_progress_meter(bool is_enabled) const
{
	const auto value = is_enabled ? 0l : 1l;
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
	if(curl_easy_setopt(m_handle, CURLOPT_USERAGENT, ua.c_str()) != CURLE_OK)
	{
		throw std::runtime_error("HTTP not supported");
	}
}




void yuki::http::request::set_auth_username(const std::string& username) const
{
	if(curl_easy_setopt(m_handle, CURLOPT_USERNAME, username.c_str()) != CURLE_OK)
	{
		throw std::runtime_error("Username setting not supported");
	}
}


void yuki::http::request::set_auth_password(const std::string& password) const
{
	if( curl_easy_setopt(m_handle, CURLOPT_PASSWORD, password.c_str()) != CURLE_OK)
	{
		throw std::runtime_error("Password setting not supported");
	}
}




void yuki::http::request::set_header(const std::string& name, const std::string& value)
{
	m_headers_map[name] = value;
}




yuki::http::response yuki::http::request::execute_request() const
{
	curl_slist_wrapper m_curl_headers;
	for(const auto & it : m_headers_map)
	{
		const auto header = it.first + ": " + it.second;
		m_curl_headers.append(header);
	}
	
	auto header_list = m_curl_headers.get_list();
	if(header_list)
	{
		if(	curl_easy_setopt(m_handle, CURLOPT_HTTPHEADER, header_list) != CURLE_OK)
		{
			throw std::runtime_error("HTTP not supported");
		}
	}
	
	yuki::http::response rep;

	curl_easy_setopt(m_handle, CURLOPT_WRITEFUNCTION, yuki::http::callbacks::write_callback);
	curl_easy_setopt(m_handle, CURLOPT_WRITEDATA, &rep);

	curl_easy_setopt(m_handle, CURLOPT_HEADERFUNCTION, yuki::http::callbacks::header_callback);
	curl_easy_setopt(m_handle, CURLOPT_HEADERDATA, &rep);


	const auto code = curl_easy_perform(m_handle);
	if(code != CURLE_OK)
	{
		rep.code = -1;
	}
	else
	{
		if(curl_easy_getinfo(m_handle, CURLINFO_RESPONSE_CODE , &rep.code) != CURLE_OK)
		{
			throw std::runtime_error("Response code not supported");
		}
	}

	return rep;
}




yuki::http::response yuki::http::request::GET() const
{
	if(curl_easy_setopt(m_handle, CURLOPT_HTTPGET, 1L) != CURLE_OK)
	{
		throw std::runtime_error("HTTP not supported");
	}
	return execute_request();
}


yuki::http::response yuki::http::request::HEAD() const
{
	curl_easy_setopt(m_handle, CURLOPT_NOBODY, 1L);
	return execute_request();
}


yuki::http::response yuki::http::request::POST(const std::string & data ) const
{
	if(curl_easy_setopt(m_handle, CURLOPT_POST, 1L) != CURLE_OK)
	{
		throw std::runtime_error("HTTP not supported");
	}

	if(data != "")
	{
		curl_easy_setopt(m_handle, CURLOPT_POSTFIELDS, data.data());
		if(curl_easy_setopt(m_handle, CURLOPT_POSTFIELDSIZE, data.size()) != CURLE_OK)
		{
			throw std::runtime_error("HTTP not supported");
		}
	}
	return execute_request();
}


yuki::http::response yuki::http::request::DELETE() const
{
	if(curl_easy_setopt(m_handle, CURLOPT_CUSTOMREQUEST, "DELETE") != CURLE_OK)
	{
		throw std::runtime_error("DELETE not supported");
	}
	return execute_request();
}


yuki::http::response yuki::http::request::POST(const yuki::http::mime& mime_data) const
{
	curl_easy_setopt(m_handle, CURLOPT_POST, 1L);
	yuki::http::mime_wrapper curl_mime_data(m_handle);
	for(auto& part : mime_data.m_parts)
	{
		part->set_up_handle(curl_mime_data.get_handle());
	}
	curl_easy_setopt(m_handle, CURLOPT_MIMEPOST, curl_mime_data.get_handle());
	return execute_request();
}















































