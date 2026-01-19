#version 130

precision highp float;

uniform sampler2D tex;
uniform float time;
uniform float rand;
uniform vec2 resolution;

in vec2 texCoords;

void main(void) {
	vec2 position = (gl_FragCoord.xy / resolution.xy);
	float pos = position.y + (position.x * 0.1 );
	float lines_small = sin(pos * 800. + time * 2.5) ;
	float lines_big = clamp((sin(pos * 30. + time*-1.) + 1.) / 2., 0., 0.8);
	float blink = clamp(tan(time * 40. + rand * 10.), 0, 0.7);
	float col = lines_small + lines_big ;
	col = 1. - clamp(col, 0.0, 1.);
	vec4 res = vec4(col, col, col, 0.75);
	gl_FragColor = texture(tex, position) * res; 
}
