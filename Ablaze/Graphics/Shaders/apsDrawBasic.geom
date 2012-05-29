
#pragma once

const char *APS_DRAW_B_GEOM =

"uniform vec2 pixelSize;"

"uniform float inSize;"
"uniform float inAlpha;"

"varying vec2 texCoord;"

"const mat4 points = mat4(-1.0, 1.0, 0.0, 1.0,"
"                         1.0, 1.0, 0.0, 1.0,"
"                         -1.0, -1.0, 0.0, 1.0,"
"                         1.0, -1.0, 0.0, 1.0);"

"void main(void) {"
"    vec4 clipCoord = vec4(pixelSize * inSize / length(gl_PositionIn[0].xyz), 0.0, 0.0);"

"    vec4 pos = gl_ProjectionMatrix * gl_PositionIn[0];"

// In place of for (int i = 0;" i < 4;" i++) which is the devil

// Up left
"    texCoord = vec2(0.0, 1.0);"

"    gl_Position = points[0] * clipCoord + pos;"
"    EmitVertex();"

// Up right
"    texCoord = vec2(1.0, 1.0);"

"    gl_Position = points[1] * clipCoord + pos;"
"    EmitVertex();"

// Down left
"    texCoord = vec2(0.0, 0.0);"

"    gl_Position = points[2] * clipCoord + pos;"
"    EmitVertex();"

// Down right
"    texCoord = vec2(1.0, 0.0);"

"    gl_Position = points[3] * clipCoord + pos;"
"    EmitVertex();"

// End for loop

"    EndPrimitive();"
"}";