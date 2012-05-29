#pragma once

const char *GPS_SORT_EOMERGE_FRAG =

"#extension GL_EXT_gpu_shader4 : enable\n"

"uniform sampler2D positionTexture;"
"uniform sampler2D indexTexture;"
"uniform vec2 bufferSize;"
"uniform vec2 pixelSize;"

"uniform int recTree, subTree;"
"uniform vec3 eyePosition;"

"varying vec2 texcoord;"

"int getIndex(vec2 tex) {"
"   tex = floor(tex * bufferSize);"
"   return int(tex.x * bufferSize.y + tex.y);"
"}"

"vec2 getTex(int idx) {"
"   int x = idx / int(bufferSize.y);"
"   int y = idx - int(bufferSize.y) * x;"
"   return vec2(float(x) + .5, float(y) + .5) * pixelSize;"
"}"

//#define INDEXSORT

#ifndef INDEXSORT
"bool compare(vec2 tex0, vec2 tex1) {"
"   return (length(texture2D(positionTexture, texture2D(indexTexture, tex1).xy).xyz - eyePosition) < length(texture2D(positionTexture, texture2D(indexTexture, tex0).xy).xyz - eyePosition));"
"}"

"bool lequal(vec2 tex0, vec2 tex1) {"
"   return (length(texture2D(positionTexture, texture2D(indexTexture, tex1).xy).xyz - eyePosition) == length(texture2D(positionTexture, texture2D(indexTexture, tex0).xy).xyz - eyePosition));"
"}"

/*
 * -0 for equal, 1 if tex1 > tex0 (tex1 should draw second : dist(tex1) < dist(tex0)), -1 otherwise
 */
"int iComp(vec2 tex0, vec2 tex1) {"
"   float val0 = length(texture2D(positionTexture, texture2D(indexTexture, tex0).xy).xyz - eyePosition);"
"   float val1 = length(texture2D(positionTexture, texture2D(indexTexture, tex1).xy).xyz - eyePosition);"
"   return int(sign(val0 - val1));"
"}"

#else
"float lum(vec3 vect)"
"{"
"   return .27 * vect.r + .67 * vect.g + .06 * vect.b;"
"}"

"bool compare(vec2 tex0, vec2 tex1) {"
"   return (lum(texture2D(indexTexture, tex1).xyz) > lum(texture2D(indexTexture, tex0).xyz));"
"}"

"bool lequal(vec2 tex0, vec2 tex1) {"
"   return (lum(texture2D(indexTexture, tex1).xyz) == lum(texture2D(indexTexture, tex0).xyz));"
"}"

#endif

"void main(void) {"
"   int idx = getIndex(texcoord);"

"   if (recTree == subTree) {"
       // recMult = -1 if idx is even mult of (recTree / 2), 1 otherwise"
"       int recMult = ((2 * idx / recTree) & 1) * 2 - 1;"

"       int idxc = idx - recMult * recTree / 2;"
"       vec2 texComp = getTex(idxc);"

"       if (recMult == iComp(texcoord, texComp)) {"
           // isLower and isHigher (-1 == -1), isHigher and isLower (1 == 1): switch"
"           gl_FragColor = texture2D(indexTexture, texComp);"
"       } else {"
           // else leave 'em"
"           gl_FragColor = texture2D(indexTexture, texcoord);"
"       }"

"   } else {"
"       int idxmod = idx - idx / recTree * recTree;"
"       int idxstep = (idxmod - subTree / 2);"

"       if (idxstep >= 0 && idxstep < recTree - subTree) {"
           // excluding edges...
           // subMult = -1 if idxstep is even mult of (subTree / 2), 1 otherwise
"           int subMult = ((2 * idxstep / subTree) & 1) * 2 - 1;"

"           int idxc = idx - subMult * subTree / 2;"
"           vec2 texComp = getTex(idxc);"

"           if (subMult == iComp(texcoord, texComp)) {"
               // isLower and isHigher (-1 == -1), isHigher and isLower (1 == 1): switch
"               gl_FragColor = texture2D(indexTexture, texComp);"
"           } else {"
               // else leave 'em
"               gl_FragColor = texture2D(indexTexture, texcoord);"
"           }"
"       } else {"
"           gl_FragColor = texture2D(indexTexture, texcoord);"
"       }"
"   }"
"}";