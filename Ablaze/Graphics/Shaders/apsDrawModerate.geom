
#pragma once

const char *APS_DRAW_M_GEOM =

"uniform vec2 pixelSize;"

"uniform float inSize;"
"uniform float inAngle;"
"uniform float inAlpha;"
"uniform vec3 inColor;"

"varying vec2 texCoord;"

"const mat4 points = mat4(-1.0, 1.0, 0.0, 1.0,"
"                         1.0, 1.0, 0.0, 1.0,"
"                         -1.0, -1.0, 0.0, 1.0,"
"                         1.0, -1.0, 0.0, 1.0);"

"void main(void) {"
"    float c = cos(inAngle), s = sin(inAngle);"
"   mat2 rotMat = mat2(c, s, -s, c);"
//"    mat4 rotMat = mat2(c, s, 0.0, 0.0, -s, c, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);"
"    vec4 clipCoord = vec4(pixelSize * inSize / length(gl_PositionIn[0].xyz), 0.0, 0.0);"
    
"    vec4 pos = gl_ProjectionMatrix * gl_PositionIn[0];"
    
    // In place of for (int i = 0;" i < 4;" i++) which is the devil
    
    // Up left
"    texCoord = (rotMat * vec2(-0.5, 0.5)) + .5;"
    
"    gl_Position = points[0] * clipCoord + pos;"
"    EmitVertex();"
    
    // Up right
"    texCoord = (rotMat * vec2(0.5, 0.5)) + .5;"
    
"    gl_Position = points[1] * clipCoord + pos;"
"    EmitVertex();"
    
    // Down left
"    texCoord = (rotMat * vec2(-0.5, -0.5)) + .5;"
    
"    gl_Position = points[2] * clipCoord + pos;"
"    EmitVertex();"
    
    // Down right
"    texCoord = (rotMat * vec2(0.5, -0.5)) + .5;"
    
"    gl_Position = points[3] * clipCoord + pos;"
"    EmitVertex();"
    
    // End for loop
    
"    EndPrimitive();"
"}";