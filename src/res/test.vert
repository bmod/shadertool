attribute highp vec4 posAttr;
attribute lowp vec4 colAttr;
varying lowp vec4 col;
varying lowp vec2 pos;
uniform highp mat4 matrix;


void main() {
   col = colAttr;
   pos = posAttr.xy;
   gl_Position = matrix * posAttr;
}

