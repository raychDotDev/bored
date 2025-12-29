#version 130

precision highp float;

uniform sampler2D tex;
uniform float time;
uniform vec2 resolution;

in vec2 texCoords;

void main(void) {
	vec2 position = ( gl_FragCoord.xy / resolution.xy );
	float col = sin(position.y*900. + time*2.5) * sin(position.y*200. + time*-1.5);
	col = 1.- col + 0.3;
	vec4 res = vec4(col, col, col, 1.);
	gl_FragColor = texture(tex, position) * res; 
}
