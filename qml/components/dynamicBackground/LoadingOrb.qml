import QtQuick 2.2

Shader
{
  fragmentCode: "
    #define const

    float saturate(float color)
    {
      return clamp(color, 0.0, 1.0);
    }

    vec2 saturate(vec2 color)
    {
      return clamp(color, 0.0, 1.0);
    }

    vec3 saturate(vec3 color)
    {
      return clamp(color, 0.0, 1.0);
    }

    vec4 saturate(vec4 color)
    {
      return clamp(color, 0.0, 1.0);
    }

    void main(void)
    {
      vec2 glUV = gl_FragCoord.xy / iResolution.xy;
      vec4 cvSplashData = vec4(iResolution.x, iResolution.y, iGlobalTime, 0.0);
      vec2 InUV = glUV * 2.0 - 1.0;

      // Constants
      const float TimeElapsed		= cvSplashData.z;
      const float Brightness		= sin(TimeElapsed) * 0.2;
      const vec2 Resolution     = vec2(cvSplashData.x, cvSplashData.y);
      const float AspectRatio		= Resolution.x / Resolution.y;
      const vec3 InnerColor     = vec3( 0.50, 0.50, 0.50 );
      const vec3 OuterColor     = vec3( 0.05, 0.05, 0.90 );
      const vec3 WaveColor      = vec3( 0.70, 0.70, 1.00 );

      // Input
      vec2 uv           = (InUV + 1.0) / 2.0;

      // Output
      vec4 outColor			= vec4(0.0);

      // Positioning
      vec2 outerPos			= -0.5 + uv;
      outerPos.x				*= AspectRatio;

      vec2 innerPos			= InUV * ( 2.0 - Brightness );
      innerPos.x				*= AspectRatio;

      // logic
      float innerWidth      = length(outerPos);
      float circleRadius		= 0.24 + Brightness * 0.1;
      float invCircleRadius = 1.0 / circleRadius;
      float circleFade      = pow(length(2.0 * outerPos), 0.5);
      float invCircleFade		= 1.0 - circleFade;
      float circleIntensity	= pow(invCircleFade * max(1.1 - circleFade, 0.0), 2.0) * 60.0;
      float circleWidth     = dot(innerPos,innerPos);
      float circleGlow      = ((1.0 - sqrt(abs(1.0 - circleWidth))) / circleWidth) + Brightness * 0.5;
      float outerGlow       = min( max( 1.0 - innerWidth * ( 1.0 - Brightness ), 0.0 ), 1.0 );
      float waveIntensity		= 0.0;

      // Inner circle logic
      if( innerWidth < circleRadius )
      {
        circleIntensity		*= pow(innerWidth * invCircleRadius, 24.0);

        float waveWidth		= 0.05;
        vec2 waveUV		= InUV;

        for(float i = 0.0; i < 3.0; i++)
        {
          waveUV.y		+= 0.4 * (0.20 * cos((waveUV.x * 2.0) + (i / 7.0) + (TimeElapsed * 1.5)));
          waveWidth		= abs(1.0 / (200.0 * waveUV.y));
          waveIntensity 	+= waveWidth;
        }
      }

      // Compose outColor
      outColor.rgb	= outerGlow * OuterColor;
      outColor.rgb	+= circleIntensity * InnerColor * 2.0;
      outColor.rgb	+= circleGlow * InnerColor * (0.6 + Brightness * 1.2);
      outColor.rgb	+= WaveColor * waveIntensity;
      outColor.a		= 1.0;

      // Fade in
      outColor.rgb	= saturate(outColor.rgb);
      outColor.rgb	*= min(TimeElapsed / 2.0, 1.0);

      //////////////////////////////////////////////////////////////
      // Start of ShaderToy Output Compat
      //////////////////////////////////////////////////////////////

      gl_FragColor = outColor;
    }
  "
}
