import QtQuick 2.2

Shader
{
  property real speed: 10.0;
  property real zoom: 10.0;
  property real finalTone: 1.0;

  iGlobalTimeIncrement: 0.01;

  fragmentCode: "
    uniform float speed;
    uniform float zoom;
    uniform float finalTone;

    #define FIX_ASPECT
    #define VIGNETTING
    #define BOXSTENCIL
    #define ROTATE
    #define MEGADETHLAZORS

    void main(void)
    {
      // uvset [0,1]
      vec2 uv = gl_FragCoord.xy / iResolution.xy;
      // [0,1] to [-1,1]
      vec2 p = 2.0 * uv - 1.0;

      // force 1:1 aspect ratio if defined
    #ifdef FIX_ASPECT
      p.x *= iResolution.x/iResolution.y;
    #endif
      float t = iGlobalTime * speed;
      vec3 color = vec3(0.0);
      float lp = length(p);


      // performance no/no
    #ifdef ROTATE
      float angle = (t/speed)*0.5;
      p *= mat2(cos(angle),-sin(angle),sin(angle),cos(angle));
    #endif

      vec2 zp = p * zoom;
      vec2 zp2 = 0.2*zp;
    #if 1
      // the outer bars
      float ofs = 1.0/cos(p.y);
      color.b = clamp(1.0,0.0,(5.0*sin(zp.x * ofs + t))-2.0);
      color.b += clamp(1.0,0.0,(3.0*sin(0.9*zp.x * ofs + t*0.6))-2.0);

      // the inner bulbe
      color.g += (1.0 - length(p * vec2(1.0,1.5))) * -color.r;
      //color.rgb += color.r;
      //color.rgb = color.r * vec3(0.5,0.5,0.5);
    #endif

      // box stencil / force nova
    #ifdef BOXSTENCIL
      float stencil = ceil(1.0-clamp(1.0,0.0,lp));
      float boxstencil = stencil * length(p * sin(zp2.x) * sin(0.4*t-zp2.y));
      boxstencil = clamp(1.0,0.0,boxstencil);
      boxstencil += -1.4+sin(p.y+t*0.2)*2.0;
      color *= (0.6*boxstencil*10.0*(1.0-lp));
    #endif


      color = vec3(color.b*0.3,color.b*0.7,color.b*3.0);
      color += length(p);

      // vignetting
    #ifdef VIGNETTING
      color *= 1.0-clamp(1.0,0.0,length(p));
    #endif

    #ifdef MEGADETHLAZORS
      color = (color+length(p))*color.r;
    #endif


      //clamp the color
      color = clamp(vec3(1.0),vec3(0.0),color);

      // output final color
      gl_FragColor = vec4(color,1.0)*finalTone;
    }
  "
}
