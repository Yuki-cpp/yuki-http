#pragma once

#include <string>

#include <curl/curl.h>

namespace yuki::http
{


class part
{
public:
	explicit part(const std::string & name);
	part(const part&) = delete;
	part& operator=(const part&) = delete;
	part(part&&) = default;
	part& operator=(part && ) = default;
	virtual ~part() = default;

	void set_up_handle(curl_mime* mime_handle) const;

protected:
	virtual void set_up_handle_impl(curl_mimepart* handle) const = 0;

private:
	std::string m_name;
};



class file_part : public part
{
public:
	file_part(const std::string & name, const std::string & file_name);

protected:
	void set_up_handle_impl(curl_mimepart* handle) const override;

private:
	std::string m_file_name;
};



class file_data_part : public part
{
public:
	file_data_part(const std::string & name, const std::string & file_name, const std::string & file_content);

protected:
	void set_up_handle_impl(curl_mimepart* handle) const override;

private:
	std::string m_file_name;
	std::string m_file_content;
};


class raw_part : public part
{
public:
	raw_part(const std::string & name, const std::string & data);

protected:
	virtual void set_up_handle_impl(curl_mimepart* handle) const override;

private:
	std::string m_data;
};




















































}
