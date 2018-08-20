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
* @brief HTTP(S) response
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
	* @param is_verbose : true if verbose mode should be enabled, false if not.
	*/
	void set_verbose(bool is_verbose) const;

	/**
	* @brief Set up the verbose output stream.
	*
	* @param out : Stream to use for verbose output.
	*/
	void set_verbose_output(std::ostream & out) const;

	/**
	* @brief Enable or disable the progress meter.
	*
	* If the progress meter is enabled, the yuki::http::progress_callback set up by set_progress_callback(yuki::http::progress_callback & callback) const will be called periodically as the transfer advances.
	* If set_progress_callback(yuki::http::progress_callback & callback) const isn't called, cURL progress callback is used instead.
	*
	* @param is_enabled : true if the progress meter should be enabled, false if not.
	*/
	void enable_progress_meter(bool is_enabled) const;


	/**
	* @brief Set up the callback used for the progress meter.
	*
	* @param callback : Callback handling the progress of the transfer.
	*/
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


	/**
	* @brief Set up an header for the request.
	*
	* Add a key:value pair to the headers of the request.
	* If the key already exists, the old value is overwritten.
	*
	* @param name : key of the header
	* @param value : value associated to the key
	*/
	void set_header(const std::string & name, const std::string & value);


	/**
	* @brief Execute a GET request on the URL set up by request(const std::string & url).
	*
	* @return yuki::http::response
	*/
	yuki::http::response GET() const;

	/**
	* @brief Execute a HEAD request on the URL set up by request(const std::string & url).
	*
	* @return yuki::http::response
	*/
	yuki::http::response HEAD() const;

	/**
	* @brief Execute a DELETE request on the URL set up by request(const std::string & url).
	*
	* @return yuki::http::response
	*/
	yuki::http::response DELETE() const;


	/**
	* @brief Execute a POST on the URL set up by request(const std::string & url).
	*
	* The POST data is directly given by the user. For mime request forms, see yuki::http::response POST(const yuki::http::mime & mime_data) const.
	*
	* @param data : POST field data
	* @return yuki::http::response
	*/
	yuki::http::response POST(const std::string & data) const;


	/**
	* @brief Execute a POST on the URL set up by request(const std::string & url).
	*
	* The POST data is generated from a provided MIME form.
	*
	* @param mime_data : mime data used to generate the request.
	* @return yuki::http::response
	*/
	yuki::http::response POST(const yuki::http::mime & mime_data) const;

private:
	/**
	* @brief Actually execute the request.
	*
	* Set up the headers based on m_headers_map and execute the request.
	*
	* @return yuki::http::response The response to the request.
	*/
	yuki::http::response execute_request() const;

	/**
	* @brief cURL handle
	*
	* This handle is managed by this class. Be careful with it.
	*/
	void* m_handle = nullptr;

	/**
	* @brief Headers to be used by the request.
	*
	*/
	yuki::http::headers_t m_headers_map;
};






}
