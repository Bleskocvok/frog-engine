#version 450


layout(location = 0) uniform vec2 position;
layout(location = 1) uniform vec2 size;

layout(location = 2) uniform vec2 tex_pos;
layout(location = 3) uniform vec2 tex_size;


layout(location = 0) out vec2 fs_tex_coord;


const vec2 coords[6] =
{
    vec2(0, 0), vec2(1, 0), vec2(0, 1),
    vec2(1, 0), vec2(1, 1), vec2(0, 1),
};


void main()
{
    vec2 scaled = position + size * coords[gl_VertexID];

    vec2 converted = vec2(coords[gl_VertexID].x,
                          1 - coords[gl_VertexID].y);
    fs_tex_coord = tex_pos + tex_size * converted;

    gl_Position = vec4(scaled.x, scaled.y, 0, 1);
}
