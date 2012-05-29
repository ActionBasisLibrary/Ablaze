
#pragma once

const char *APS_DRAW_B_FRAG =

"uniform sampler2D texture;"

"uniform float inAlpha;"

"varying vec2 texCoord;"

"void main() {"
//"   gl_FragColor = vec4(texCoord, 0.0, 1.0);"
//"   return;"
"   gl_FragColor = texture2D(texture, texCoord);"
"   gl_FragColor.a *= inAlpha;"
"}";