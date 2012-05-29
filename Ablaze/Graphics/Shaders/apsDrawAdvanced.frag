
#pragma once

const char *APS_DRAW_A_FRAG =

"uniform sampler2D texture;"

"uniform float inAlpha;"
"uniform float inFalloff;"
"uniform vec3 inColor;"

"varying vec2 texCoord;"
"varying vec2 sqTexCoord;"

"void main() {"
"   float comp = pow(1.0 - length(sqTexCoord), inFalloff);"
//"   float comp = pow(inFalloff, length(sqTexCoord));"
"    vec4 mult = vec4(inColor, inAlpha * comp);"
"    gl_FragColor = texture2D(texture, texCoord) * mult;"
"}";