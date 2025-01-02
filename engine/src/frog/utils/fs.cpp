#include "fs.hpp"

#ifdef NOT_FROG_BUILD_2D

#include <fstream>      // ifstream
#include <string>       // getline
#include <iterator>     // istreambuf_iterator
#include <stdexcept>    // runtime_error

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

#else

#include "frog/lib2d/bits.hpp"

std::string frog::load_file(const std::string& filename)
{
    return lib2d::fs::file_read(filename.c_str(), lib2d::fs::mode::Text);
}

#endif
