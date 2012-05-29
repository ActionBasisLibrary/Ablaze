
#pragma once

const char *APS_DRAW_M_FRAG =

"uniform sampler2D texture;"

"uniform float inAlpha;"
"uniform vec3 inColor;"

"varying vec2 texCoord;"

"void main() {"
"    vec4 mult = vec4(inColor, inAlpha);"
"    gl_FragColor = texture2D(texture, texCoord) * mult;"
"}";