#version 300 es

precision highp float;

uniform vec2 u_mouse;
uniform vec2 u_resolution;
uniform float u_time;

out vec4 fragColor;

float lineSegment(vec2 p, vec2 a, vec2 b, vec2 thickness) {
    vec2 pa = p - a;
    vec2 ba = b - a;
    float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
    float line = smoothstep(thickness.x, thickness.y, length(pa - ba * h));

    return line;
}

float circle(in vec2 _st, in float _radius){
    vec2 dist = _st-vec2(0.5);
	return 1.-smoothstep(_radius-(_radius*0.01),
                         _radius+(_radius*0.01),
                         dot(dist,dist)*4.0);
}

vec2 displayCenter(vec2 resolution, bool adjustScreen){
    vec2 uv = gl_FragCoord.xy/resolution;
    uv = uv*0.2 - 0.1;

    if(adjustScreen){
        uv *= resolution.x / resolution.y;
    }
    return uv;
}

vec3 hsl2rgb(vec3 c) {
    vec3 rgb = clamp(abs(mod(c.x * 6.0 + vec3(0.0, 4.0, 2.0), 6.0) - 3.0) - 1.0, 0.0, 1.0);
    return c.z + c.y * (rgb - 0.5) * (1.0 - abs(2.0 * c.z - 1.0));
}

vec3 colorPicker(vec2 size){
    vec2 canva = gl_FragCoord.xy/size;

    if(canva.x > 1.0 || canva.y > 1.0){
        return vec3(0.0);
    }
    vec3 color = hsl2rgb(vec3(canva.x, canva.y, 0.5));

    return color;
}

void main(){
    float snow = 0.0;
    float gradient = (1.0-float(gl_FragCoord.y / u_resolution.x))*0.4;
    float random = fract(sin(dot(gl_FragCoord.xy,vec2(12.9898,78.233)))* 43758.5453);
    for(int k=0;k<6;k++){
        for(int i=0;i<12;i++){
            float cellSize = 2.0 + (float(i)*3.0);
			float downSpeed = 0.3+(sin(u_time*0.4+float(k+i*20))+1.0)*0.00008;
            vec2 uv = (gl_FragCoord.xy / u_resolution.x)+vec2(0.01*sin((u_time+float(k*6185))*0.6+float(i))*(5.0/float(i)),downSpeed*(u_time+float(k*1352))*(1.0/float(i)));
            vec2 uvStep = (ceil((uv)*cellSize-vec2(0.5,0.5))/cellSize);
            float x = fract(sin(dot(uvStep.xy,vec2(12.9898+float(k)*12.0,78.233+float(k)*315.156)))* 43758.5453+float(k)*12.0)-0.5;
            float y = fract(sin(dot(uvStep.xy,vec2(62.2364+float(k)*23.0,94.674+float(k)*95.0)))* 62159.8432+float(k)*12.0)-0.5;

            float randomMagnitude1 = sin(u_time*2.5)*0.7/cellSize;
            float randomMagnitude2 = cos(u_time*2.5)*0.7/cellSize;

            float d = 5.0*distance((uvStep.xy + vec2(x*sin(y),y)*randomMagnitude1 + vec2(y,x)*randomMagnitude2),uv.xy);

            float omiVal = fract(sin(dot(uvStep.xy,vec2(32.4691,94.615)))* 31572.1684);
            if(omiVal<0.08?true:false){
                float newd = (x+1.0)*0.4*clamp(1.9-d*(15.0+(x*6.3))*(cellSize/1.4),0.0,1.0);
                snow += d<(0.08+(x*0.3))/(cellSize/1.4)?
                    newd
                    :newd;
                // snow += newd;
            }
        }
    }


    fragColor = vec4(snow)+gradient*vec4(0.4,0.8,1.0,0.0) + random*0.11;
    fragColor.xyz = fragColor.xyz*0.1;
}