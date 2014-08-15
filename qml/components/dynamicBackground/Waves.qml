import QtQuick 2.2

Shader
{
  property vector3d color1: Qt.vector3d(0.0, 0.0, 0.3);
  property vector3d color2: Qt.vector3d(0.5, 0.0, 0.0);
  property real blockWidth: 0.01;

  fragmentCode: "
    uniform vec3 color1;
    uniform vec3 color2;

    uniform float blockWidth;

    void main(void)
    {
      vec2 uv = gl_FragCoord.xy / iResolution.xy;

      // To create the BG pattern
      vec3 final_color = vec3(1.0);
      vec3 bg_color = vec3(0.0);
      vec3 wave_color = vec3(0.0);

      float c1 = mod(uv.x, 2.0 * blockWidth);
      c1 = step(blockWidth, c1);

      float c2 = mod(uv.y, 2.0 * blockWidth);
      c2 = step(blockWidth, c2);

      bg_color = mix(uv.x * color1, uv.y * color2, c1 * c2);


      // To create the waves
      float wave_width = 0.01;
      uv  = -1.0 + 2.0 * uv;
      uv.y += 0.1;
      for(float i = 0.0; i < 10.0; i++) {

        uv.y += (0.07 * sin(uv.x + i/7.0 + iGlobalTime ));
        wave_width = abs(1.0 / (150.0 * uv.y));
        wave_color += vec3(wave_width * 1.9, wave_width, wave_width * 1.5);
      }

      final_color = bg_color + wave_color;


      gl_FragColor = vec4(final_color, 1.0);
    }
  "
}
