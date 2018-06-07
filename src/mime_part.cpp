#include "mime_part.hpp"

#include <string>

#include <curl/curl.h>

yuki::http::part::part(const std::string& name):
	m_name(name)
{}

yuki::http::raw_part::raw_part(const std::string& name, const std::string& data):
	part(name),
	m_data(data)
{}


yuki::http::file_part::file_part(const std::string& name, const std::string& file_name):
	part(name),
	m_file_name(file_name)
{}

yuki::http::file_data_part::file_data_part(const std::string& name, const std::string& file_name, const std::string& file_content):
	part(name),
	m_file_name(file_name),
	m_file_content(file_content)
{}



void yuki::http::part::set_up_handle(curl_mime* mime_handle) const
{
	curl_mimepart* handle = curl_mime_addpart(mime_handle);
	curl_mime_name(handle, m_name.c_str());
	set_up_handle_impl(handle);
}


void yuki::http::file_part::set_up_handle_impl(curl_mimepart* handle) const
{
	curl_mime_filedata(handle, m_file_name.c_str());
}


void yuki::http::file_data_part::set_up_handle_impl(curl_mimepart* handle) const
{
	curl_mime_data(handle, m_file_content.data(), m_file_content.size());
	curl_mime_filename(handle, m_file_name.c_str());
}

void yuki::http::raw_part::set_up_handle_impl(curl_mimepart* handle) const
{
	curl_mime_data(handle, m_data.data(), m_data.size());
}
