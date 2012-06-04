
varying lowp vec4 vColor;
varying lowp float vTexId;

void main(void)
{
    // Transform coord to [-1,1]
    mediump vec2 cpt = gl_PointCoord * 2.0 - 1.0;
    
    // Radius fade to make points circular
    mediump float rfade = pow(clamp(1.0-length(cpt),0.0, 1.0),0.125);
    
    // Set final color--no texture yet
    gl_FragColor = vColor;
    gl_FragColor.a *= rfade;
}