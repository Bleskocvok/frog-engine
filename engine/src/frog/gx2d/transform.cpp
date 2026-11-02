#ifndef NOT_FROG_BUILD_2D

#include "transform.hpp"

#include "frog/lib2d/utils/surface_editor.hpp"
#include "frog/geometry/vector.hpp"

namespace frog::gx2d::trans {

Surf CopyRect::operator()(Surf src)
{
    auto editor = frog::lib2d::utils::SurfaceEditor(src);

    auto dst = dst_pos.value_or(irect.pos);

    for (int y = 0; y < irect.size.y(); y++)
        for (int x = 0; x < irect.size.x(); x++)
        {
            auto delta = frog::geo::ivec2{ x, y };
            editor.write_pixel(dst + delta, r.read_pixel(irect.pos + delta));
        }

    return src;
}

Surf Outline::operator()(Surf src)
{
    auto reader = frog::lib2d::utils::SurfaceReader(src);
    auto size = reader.size();
    auto around = pixels;

    auto result = create(reader.size());
    auto editor = frog::lib2d::utils::SurfaceEditor(result);

    auto is_around = [&size, &reader](int d, int x, int y)
    {
        for (int i = std::max(x - d, 0); i <= std::min(x + d, size.x() - 1); i++)
        {
            for (int k = std::max(y - d, 0); k <= std::min(y + d, size.y() - 1); k++)
            {
                if (i == x && k == y)
                    continue;

                auto next = reader.read_pixel({ i, k });
                if (next.a() != 0)
                    return true;
            }
        }
        return false;
    };

    for (int y = 0; y < size.y(); y++)
        for (int x = 0; x < size.x(); x++)
        {
            auto c = reader.read_pixel({ x, y });
            // if (c.a() != 0)
            //     editor.write_pixel({ x, y }, c);
            // else
                if (is_around(around, x, y))
                editor.write_pixel({ x, y }, rgb);
        }

    return result;
}

} // namespace frog::gx2d::trans

#endif
