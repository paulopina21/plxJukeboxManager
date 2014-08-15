import QtQuick 2.2

Shader
{
  id: shaderHotShower;
  property real sphsize: 0.7; // planet size
  property real dist: 0.27; // distance for glow and distortion
  property real perturb: 0.3; // distance amount of the flow around the planet
  property real displacement: 0.015; // hot air effect
  property real windspeed: 0.4; // speed of wind flow
  property real steps: 110.0; // number of steps for the volumetric rendering
  property real stepsize: 0.025;
  property real brightness: 0.43;
  property real fade: 0.005; // fade by distance
  property real glow: 3.5; // glow amount, mainly on hit side
  property vector3d planetcolor: Qt.vector3d(0.55,0.4,0.3);

  // Fractal params
  property int iterations: 13;
  property real fractparam: 0.7;
  property vector3d offset: Qt.vector3d(1.5,2.,-1.5);

  fragmentCode: "
    uniform sampler2D iChannel0;
    uniform sampler2D iChannel1;

    // rendering params
    uniform float sphsize; // planet size
    uniform float dist; // distance for glow and distortion
    uniform float perturb; // distortion amount of the flow around the planet
    uniform float displacement; // hot air effect
    uniform float windspeed; // speed of wind flow
    uniform float steps; // number of steps for the volumetric rendering
    uniform float stepsize;
    uniform float brightness;
    uniform float fade; //fade by distance
    uniform float glow; // glow amount, mainly on hit side
    uniform vec3 planetcolor;

    // fractal params
    uniform int iterations;
    uniform float fractparam;
    uniform vec3 offset;

    float wind(vec3 p)
    {
      float d=max(0.,dist-max(0.,length(p)-sphsize)/sphsize)/dist; // for distortion and glow area
      float x=max(0.2,p.x*2.); // to increase glow on left side
      p.y*=1.+max(0.,-p.x-sphsize*.25)*1.5; // left side distortion (cheesy)
      p-=d*normalize(p)*perturb; // spheric distortion of flow
      p+=vec3(iGlobalTime*windspeed,0.,0.); // flow movement
      p=abs(fract((p+offset)*.1)-.5); // tile folding

      for (int i=0; i<iterations; i++)
      {
        p=abs(p)/dot(p,p)-fractparam; // the magic formula for the hot flow
      }

      return length(p)*(1.+d*glow*x)+d*glow*x; // return the result with glow applied
    }

    void main(void)
    {
      // get ray dir
      vec2 uv = gl_FragCoord.xy / iResolution.xy-.5;
      vec3 dir=vec3(uv,1.);
      dir.x*=iResolution.x/iResolution.y;
      vec3 from=vec3(0.,0.,-2.+texture2D(iChannel0,uv*.5+iGlobalTime).x*stepsize); //from+dither

      // volumetric rendering
      float v=0., l=-0.0001, t=iGlobalTime*windspeed*.2;

      for (float r=10.;r<steps;r++)
      {
        vec3 p=from+r*dir*stepsize;
        float tx=texture2D(iChannel0,uv*.2+vec2(t,0.)).x*displacement; // hot air effect
        if (length(p)-sphsize-tx>0.)
        // outside planet, accumulate values as ray goes, applying distance fading
          v+=min(50.,wind(p))*max(0.,1.-r*fade);
        else if (l<0.)
          //inside planet, get planet shading if not already
          //loop continues because of previous problems with breaks and not always optimizes much
          l=pow(max(.53,dot(normalize(p),normalize(vec3(-1.,.5,-0.3)))),4.)
          *(.5+texture2D(iChannel1,uv*vec2(2.,1.)*(1.+p.z*.5)+vec2(tx+t*.5,0.)).x*2.);
      }
      v/=steps; v*=brightness; // average values and apply bright factor
      vec3 col=vec3(v*1.25,v*v,v*v*v)+l*planetcolor; // set color
      col*=1.-length(pow(abs(uv),vec2(5.)))*14.; // vignette (kind of)
      gl_FragColor = vec4(col,1.0);
    }
  "
}
