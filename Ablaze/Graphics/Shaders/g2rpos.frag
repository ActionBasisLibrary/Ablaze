#pragma once

const char *GPS_RESET_POSITION_FRAG =

"uniform sampler2D positionTexture;"
"uniform sampler2D velocityTexture;"
"uniform sampler2D randomTexture;"

"uniform vec2 infoPixelSize;"

"uniform float time;"
"uniform float life;"
"uniform int reset;"

"uniform vec3 positionMin;"
"uniform vec3 positionMax;"
"uniform float size;"

"varying vec2 texcoord;"

"vec4 rand4();"
"vec3 randPosition(vec3 interp);"
"bool kill();"

"void main()"
"{"
"    if (kill() || (reset == 1)) {"
"        vec4 v = rand4();"
"        gl_FragColor = vec4(randPosition(v.xyz), .5 * size + .5 * v.a * size);"
"    } else {"
"        gl_FragColor = texture2D(positionTexture, texcoord);"
"    }"
"}"

"bool kill()"
"{"
"	return texture2D(velocityTexture, texcoord).w + life < time;"
"}"

"vec4 rand4()"
"{"
"    vec2 seed = texcoord - 7.0 * time * infoPixelSize + 0.3;"
"	return texture2D(randomTexture, seed);"
"}"

"vec3 randPosition(vec3 interp)"
"{"
"	return (positionMax - positionMin) * interp + positionMin;"
"}";