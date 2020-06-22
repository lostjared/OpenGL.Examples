#version 410

layout (location = 0) in vec3 position;

uniform mat4 m_matrix;
uniform mat4 proj_matrix;
uniform mat4 v_matrix;
uniform float tf;


out vec4 varyingColor;

mat4 buildTranslate(float x, float y, float z) {
    mat4 trans = mat4(1.0, 0.0,0.0,0.0,
                      0.0,1.0,0.0,0.0,
                      0.0,0.0,1.0,0.0,
                      x,y,z,1.0
                      );
    return trans;
}

mat4 buildRotateX(float rad) {
    mat4 xrot = mat4(1.0, 0.0, 0.0, 0.0,
                     0.0, cos(rad), -sin(rad),0.0,
                     0.0, sin(rad), cos(rad), 0.0,
                     0.0, 0.0, 0.0, 1.0);
    return xrot;
}

mat4 buildRotateY(float rad) {
    mat4 xrot = mat4(cos(rad), 0.0, sin(rad), 0.0,
                     0.0, 1.0, 0.0,0.0,
                     -sin(rad), 0.0, cos(rad), 0.0,
                     0.0, 0.0, 0.0, 1.0);
    return xrot;
}

mat4 buildRotateZ(float rad) {
    mat4 xrot = mat4(cos(rad), -sin(rad), 0.0, 0.0,
                     sin(rad), cos(rad), 0.0,0.0,
                     0.0, 0.0, 1.0, 0.0,
                     0.0, 0.0, 0.0, 1.0);
    return xrot;
}

mat4 buildScale(float x, float y, float z) {
    mat4 scale = mat4(x,0.0,0.0,0.0,
                      0.0,y,0.0,0.0,
                      0.0,0.0,z,0.0,
                      0.0,0.0,0.0,1.0);
    return scale;
}

void main(void) {
    float i = gl_InstanceID + tf;
    float a = sin(2.0 * i) * 8.0;
    float b = sin(3.0 * i) * 8.0;
    float c = sin(4.0 * i) * 8.0;
    
    mat4 localX = buildRotateX(1.75*i);
    mat4 localY = buildRotateY(1.75*i);
    mat4 localZ = buildRotateZ(1.75*i);
    
    mat4 local_trans = buildTranslate(a,b,c);
    mat4 new_matrix = local_trans * localX * localY * localZ;
    
    mat4 mv_matrix = v_matrix * new_matrix;
    gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);
    
    varyingColor = vec4(position,1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 1.0);
}

