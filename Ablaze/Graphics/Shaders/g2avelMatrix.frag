#pragma once

const char *GPS_ADVANCE_VELOCITY_FRAG =

"#extension GL_EXT_gpu_shader4 : enable\n"

"uniform sampler2D positionTexture;"
"uniform sampler2D velocityTexture;"
"uniform sampler2D infoTexture;"

"uniform float time;"
"uniform float timePassed;"

"uniform vec3 forceConstant;"
"uniform vec3 forcePosition;"
"uniform vec3 forceVelocity;"

"varying vec2 texcoord;"

//#define GPSSPEED
"void main()"
"{"
#ifdef SPEED
"    vec4 v = texture2D(velocityTexture, texcoord);"

"    float dif = v.w - time;"
"    dif = float(int(dif) >> 32);"

"   vec3 acceleration = forceConstant + forcePosition * texture2D(positionTexture, texcoord).xyz + forceVelocity * v.xyz;"
"    gl_FragColor = dif * vec4(acceleration, 0.0) * timePassed + v;"
#else
"    vec4 v = texture2D(velocityTexture, texcoord);"
"    if (v.w < time) {"
"       vec3 acceleration = forceConstant + forcePosition * texture2D(positionTexture, texcoord).xyz + forceVelocity * v.xyz;"
"        gl_FragColor = vec4(acceleration, 0.0) * timePassed + v;"
"    } else {"
"        gl_FragColor = v;"
"    }"
#endif
#undef GPSSPEED
"}";