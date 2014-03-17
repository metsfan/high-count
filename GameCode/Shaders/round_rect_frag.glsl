precision highp float;

varying vec2 v_texCoord;

uniform float u_cornerRadius;
uniform vec4 u_color;
uniform vec2 u_size;

void main()
{
    gl_FragColor = u_color;
}