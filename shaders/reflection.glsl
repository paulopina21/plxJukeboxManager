varying highp vec2 qt_TexCoord0;
uniform sampler2D source;
uniform sampler2D mask;

void main(void) { 
  gl_FragColor = texture2D(source, vec2(qt_TexCoord0.s, 1.0 - qt_TexCoord0.t)) * texture2D(mask, qt_TexCoord0).a; 
}