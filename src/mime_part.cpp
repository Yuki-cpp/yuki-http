#include "mime_part.hpp"

#include <string>
#include <stdexcept>

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
	if(not handle)
	{
		throw std::runtime_error("Impossible to add a part to the curl mime object");
	}

	if(auto err = curl_mime_name(handle, m_name.c_str()); err != CURLE_OK)
	{
		throw std::runtime_error("Impossible to set the curl mime part name. Error : " + std::to_string(err));
	}
	set_up_handle_impl(handle);
}


void yuki::http::file_part::set_up_handle_impl(curl_mimepart* handle) const
{
	if(auto err = curl_mime_filedata(handle, m_file_name.c_str()); err != CURLE_OK)
	{
		throw std::runtime_error("Impossible to set the curl mime part filedata. Error : " + std::to_string(err));
	}
}


void yuki::http::file_data_part::set_up_handle_impl(curl_mimepart* handle) const
{
	if(auto err = curl_mime_data(handle, m_file_content.data(), m_file_content.size()); err != CURLE_OK)
	{
		throw std::runtime_error("Impossible to set the curl mime part file data. Error : " + std::to_string(err));
	}

	if(auto err = curl_mime_filename(handle, m_file_name.c_str()); err != CURLE_OK)
	{
		throw std::runtime_error("Impossible to set the curl mime part file name. Error : " + std::to_string(err));
	}
}

void yuki::http::raw_part::set_up_handle_impl(curl_mimepart* handle) const
{
	if(auto err = curl_mime_data(handle, m_data.data(), m_data.size()); err != CURLE_OK)
	{
		throw std::runtime_error("Impossible to set the curl mime part raw data. Error : " + std::to_string(err));
	}
}
