#include "utils.hpp"


#include <string>
#include <algorithm>

#include "yukihttp/request.hpp"


namespace
{

std::string &left_trim(std::string & s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](char c)
	{
		return not std::isspace(c);
	}));
	return s;
}

std::string &right_trim(std::string & s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](char c)
	{
		return not std::isspace(c);
	}).base(), s.end());
	return s;
}

std::string &trim(std::string & s)
{
	return left_trim(right_trim(s));
}

}


std::size_t write_callback(void *data, std::size_t size, std::size_t nmemb, void *userdata)
{
	yuki::http::response* r;
	const auto total_size = nmemb * size;
	r = reinterpret_cast<yuki::http::response*>(userdata);
	r->body += std::string(reinterpret_cast<char*>(data), total_size);
	return (total_size);
}


std::size_t header_callback(void *data, std::size_t size, std::size_t nmemb, void *userdata)
{
	yuki::http::response* r;
	r = reinterpret_cast<yuki::http::response*>(userdata);

	const auto total_size = nmemb * size;
	std::string header(reinterpret_cast<char*>(data), total_size);



	std::size_t sep_index = header.find_first_of(':');
	if ( std::string::npos == sep_index )
	{
		trim(header);
		if (header == "")
		{
			return (total_size);
		}
		r->headers[header] = "present";
	}
	else
	{
		std::string key = header.substr(0, sep_index);
		trim(key);
		std::string value = header.substr(sep_index + 1);
		trim(value);
		r->headers[key] = value;
	}

	return (total_size);
}

