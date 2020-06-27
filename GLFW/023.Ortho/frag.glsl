#version 410
in vec2 tc;
out vec4 color;
in float alpha_r;
in float alpha_g;
in float alpha_b;
in float current_index;
in float timeval;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform sampler2D samp;
uniform float value_alpha_r, value_alpha_g, value_alpha_b;
uniform float index_value;
uniform float time_f;



void main(void)
{
    color = texture(samp, tc);
    ivec3 source;
    for(int i = 0; i < 3; ++i) {
        source[i] = int(255 * color[i]);
    }
    color[0] += (color[0]*alpha_r);
    color[1] += (color[1]*alpha_g);
    color[2] += (color[2]*alpha_b);
    
    switch(int(current_index)) {
        case 0: {
            ivec3 int_color;
            for(int i = 0; i < 3; ++i) {
                int_color[i] = int(255 * color[i]);
                int_color[i] = int_color[i]^source[i];
                int_color[i] = int_color[i]%255;
                color[i] = float(int_color[i])/255;
            }
        }
            break;
        case 1:
            break;
        case 2: {
            ivec3 int_color;
            for(int i = 0; i < 3; ++i) {
                int_color[i] = int(255 * color[i]);
                int_color[i] = int_color[i]^source[i];
                int_color[i] = int_color[i]%255;
                color[i] = (0.3578*timeval) *  float(int_color[i])/255;
            }
        }
    }
}

