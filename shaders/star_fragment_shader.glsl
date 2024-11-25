#version 330 core
out vec4 FragColor;
uniform float u_time;

void main() {
    // Створюємо круглу форму зірки
    vec2 circCoord = 2.0 * gl_PointCoord - 1.0;
    float alpha = 1.0 - length(circCoord);

    // Додаємо різні кольори для зірок на основі їх position
    float hue = fract(gl_FragCoord.x * 0.01 + gl_FragCoord.y * 0.01);
    vec3 baseColor;

    // Визначаємо різні кольори зірок
    if (hue < 0.2) {
        baseColor = vec3(1.0, 0.8, 0.8); // Червонуваті
    } else if (hue < 0.4) {
        baseColor = vec3(0.8, 0.8, 1.0); // Блакитні
    } else if (hue < 0.6) {
        baseColor = vec3(1.0, 1.0, 0.8); // Жовтуваті
    } else if (hue < 0.8) {
        baseColor = vec3(0.9, 0.9, 1.0); // Білі з голубим відтінком
    } else {
        baseColor = vec3(1.0); // Білі
    }

    // Додаємо мерехтіння
    float time = u_time + gl_FragCoord.x * 0.05 + gl_FragCoord.y * 0.05;
    float twinkle = 0.7 + 0.3 * sin(time + fract(gl_FragCoord.x * 0.1) * 6.28);

    // Створюємо світіння навколо зірки
    float glow = smoothstep(0.0, 1.0, alpha);

    if (alpha < 0) discard;
    FragColor = vec4(baseColor * twinkle, glow);
}