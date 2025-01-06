#pragma once

#include <string>       // getline


namespace frog
{


std::string load_file(const std::string& filename);

void save_file(const std::string& filename, const std::string& content);


} // namespace frog
