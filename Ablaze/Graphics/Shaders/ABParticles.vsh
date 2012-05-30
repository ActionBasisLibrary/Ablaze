
attribute vec3 position;
attribute vec4 color;
attribute float texId;
attribute float size;
attribute float live;

uniform mat4 modelviewMatrix;
uniform mat4 projectionMatrix;

varying lowp vec4 vColor;
varying lowp float vTexId;

void main(void)
{
    // Convert position to vec4
    vec4 position4 = vec4(position, 1.0);
    
    // Set outgoing color
    vColor = vec4(1.0);
    
    gl_PointSize = 40.0;
    gl_Position = projectionMatrix * modelviewMatrix * position4;
}