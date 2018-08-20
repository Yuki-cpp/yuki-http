#include "utils.hpp"

#include <string>
#include <algorithm>

#include <boost/algorithm/string.hpp>

#include "yukihttp/request.hpp"


std::size_t write_callback(void *data, std::size_t size [[maybe_unused]], std::size_t data_size, void *user_data)
{
	if(data and user_data)
	{
		yuki::http::response* response = reinterpret_cast<yuki::http::response*>(user_data);
		response->body += std::string(reinterpret_cast<char*>(data), data_size);
		return (data_size);
	}
	return 0;
}


std::size_t header_callback(void *data, std::size_t size, std::size_t data_size, void *user_data)
{
	if(data and user_data)
	{
		const auto total_size = data_size * size;

		std::string header_line(reinterpret_cast<char*>(data), total_size);
		boost::algorithm::trim(header_line);

		if(header_line == "")
		{
			return total_size;
		}

		std::string header_key = header_line;
		std::string header_value = "";

		const auto separator_index = header_line.find_first_of(':');
		if(separator_index != std::string::npos)
		{
			header_key = boost::algorithm::trim_copy(header_line.substr(0, separator_index));
			header_value = boost::algorithm::trim_copy(header_line.substr(separator_index + 1));
		}

		yuki::http::response* response = reinterpret_cast<yuki::http::response*>(user_data);
		response->headers[header_key] = header_value;

		return (total_size);
	}
	return 0;
}











































