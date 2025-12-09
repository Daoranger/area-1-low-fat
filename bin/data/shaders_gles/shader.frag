#ifdef GL_ES
// define default precision for float, vec, mat.
precision highp float;
#endif

uniform sampler2D tex;

void main (void) {
    
    gl_FragColor = gl_Color;
    
}