import QtQuick 2.2

Shader
{
  id: shaderTruchetTentacles;
  property int iterations: 64;
  property real thickness: 0.1;
  property real superQuadPower: 8.0;
  property real fisheye: 0.5;

  fragmentCode: "
    uniform int iterations;
    uniform float thickness;
    uniform float superQuadPower;
    uniform float fisheye;

    float rand(vec3 r) { return fract(sin(dot(r.xy,vec2(1.38984*sin(r.z),1.13233*cos(r.z))))*653758.5453); }

    float truchetarc(vec3 pos)
    {
      float r=length(pos.xy);
    //	return max(abs(r-0.5),abs(pos.z-0.5))-thickness;
    //	return length(vec2(r-0.5,pos.z-0.5))-thickness;
      return pow(pow(abs(r-0.5),superQuadPower)+pow(abs(pos.z-0.5),superQuadPower),1.0/superQuadPower)-thickness;
    }

    float truchetcell(vec3 pos)
    {
      return min(min(
      truchetarc(pos),
      truchetarc(vec3(pos.z,1.0-pos.x,pos.y))),
      truchetarc(vec3(1.0-pos.y,1.0-pos.z,pos.x)));
    }

    float distfunc(vec3 pos)
    {
      vec3 cellpos=fract(pos);
      vec3 gridpos=floor(pos);

      float rnd=rand(gridpos);

      if(rnd<1.0/8.0) return truchetcell(vec3(cellpos.x,cellpos.y,cellpos.z));
      else if(rnd<2.0/8.0) return truchetcell(vec3(cellpos.x,1.0-cellpos.y,cellpos.z));
      else if(rnd<3.0/8.0) return truchetcell(vec3(1.0-cellpos.x,cellpos.y,cellpos.z));
      else if(rnd<4.0/8.0) return truchetcell(vec3(1.0-cellpos.x,1.0-cellpos.y,cellpos.z));
      else if(rnd<5.0/8.0) return truchetcell(vec3(cellpos.y,cellpos.x,cellpos.z));
      else if(rnd<6.0/8.0) return truchetcell(vec3(cellpos.y,1.0-cellpos.x,cellpos.z));
      else if(rnd<7.0/8.0) return truchetcell(vec3(1.0-cellpos.y,cellpos.x,cellpos.z));
      else  return truchetcell(vec3(1.0-cellpos.y,1.0-cellpos.x,cellpos.z));
    }

    vec3 gradient(vec3 pos)
    {
      const float eps=0.0001;
      float mid=distfunc(pos);
      return vec3(
      distfunc(pos+vec3(eps,0.0,0.0))-mid,
      distfunc(pos+vec3(0.0,eps,0.0))-mid,
      distfunc(pos+vec3(0.0,0.0,eps))-mid);
    }

    void main()
    {
      const float pi=3.141592;

      vec2 coords=(2.0*gl_FragCoord.xy-iResolution.xy)/length(iResolution.xy);

      float a=iGlobalTime/3.0;
      mat3 m=mat3(
      0.0,1.0,0.0,
      -sin(a),0.0,cos(a),
      cos(a),0.0,sin(a));
      m*=m;
      m*=m;

      vec3 ray_dir=m*normalize(vec3(1.4*coords,-1.0+fisheye*(coords.x*coords.x+coords.y*coords.y)));

      float t=iGlobalTime/3.0;
      vec3 ray_pos=vec3(
        2.0*(sin(t+sin(2.0*t)/2.0)/2.0+0.5),
        2.0*(sin(t-sin(2.0*t)/2.0-pi/2.0)/2.0+0.5),
        2.0*((-2.0*(t-sin(4.0*t)/4.0)/pi)+0.5+0.5));

      float i=float(iterations);
      for(int j=0;j<iterations;j++)
      {
        float dist=distfunc(ray_pos);
        ray_pos+=dist*ray_dir;

        if(abs(dist)<0.001) { i=float(j); break; }
      }

      vec3 normal=normalize(gradient(ray_pos));

      float ao=1.0-i/float(iterations);
      float what=pow(max(0.0,dot(normal,-ray_dir)),2.0);
      float vignette=pow(1.0-length(coords),0.3);
      float light=ao*what*vignette*1.4;

      float z=ray_pos.z/2.0;
    //	vec3 col=(sin(vec3(z,z+pi/3.0,z+pi*2.0/3.0))+2.0)/3.0;
      vec3 col=(cos(ray_pos/2.0)+2.0)/3.0;

      vec3 reflected=reflect(ray_dir,normal);
      // Use this for reflexions
      // vec3 env=textureCube(source,reflected*reflected*reflected).xyz;

      gl_FragColor=vec4(col*light+0.1,1.0);
      // gl_FragColor=vec4(col*light+0.1*env,1.0);
    }
  "
}
