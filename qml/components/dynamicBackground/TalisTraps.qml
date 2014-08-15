import QtQuick 2.2

Shader
{
  property vector2d basepos: Qt.vector2d(-0.42,0.008);
  property real scale: -0.55;
  property int iterations: 28;
  property real orbittrap: 2.0;
  property real zoom: 150.0;
  property real brightness: 0.85;
  property real saturation: 0.5;
  property real antialias: 4.0;

  fragmentCode: "
    //parameters
    uniform vec2 basepos;
    uniform float scale;
    uniform int iterations;
    uniform float orbittrap;
    uniform float zoom;
    uniform float brightness;
    uniform float saturation;
    uniform float antialias;

    vec2 rotate(vec2 p, float angle) {
    return vec2(p.x*cos(angle)-p.y*sin(angle),
           p.y*cos(angle)+p.x*sin(angle));
    }

    void main(void)
    {
      vec3 aacolor=vec3(0.);
      vec2 uv=gl_FragCoord.yx / iResolution.yx - 0.5;
      vec2 pos=uv;
      float aspect=iResolution.x/iResolution.y;
      pos.y*=aspect;
      float t=iGlobalTime*.05;
      float zz=zoom+sin(t)*80.;
      pos/=zz;
      vec2 pixsize=1./zz/iResolution.yx;
      pixsize.y*=aspect;
      float av=0.;
      pos=rotate(pos,t);
      pos+=basepos+vec2(sin(t)*.03,0.);
      for (float aa=0.; aa<16.; aa++) {
        float ot=1000.;
        if (aa<antialias*antialias) {
          vec2 aacoord=floor(vec2(aa/antialias,mod(aa,antialias)));
          vec2 c=pos+aacoord*pixsize/antialias;
          vec2 z=vec2(1.,0.);
          for (int i=0; i<iterations; i++) {
            z=z+vec2(z.x,-z.y)/dot(z,z)+c+vec2(c.x,-c.y)/dot(c,c);
            z=z*scale;
            float l=length(z);
            ot=min(ot,abs(l-orbittrap+.3*length(sin(z*2.+t*5.))));
            av+=l-.5;
          }
        }
        av/=float(iterations);
        av=av*.1*(2.+sin(t*60.*(1.+av*.001))*.2)*(1.-sin(iGlobalTime*100.)*.05);
        ot=pow(max(0.,.2-ot),3.)*60.;
        aacolor+=vec3(ot*2.,ot*ot,0.);
        aacolor+=vec3(.5,.3,1.)*max(0.,av-ot*.5);
      }
      aacolor/=(antialias*antialias);
      vec3 color=mix(vec3(length(aacolor)),aacolor,saturation)*brightness;
      color*=max(0.,1.-length(pow(abs(uv),vec2(6.)))*40.);
      gl_FragColor = vec4(color,1.0);
    }
  "
}
