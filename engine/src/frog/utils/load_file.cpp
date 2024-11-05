#include "load_file.hpp"


std::string frog::load_file(const std::string& filename)
{
    auto in = std::ifstream{ filename };
    if (!in)
    {
        throw std::runtime_error("file not found: '" + filename + "'");
    }

    std::string result;
    in.seekg(0, std::ios::end);
    result.reserve(in.tellg());
    in.seekg(0, std::ios::beg);

    using it = std::istreambuf_iterator<char>;
    result.assign(it{ in }, it{});
    return result;
}
