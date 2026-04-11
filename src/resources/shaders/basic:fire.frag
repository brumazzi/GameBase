#version 300 es

precision highp float;

uniform float u_time;         // tempo para animação
uniform vec2  u_resolution;   // window.getSize()
uniform vec2  u_position;   // window.getSize()

out vec4 fragColor;

// Simple pixelated torch shader written by Matej Vanco just for fun, June 2023.

// Shader global params

const float particleCount = 128.0;

const float speed = 1.0;
const float pixelSize = 0.015;
const float brightness = 0.001;

const float fireShift = 0.15;
const float fireShiftFrequency = 5.0;

const vec2 size = vec2(0.05, 0.45);
const float bend = 0.0;

const vec2 glow = vec2(0.001, 0.05);

const vec3 toColor = vec3(0.4,0.35,0.2);
const vec3 fromColor = vec3(0.9,0.2,0.1);

const float alpha = 1.0;

// Method shortcuts

#define saturate(x) clamp(x, 0., 1.)

#define Hash1(t) fract(cos(t * 124.97) * 248.842)-0.5;



vec3 Simulate(vec2 uv, float t)
{
	vec3 res = vec3(0.);

	for(float i = 0.; i < particleCount; i++)
	{
        // Current loop time
        float ct = fract(t + (i+1.) / particleCount);
        float seed = Hash1((i+1.) * (t - ct));

        // Direction
		vec2 dir = vec2(bend, size.y);
        dir.x += (cos(t * seed) * sin(ct * fireShiftFrequency)) * mix(.0, fireShift, log(ct));

        // Current brightness
		float cb = saturate(mix(brightness, 0.0, ct));
        // Current offset
        vec2 off = vec2(seed*size.x, 0.);

		res += mix(fromColor * abs(seed), toColor, ct) * smoothstep(glow.x, glow.y,
        cb / length((uv - off - (dir * ct))));
	}

	return res;
}
void main(){
	vec2 frag = ( gl_FragCoord.xy - (0.5 * u_resolution.xy)) / u_resolution.y;
	frag *= 4.0;

    vec2 posOffset = vec2(0.,-.2);
    frag -= posOffset;

	fragColor = vec4( Simulate(floor(frag / pixelSize) * pixelSize, (u_time+100.) * speed) * saturate(alpha), 1. );
	fragColor.a = fragColor.r;
}