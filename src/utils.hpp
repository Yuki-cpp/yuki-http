#pragma once

#include <cstddef>

std::size_t write_callback(void *data, std::size_t size, std::size_t nmemb, void *userdata);

std::size_t header_callback(void *data, std::size_t size, std::size_t nmemb, void *userdata);
