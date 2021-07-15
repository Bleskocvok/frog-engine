#version 450


layout(location = 0) uniform mat4 proj;
layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 model;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex_coord;

layout(location = 0) out vec3 fs_position;
layout(location = 1) out vec3 fs_normal;
layout(location = 2) out vec2 fs_tex_coord;


void main()
{
    // coordinates are left unchanged
    fs_tex_coord = tex_coord;

    // normals are relative to the model
    // but we need to know their absolute position
    mat4 normal_mat = transpose(inverse(model));
    fs_normal = vec3(normal_mat * vec4(normal, 1));

    // position needed to calculate specular light
    fs_position = vec3(model * vec4(position, 1.0));

    // the standard procedure for coordinate calculation
    gl_Position = proj * view * model * vec4(position, 1.0);
}
