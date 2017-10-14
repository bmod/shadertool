varying lowp vec4 col;
varying lowp vec2 pos;
uniform highp float time;
uniform highp vec2 res;

float circle(vec2 pos, float radius) {
    return length(pos) - radius;
}

void main() {
    vec2 p = pos;
    float ratio = res.x/res.y;
    if (ratio > 1.0) {
        p.x *= ratio;
    } else {
        p.y /= ratio;
    }


    vec3 col;

//    p.x += sin(time*12.1);
//    p.y += sin(time*11.6);

    float m = circle(p, sin(time) / 2.0 + 0.5);
    col = m < 0.0 ? vec3(1.0, 0.5, 0.0) : vec3(0.0);

    gl_FragColor = vec4(col, 1.0) + m;
}


