#version 330 core

/*----------------------------------------------------------------------------*/

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord;

/*----------------------------------------------------------------------------*/

uniform mat4 u_model_matrix;

/*----------------------------------------------------------------------------*/

out vec3 i_position;
out vec3 i_normal;
out vec2 i_tex_coord;

/*----------------------------------------------------------------------------*/

void main() {
    i_position = pos;
    i_normal = normal;
    i_tex_coord = tex_coord;
    gl_Position = u_model_matrix * vec4(pos, 1.0);
}

/*----------------------------------------------------------------------------*/
