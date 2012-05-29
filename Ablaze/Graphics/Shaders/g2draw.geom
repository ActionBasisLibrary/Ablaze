#pragma once

const char *GPS_DRAW_GEOM =

"uniform sampler2D positionTexture;"
"uniform sampler2D velocityTexture;"
"uniform sampler2D infoTexture;"

"uniform vec2 infoPixelCoord;"
"uniform vec2 infoPixelSize;"
"uniform int infoMaxSteps;"

"uniform float time;"
"uniform float life;"

"uniform vec2 pixelSize;"

"varying vec4 texCoord;"

"const mat4 points = mat4(-1.0, 1.0, 0.0, 1.0,"
"                        1.0, 1.0, 0.0, 1.0,"
"                        -1.0, -1.0, 0.0, 1.0,"
"                        1.0, -1.0, 0.0, 1.0);"

//#define USEROT

"void main(void)"
"{"
"   vec2 iCoord = infoPixelCoord * infoPixelSize;"
"   iCoord.y -= infoPixelSize.y;"

"   for (int i = 0; i < infoMaxSteps; i++) {"
"       iCoord.y += infoPixelSize.y;"

"       vec4 vel = texture2D(velocityTexture, iCoord);"
"       if (vel.w >= time) continue;"

"       vec4 texpos = texture2D(positionTexture, iCoord);"

"       vec4 eyepos = gl_ModelViewMatrix * vec4(texpos.xyz, 1.0);"
"       vec4 clipCoord = vec4(10.0 * pixelSize * texpos.w / length(eyepos.xyz), 0.0, 0.0);"
"       vec4 pos = gl_ProjectionMatrix * eyepos;"

#ifdef USEROT
"       float c = cos(20.0), s = sin(20.0);"
"       mat2 rotMat = mat2(c, s, -s, c);"
#endif

"       float t = min(1.0, (time - vel.w) / life);"
"       float a = t * (t * (6.75 * t - 13.5) + 6.75);"
//"      float a = (vel.w + life - time) / life;"
"       texCoord.a = a;"

        // Up left
#ifdef USEROT
"       texCoord.xy = (rotMat * vec2(-0.5, 0.5)) + .5;"
#else
"       texCoord.xy = vec2(0.0, 1.0);"
#endif

"       gl_Position = points[0] * clipCoord + pos;"
"       EmitVertex();"

        // Up right
#ifdef USEROT
"       texCoord.xy = (rotMat * vec2(0.5, 0.5)) + .5;"
#else
"       texCoord.xy = vec2(1.0, 1.0);"
#endif

"       gl_Position = points[1] * clipCoord + pos;"
"       EmitVertex();"

        // Down left
#ifdef USEROT
"       texCoord.xy = (rotMat * vec2(-0.5, -0.5)) + .5;"
#else
"       texCoord.xy = vec2(0.0, 0.0);"
#endif

"       gl_Position = points[2] * clipCoord + pos;"
"       EmitVertex();"

        // Down right
#ifdef USEROT
"       texCoord.xy = (rotMat * vec2(0.5, -0.5)) + .5;"
#else
"       texCoord.xy = vec2(1.0, 0.0);"
#endif

"       gl_Position = points[3] * clipCoord + pos;"
"       EmitVertex();"

"       EndPrimitive();"
"   }"
"}";