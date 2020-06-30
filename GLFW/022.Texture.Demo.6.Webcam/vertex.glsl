#version 410

layout (location=0) in vec3 pos;
layout (location=1) in vec2 texCoord;
out vec2 tc;
out float alpha_r;
out float alpha_g;
out float alpha_b;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform float value_alpha_r, value_alpha_g, value_alpha_b;
uniform int index;
uniform float tf;


uniform sampler2D samp;

void main(void)
{    gl_Position = proj_matrix * mv_matrix * vec4(pos,1.0);
    tc = texCoord;
    alpha_r = value_alpha_r;
    alpha_g = value_alpha_g;
    alpha_b = value_alpha_b;
}
