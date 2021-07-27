#version 450


layout(location = 3) uniform vec4 mat_ambi;
layout(location = 4) uniform vec4 mat_diff;
layout(location = 5) uniform vec4 mat_spec;

layout(binding = 0) uniform sampler2D mat_texture;

layout(location = 9) uniform vec4 sun_pos;
layout(location = 10) uniform vec3 eye_pos;

layout(location = 0) in vec3 fs_position;
layout(location = 1) in vec3 fs_normal;
layout(location = 2) in vec2 fs_tex_coord;

layout(location = 0) out vec4 color;


void main()
{
    vec3 sun_loc = sun_pos.xyz;

    vec3 ambi = mat_ambi.rgb;
    vec3 diff = mat_diff.rgb * texture(mat_texture, fs_tex_coord).rgb;
    vec3 spec = mat_spec.rgb;
    float shininess = mat_spec.w;

    // vectors
    vec3 n = normalize(fs_normal);
    vec3 l = normalize(sun_loc);
    vec3 v = normalize(eye_pos - fs_position);

    // blinn-phong
    vec3 halfv = normalize(l + normalize(v));
    float reflection = max(0.0001, dot(n, halfv));

    // phong
    // // vec3 r = normalize(2 * dot(n, l) * n - l);
    // vec3 r = normalize(l - 2 * dot(n, l) * n);

    // whether or not reflection should be visible: 1 or 0
    // float reflection_allow = (sign(dot(n, l)) + 1.0) * 0.5;
    // float reflection = max(0.0001, dot(v, r)) * reflection_allow;

    float diff_res = max(0, dot(n, l));
    float spec_res = max(0, pow(reflection, shininess));

    vec3 result = ambi + diff_res * diff + spec_res * spec;


    color = vec4(result, 1.0);
}
