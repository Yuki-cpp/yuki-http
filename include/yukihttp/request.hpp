#pragma once

#include <iosfwd>
#include <functional>
#include <map>
#include <string>


namespace yuki::http
{

/**
* @brief Upload/Download progress callback.
*
* Defines the signature of a callback usable as a progress callback.
* The argument are in order :
* \li The number of bytes already downloaded
* \li The expected number of bytes to download
* \li The number of bytes already upload
* \li The expected number of bytes to upload
*/
using progress_callback = std::function<void(long, long, long, long)>;

/**
* @brief Alias for headers structure
*
*/
using headers_t = std::map<std::string, std::string>;

class mime;


/**
* @brief A HTTP(s) response
*
* \todo Fix error reporting. Separate cURL error to HTTP errors
*/
struct response
{
	/*! Return code of the response. 200 if no error, -1 for cURL related errors */
	int code = 0;

	/*! Body of the HTTP response */
	std::string body = "";

	/*! Headers of the HTTP response  */
	yuki::http::headers_t headers;
};


/**
* @brief HTTP(S) request
*
* Class used to perform HTTP(S) requests via cURL. Meant to be used as a REST client it only provides the following requests :
* \li GET
* \li HEAD
* \li DELETE
* \li POST
*
*/
class request
{
public:

	/**
	*
	* @param url : URL on which the request should be performed.
	*/
	explicit request(const std::string & url);

	~request();

	request(const request&) = delete;
	request& operator=(const request&) = delete;
	request(request&&) = default;
	request& operator=(request && ) = default;

	/**
	* @brief Enable or disable verbose mode.
	*
	* If verbose mode is enabled, debug information provided by cURL will be written on the stream provided to set_verbose_output(std::ostream & out) const.
	* If set_verbose_output(std::ostream & out) const isn't called, then the output is printed to stdcerr instead.
	*
	* @param is_verbose : true if verbose mode should be enable, false if not.
	*/
	void set_verbose(bool is_verbose) const;
	
	/**
	* @brief Set up the verbose output stream.
	* 
	* @param out : Stream to use for verbose output. 
	*/
	void set_verbose_output(std::ostream & out) const;

	void enable_progress_meter(bool is_enabled) const;
	void set_progress_callback(yuki::http::progress_callback & callback) const;

	
	/**
	* @brief Set the timeout for request.
	* 
	* @param ms : Timeout duration in milliseconds.
	*/
	void set_timeout(unsigned int ms) const;

	/**
	* @brief Set the user agent used for the request.
	* 
	* @param ua : User agent to use.
	*/
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
