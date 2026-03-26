#version 130

uniform float time;         // tempo para animação
uniform vec2  resolution;   // window.getSize()

// Função de noise simples (pseudo-Perlin, barata)
float hash(vec2 p) {
    return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453);
}

float noise(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);
    f = f * f * (3.0 - 2.0 * f);  // smoothstep

    float a = hash(i + vec2(0.0, 0.0));
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));

    return mix(mix(a, b, f.x), mix(c, d, f.x), f.y);
}

float fbm(vec2 p) {  // Fractional Brownian Motion = várias camadas de noise
    float value = 0.0;
    float amplitude = 0.5;
    for (int i = 0; i < 5; i++) {
        value += amplitude * noise(p);
        p *= 2.0;          // frequência dobra a cada camada
        amplitude *= 0.5;  // amplitude cai pela metade
    }
    return value;
}

void main() {
    vec2 uv = gl_FragCoord.xy / resolution.xy;
    uv.y = 1.0 - uv.y;  // inverte Y se quiser céu em cima

    // Movimento lento das nuvens (direita → esquerda ou vice-versa)
    vec2 cloudUV = uv * vec2(3.0, 1.5) + vec2(time * 0.03, time * 0.01);

    // Gera nuvens com camadas
    float clouds = fbm(cloudUV * 5.0 + vec2(0.0, sin(time * 0.1) * 0.1));
    clouds += fbm(cloudUV * 4.0 + vec2(time * 0.05, 0.0)) * 0.5;
    clouds = smoothstep(0.35, 0.75, clouds);  // threshold para forma de nuvens

    // Cor do céu (gradiente simples)
    vec3 skyBlue = mix(vec3(0.5, 0.8, 1.0), vec3(0.1, 0.4, 0.9), uv.y);
    vec3 cloudColor = vec3(1.0, 1.0, 1.0);  // branco puro, ou ajuste para cinza

    // Mistura nuvens com céu
    vec3 finalColor = mix(cloudColor, skyBlue, clouds * 2);

    // Opcional: leve sombreamento nas bordas das nuvens
    finalColor *= 1.0 - clouds * 0.15 * (1.0 - uv.y);

    gl_FragColor = vec4(finalColor, 1.0);
}