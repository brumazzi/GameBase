#version 130

uniform sampler2D texture;
uniform float     time;
uniform vec2      resolution;

// Parâmetros ajustáveis
const float flow_speed   = 0.35;   // velocidade do fluxo principal
const float wave_speed   = 1.8;
const float distortion   = 0.018;
const float foam_amount  = 0.4;

// Função de noise simples (para partículas "aleatórias")
float noise(vec2 p) {
    return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453);
}

float particle(vec2 uv, vec2 pos, float size, float speedMult) {
    vec2 delta = uv - pos;
    float dist = length(delta);
    float brightness = smoothstep(size, size * 0.3, dist);
    // Fade out com o tempo para simular movimento
    brightness *= (1.0 - fract(time * speedMult + noise(pos * 10.0) * 10.0));
    return brightness;
}

void main()
{
    vec2 uv = gl_TexCoord[0].xy;

    // Movimento do fluxo
    vec2 flow = vec2(0.0, -flow_speed * time);
    flow.x += sin(time * 0.4 + uv.y * 5.0) * 0.08;

    // Ondulações
    float wave1 = sin(uv.y * 12.0 + uv.x * 8.0 + time * wave_speed) * distortion;
    float wave2 = cos(uv.y * 15.0 - uv.x * 10.0 + time * wave_speed * 1.3) * distortion * 0.7;
    float wave  = wave1 + wave2;

    vec2 uv_flow = uv + flow + vec2(wave, wave * 1.5);

    vec4 color = texture2D(texture, uv_flow);

    // Cor de água
    color.r *= 0.80;
    color.g *= 0.95;
    color.b *= 1.12;

    // Espuma base (das ondas)
    float foam = sin((uv.x + uv.y) * 30.0 + time * 3.5) * 0.5 + 0.5;
    foam = smoothstep(0.45, 0.65, foam);
    foam *= (abs(wave1) + abs(wave2)) * foam_amount;
    color.rgb += vec3(foam * 0.9);

    // ────────────────────────────────────────────────
    // PARTÍCULAS (folhas, detritos, espuma flutuante)
    // ────────────────────────────────────────────────
    vec3 particles = vec3(0.0);

    // Gera várias partículas em posições "aleatórias" que se movem com o flow
    for (int i = 0; i < 12; i++) {  // 12 partículas – aumente se quiser mais (mas custa performance)
        float fi = float(i);
        vec2 seed = vec2(fi * 1.618, fi * 3.14);  // pseudo-random seeds

        // Posição base da partícula (repetitiva no espaço)
        vec2 basePos = vec2(
            fract(noise(seed) * 2.0 - 0.5 + time * 0.03 * (1.0 + fi * 0.1)),
            fract(noise(seed + 10.0) * 2.0 - 0.5)
        );

        // Move com o fluxo (mais rápido que a água para parecer flutuando por cima)
        vec2 particlePos = basePos + flow * 1.2 + vec2(sin(time * 0.7 + fi) * 0.04, 0.0);

        // Tamanho e velocidade variam por partícula
        float size  = 0.008 + noise(seed + 20.0) * 0.012;
        float speed = 1.2 + noise(seed + 30.0) * 0.8;

        float p = particle(fract(uv_flow), fract(particlePos), size, speed);

        // Cor da partícula: branco sujo para espuma/folha, ou marrom para detrito
        vec3 particleColor = mix(vec3(0.95, 0.92, 0.85), vec3(0.7, 0.6, 0.4), noise(seed + 40.0));
        particles += p * particleColor * (0.7 + sin(time * 2.0 + fi * 5.0) * 0.3);  // pisca levemente
    }

    // Adiciona as partículas por cima da cor base
    color.rgb += particles * 1.2;

    color *= gl_Color;
    gl_FragColor = color;
}