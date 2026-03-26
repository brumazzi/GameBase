#version 300 es

precision mediump float;

uniform vec2 u_mouse;
uniform vec2 u_resolution;
uniform float u_time;

out vec4 fragColor;

void main(){
    vec2 uv = ( gl_FragCoord.xy - 0.5* u_resolution.xy ) / u_resolution.y;
    vec2 mouse = vec2(u_mouse.x/u_resolution.x*1.75-0.37, 1.0-u_mouse.y/u_resolution.y);

    float position = length(uv - (mouse - vec2(0.5, 0.5)))+0.1;
    float circle = smoothstep(0.4, 0.3, position);

    fragColor = vec4(vec3(circle), position*sin(1.14))*5.0 - vec4(1.0);
}