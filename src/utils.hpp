#pragma once

#include <cstddef>


/**
* @brief Namespace used to define callbacks used internaly bu cURL
* 
*/
namespace yuki::http::callbacks
{

/**
* @brief Function passed as callback to cURL to receive data.
*
* This function receive data from cURL and fills a yuki::http::response object with it.
*
* @warning Headers are received via the header_callback(void *data, std::size_t size, std::size_t data_size, void *user_data) function.
*
* @param data p_data: Data to write into the response.
* @param size p_size: Unused. cURL always set it to 1
* @param data_size p_data_size: Data size in bytes.
* @param user_data p_user_data: Pointer to the object that will receive the data. \a must be pointing to a yuki::http::response object.
* @return std::size_t : Number of bytes read or 0 in case of error.
*/
std::size_t write_callback(void *data, std::size_t size, std::size_t data_size, void *user_data);



/**
* @brief Function passed as callback to cURL to receive headers.
*
* This function receive headers from cURL and fills a yuki::http::response object with it.
* It processes a single header line and is called by cURL as many time as needed.
*
* @warning data is received via the write_callback(void *data, std::size_t size, std::size_t data_size, void *user_data) function.
*
* @param data p_data: Header line in the forme of key:value or key.
* @param size p_size: Size of an element in bytes.
* @param data_size p_data_size: Number of elements.
* @param user_data p_user_data: Pointer to the object to which append the header. \a must be pointing to a yuki::http::response object.
* @return std::size_t : \a Must always return 0.
*/
std::size_t header_callback(void *data, std::size_t size, std::size_t data_size, void *user_data);

}
