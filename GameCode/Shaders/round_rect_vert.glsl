// Vertex shader for a grid square with rounded corners

attribute vec2 a_position;

varying vec2 v_texCoord;

uniform mat4 u_mvp;

void main()
{
    //v_texCoord = a_position;
    
    gl_Position = u_mvp * vec4(a_position, 0.0, 1.0);
}


