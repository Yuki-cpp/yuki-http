#include "slist_wrapper.hpp"

#include <stdexcept>

void yuki::http::curl_slist_wrapper::append(const std::string & str)
{
	if(auto new_list = curl_slist_append(m_slist, str.c_str());  not new_list)
	{
		throw std::runtime_error("Impossible to add a string to the list.");
	}
	else
	{
		m_slist = new_list;
	}
}


yuki::http::curl_slist_wrapper::~curl_slist_wrapper()
{
	if(m_slist)
	{
		curl_slist_free_all(m_slist);
	}
}


curl_slist const * yuki::http::curl_slist_wrapper::get_list() const
{
	return m_slist;
}


