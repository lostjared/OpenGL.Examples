#version 410
in vec2 tc;
out vec4 color;
in float alpha_r;
in float alpha_g;
in float alpha_b;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform sampler2D samp;
uniform float value_alpha_r, value_alpha_g, value_alpha_b;


void main(void)
{
    color = texture(samp, tc);
    color[0] = (color[0]*alpha_r);
    color[1] = (color[1]*alpha_g);
    color[2] = (color[2]*alpha_b);
}
