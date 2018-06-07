#include <iostream>
#include <yukihttp/yukihttp.hpp>

int main()
{
	yuki::http::request req("https://example.com");
	req.set_user_agent("yukiHTTP-Client");
	req.set_header("Accept", "text/html");

	auto result =  req.GET();
	std::cout << result.code << "\n" << result.body << std::endl;

	return 0;
}


