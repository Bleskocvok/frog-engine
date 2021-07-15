#version 450


layout(binding = 0) uniform sampler2D tex;

layout(location = 4) uniform vec4 color_mix;

layout(location = 0) in vec2 fs_tex_coord;

layout(location = 0) out vec4 color;


void main()
{
    vec4 tex_color = texture(tex, fs_tex_coord);

    color = tex_color * color_mix;
}
