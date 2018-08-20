#include "yukihttp/mime.hpp"

#include <string>
#include <vector>
#include <memory>

#include "mime_part.hpp"


void yuki::http::mime::add_data_part(const std::string & name, const std::string& data)
{
	m_parts.emplace_back(std::make_unique<yuki::http::raw_part>(name, data));
}

void yuki::http::mime::add_file_part(const std::string& name, const std::string& content, const std::string& file_name)
{
	m_parts.emplace_back(std::make_unique<yuki::http::file_data_part>(name, file_name, content));
}

void yuki::http::mime::add_file_part(const std::string& name, const std::string& path)
{
	m_parts.emplace_back(std::make_unique<yuki::http::file_part>(name, path));
}
