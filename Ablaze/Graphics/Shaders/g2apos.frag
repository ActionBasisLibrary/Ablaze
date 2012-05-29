
#pragma once

const char *GPS_ADVANCE_POSITION_FRAG = 

"#extension GL_EXT_gpu_shader4 : enable\n"

"uniform sampler2D positionTexture;"
"uniform sampler2D velocityTexture;"
"uniform sampler2D infoTexture;"

"uniform float time;"
"uniform float timePassed;"

"varying vec2 texcoord;"

//#define SPEED
"void main()"
"{"
#ifdef SPEED
"    vec4 x = texture2D(positionTexture, texcoord);"
"    vec4 v = texture2D(velocityTexture, texcoord);"
    
"    float dif = v.w - time;"
"    dif = float(int(dif) >> 32);"
    
"    gl_FragColor = dif * v * timePassed + x;"
"    gl_FragColor.w = x.w;"
#else
"    if (texture2D(velocityTexture, texcoord).w < time) {"
"        vec4 x = texture2D(positionTexture, texcoord);"
"        vec4 v = texture2D(velocityTexture, texcoord);"
        
"        gl_FragColor = v * timePassed + x;"
"        gl_FragColor.w = x.w;"
"    } else {"
"        gl_FragColor = texture2D(positionTexture, texcoord);"
"    }"
#endif
"}";