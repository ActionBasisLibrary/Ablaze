#pragma once

const char *GPS_DRAW_FRAG =

"uniform sampler2D positionTexture;"
"uniform sampler2D velocityTexture;"
"uniform sampler2D infoTexture;"

"uniform sampler2D texture;"

"uniform vec2 pixelCoord;"

"varying vec4 texCoord;"

"void main()"
"{"
"    gl_FragColor = texture2D(texture, texCoord.xy);"
"    gl_FragColor.a *= texCoord.a;"
"}";