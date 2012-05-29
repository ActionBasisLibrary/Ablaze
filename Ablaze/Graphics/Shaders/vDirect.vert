
#pragma once

const char *VPS_DIRECT_VERT = 
"varying vec3 texCoord;"

"void main()"
"{	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"

"	texCoord = vec3(gl_MultiTexCoord0); }";