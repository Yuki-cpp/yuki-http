#pragma once

#include <string>
#include <vector>
#include <memory>

namespace yuki::http
{


class part;

class mime
{
	friend class request;

public:
	mime() = default;
	mime(const mime&) = delete;
	mime& operator=(const mime&) = delete;
	mime(mime&&) = default;
	mime& operator=(mime && ) = default;
	~mime() = default;

	void add_data_part(const std::string & name, const std::string & data);
	void add_file_part(const std::string & name, const std::string & content, const std::string & file_name);
	void add_file_part(const std::string & name, const std::string & path);

private:
	std::vector<std::unique_ptr<yuki::http::part>> m_parts;
};




}
