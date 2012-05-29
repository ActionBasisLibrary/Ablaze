
#pragma once

const char *APS_DRAW_A_GEOM =

"uniform vec2 pixelSize;"

"uniform float inSize;"
"uniform float inAngle;"
"uniform float inAlpha;"
"uniform vec3 inColor;"

"uniform vec2 inTexCoord;"
"uniform float inSampleRadius;"

"varying vec2 texCoord;"
"varying vec2 sqTexCoord;"

"const mat4 points = mat4(-1.0, 1.0, 0.0, 1.0,"
"                         1.0, 1.0, 0.0, 1.0,"
"                         -1.0, -1.0, 0.0, 1.0,"
"                         1.0, -1.0, 0.0, 1.0);"

"void main(void) {"
//"    float c = inSampleRadius * cos(inAngle), s = inSampleRadius * sin(inAngle);"
"   float c = cos(inAngle), s = sin(inAngle);"
//"    mat2 rotMat = mat2(c, s, -s, c);"
"   mat4 rotMat4 = mat4(c, s, 0.0, 0.0, -s, c, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);"
"    vec4 clipCoord = vec4(pixelSize * inSize / length(gl_PositionIn[0].xyz), 0.0, 0.0);"

"    vec4 pos = gl_ProjectionMatrix * gl_PositionIn[0];"

// In place of for (int i = 0;" i < 4;" i++) which is the devil

// Up left
//"    texCoord = (rotMat * vec2(-1.0, 1.0)) + inTexCoord;"
"   sqTexCoord = vec2(-1.0, 1.0);"
"   texCoord = vec2(-inSampleRadius, inSampleRadius) + inTexCoord;"

//"    gl_Position = points[0] * clipCoord + pos;"
"   gl_Position = (rotMat4 * points[0]) * clipCoord + pos;"
"    EmitVertex();"

// Up right
//"    texCoord = (rotMat * vec2(1.0, 1.0)) + inTexCoord;"
"   sqTexCoord = vec2(1.0, 1.0);"
"   texCoord = vec2(inSampleRadius, inSampleRadius) + inTexCoord;"

//"    gl_Position = points[1] * clipCoord + pos;"
"   gl_Position = (rotMat4 * points[1]) * clipCoord + pos;"
"    EmitVertex();"

// Down left
//"    texCoord = (rotMat * vec2(-1.0, -1.0)) + inTexCoord;"
"   sqTexCoord = vec2(-1.0, -1.0);"
"   texCoord = vec2(-inSampleRadius, -inSampleRadius) + inTexCoord;"

//"    gl_Position = points[2] * clipCoord + pos;"
"   gl_Position = (rotMat4 * points[2]) * clipCoord + pos;"
"    EmitVertex();"

// Down right
//"    texCoord = (rotMat * vec2(1.0, -1.0)) + inTexCoord;"
"   sqTexCoord = vec2(1.0, -1.0);"
"   texCoord = vec2(inSampleRadius, -inSampleRadius) + inTexCoord;"

//"    gl_Position = points[3] * clipCoord + pos;"
"   gl_Position = (rotMat4 * points[3]) * clipCoord + pos;"
"    EmitVertex();"

// End for loop

"    EndPrimitive();"
"}";