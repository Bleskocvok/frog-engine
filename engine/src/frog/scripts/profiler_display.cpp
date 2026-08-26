#include "profiler_display.hpp"

#include "frog/core/engine2d.hpp"
#include "frog/core/script.hpp"
#include "frog/geometry/vector.hpp"
#include "frog/graphics/color.hpp"
#include "frog/graphics/ui_element.hpp"
#include "frog/gx2d/sprite.hpp"
#include "frog/scripts/profiler_script.hpp"
#include "frog/utils/assert.hpp"
#include "frog/utils/profiler_guard.hpp"
#include "frog/utils/ptr.hpp"

#include <algorithm>
#include <iomanip>
#include <map>
#include <sstream>
#include <string>
#include <utility>      // move
#include <vector>

using namespace frog::scripts;

static constexpr float WIDTH = 0.8;
static constexpr float LEFT_MARGIN = 0.03;
static constexpr ProfilerScript::Columns COLUMNS = { 30, 8, 8 };

void ProfilerDisplay::mk_bg(frog::game_object2d& obj, frog::scripts::ProfilerScript* profiler)
{
    auto count = profiler ? profiler->times().size() : 5;
    count = std::max(decltype(count)(5), count);

    auto* bg = obj.add_element(frog::mk_ptr<frog::gx::ui_element>());
    elems.push_back(bg);

    bg->sprite.image_tag = "box";
    bg->sprite.color = { 0, 0, 0, 128 };

    bg->sprite.rect.size = { WIDTH, label_height * count * 2 };
    bg->sprite.rect.pos = start;
    bg->sprite.rect.pos.y() += 0.5 * label_height * count;
}

struct Node
{
    frog::ProfilerGuard::Key key;
    frog::ProfilerGuard::Item item;

    std::vector<frog::ptr<Node>> children;

    Node(frog::ProfilerGuard::Key key_, frog::ProfilerGuard::Item item_)
        : key(std::move(key_))
        , item(std::move(item_))
    { }
};

void ProfilerDisplay::add_text(Ctx& ctx, std::string str)
{
    auto* ui = ctx.obj.add_element(frog::mk_ptr<frog::gx::ui_element>());
    elems.push_back(ui);

    ui->label = { "text:", 1 };
    ui->label->align = decltype(ui->label->align)::LEFT;
    ui->pos() = ctx.pos - frog::geo::vec2{ WIDTH / 2 - LEFT_MARGIN, 0 };
    ui->size() = { 0, label_height };
    // TODO: Fuck.
    ui->sprite.layer = 100000;

    ui->label->str = std::move(str);

    ctx.pos.y() += 0.025;
}

void ProfilerDisplay::mk_one(Ctx& ctx, const ProfilerGuard::Key& key, const auto& node)
{
    std::ostringstream o;
    auto bit = std::string("| ");
    for (int i = 0; i < ctx.indent; i++)
        o << bit;
    auto pair = std::pair<ProfilerGuard::Key, ProfilerGuard::Item>( key, node->item );
    // TODO: Better decomposition.
    auto columns = COLUMNS;
    std::get<0>(columns) -= ctx.indent * bit.length();

    ctx.profiler->out_line(o, pair, columns);

    add_text(ctx, std::move(o).str());

    ++ctx.indent;
    for (const auto& c : node->children)
        mk_one(ctx, c->key, c);

    --ctx.indent;
}

void ProfilerDisplay::mk_labels(frog::game_object2d& obj, frog::scripts::ProfilerScript* profiler)
{
    auto nodes = std::map<ProfilerGuard::Key, frog::ptr<Node>>{};
    auto ptrs = std::map<ProfilerGuard::Key, Node*>{};

    using Elem = std::pair<ProfilerGuard::Key, frog::ptr<Node>>;

    for (const auto& [key, item] : profiler->times())
    {
        auto ptr = frog::mk_ptr<Node>(key, item);
        ptrs.emplace(key, ptr.get());
        nodes.emplace(key, std::move(ptr));
    }

    for (const auto& [key, item] : profiler->times())
    {
        if (not key.second)
            continue;

        auto it = std::ranges::find_if(ptrs, [&key](const auto& elem)
                {
                    return elem.first.first == key.second.value();
                });
        if (it == ptrs.end())
            continue;

        frog_assert(it->second);

        auto& src = nodes.at(key);
        auto ptr = std::move(src);
        nodes.erase(key);

        it->second->children.push_back(std::move(ptr));
    }

    Ctx ctx
    {
        .indent = 0,
        .obj = obj,
        .pos = start,
        .profiler = profiler,
    };

    {
        auto [a, b, c] = COLUMNS;

        std::ostringstream o;
        o
            << std::right << std::setw(a) << "id"
            << " |"
            << std::right << std::setw(b) << "total ms"
            << " |"
            << std::right << std::setw(c) << "avg ms"
            ;
        add_text(ctx, std::move(o).str());
    }

    for (const auto& [key, node] : nodes)
    {
        mk_one(ctx, key, node);
    }

    // auto pos = start;
    // for (const auto& t : profiler->times())
    // {
    //     auto* ui = obj.add_element(frog::mk_ptr<frog::gx::ui_element>());
    //     elems.push_back(ui);
    //     ui->label = { "text:", 1 };
    //     ui->label->align = decltype(ui->label->align)::CENTER;
    //     ui->pos() = pos;
    //     ui->size() = { 0, label_height };
    //     // TODO: Fuck.
    //     ui->sprite.layer = 100000;
    //     std::ostringstream o;
    //     profiler->out_line(o, t);
    //     ui->label->str = frog::make_string(std::move(o).str());
    //     pos.y() += 0.025;
    // }

}

void ProfilerDisplay::frame_update(frog::game_object2d& obj, frog::engine2d& e)
{
    auto* profiler = e.scenes->current().get_script<frog::scripts::ProfilerScript>();
    if (not profiler)
        return;

    if (not profiler->changed())
        return;

    for (auto* elem : elems)
        obj.remove_element(elem);

    // TODO: Optimize this shit out of this crap.
    mk_bg(obj, profiler);

    mk_labels(obj, profiler);
}
