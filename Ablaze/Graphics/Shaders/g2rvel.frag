
#pragma once

const char *GPS_RESET_VELOCITY_FRAG =

"uniform sampler2D positionTexture;"
"uniform sampler2D velocityTexture;"
"uniform sampler2D randomTexture;"

"uniform vec2 infoPixelSize;"

"uniform float time;"
"uniform float life;"
"uniform float delay;"
"uniform int reset;"

"uniform vec3 velocityMin;"
"uniform vec3 velocityMax;"

"varying vec2 texcoord;"

"vec4 rand4();"
"vec3 randVelocity(vec3 interp);"
"bool kill();"

"void main()"
"{"
"    if (kill() || (reset == 1)) {"
"        vec4 v = rand4();"
"		gl_FragColor = vec4(randVelocity(v.xyz), time + delay * v.a);"
"    } else {"
"		gl_FragColor = texture2D(velocityTexture, texcoord);"
"	}"
"}"

"bool kill()"
"{"
"	return texture2D(velocityTexture, texcoord).w + life < time;"
"}"

"vec4 rand4()"
"{"
"    vec2 seed = texcoord + 13.0 * time * infoPixelSize - 0.2;"
"	return texture2D(randomTexture, seed);"
"}"

"vec3 randVelocity(vec3 interp)"
"{"
"	return (velocityMax - velocityMin) * interp + velocityMin;"
"}";