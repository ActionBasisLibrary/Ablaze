
#pragma once

const char *VPS_DIRECT_FRAG =

"uniform sampler3D texture;"

"uniform float alphaMod;"
"varying vec3 texCoord;"

"void main() {"
"   gl_FragColor = texture3D(texture, texCoord);"
"   gl_FragColor.a *= alphaMod;"
"}";