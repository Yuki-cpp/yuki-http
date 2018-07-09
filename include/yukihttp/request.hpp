#pragma once

#include <iosfwd>
#include <functional>
#include <map>
#include <string>


namespace yuki::http
{

using progress_callback = std::function<void(long, long, long, long)>;

using headers_t = std::map<std::string, std::string>;

class mime;

struct response
{
	int code = 0;
	std::string body = "";
	yuki::http::headers_t headers;
};


class request
{
public:
	explicit request(const std::string & url);
	~request();

	request(const request&) = delete;
	request& operator=(const request&) = delete;
	request(request&&) = default;
	request& operator=(request && ) = default;

	void set_verbose(bool is_verbose) const;
	void set_verbose_output(std::ostream & out) const;

	void enable_progress_meter(bool is_enabled) const;
	void set_progress_callback(yuki::http::progress_callback & callback) const;

	void set_timeout(unsigned int ms) const;

	void set_user_agent(const std::string & ua) const;

	void set_auth_username(const std::string & username) const;
	void set_auth_password(const std::string & password) const;

	void set_header(const std::string & name, const std::string & value);

	yuki::http::response GET() const;
	yuki::http::response HEAD() const;
	yuki::http::response DELETE() const;
	yuki::http::response POST(const std::string & data) const;
	yuki::http::response POST(const yuki::http::mime & mime_data) const;

private:
	yuki::http::response execute_request() const;

	void* m_handle = nullptr;
	yuki::http::headers_t m_headers_map;
};






}
