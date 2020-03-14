//
//  shaderSnippets.h
//
//  Created by Jonas Fehr on 23/01/2017.
//
//

#ifndef MixingShaderSnipets_h
#define MixingShaderSnipets_h

#define STRINGIFY(x) #x

static string uniformsHeader =
STRINGIFY(
	uniform vec2 iResolution;
uniform float iGlobalTime;
);


static string uniforms =
STRINGIFY(
	uniform sampler2DRect tex$0;
uniform float u_H_$0;
uniform float u_S_$0;
uniform float u_B_$0;
uniform float u_tintAmt_$0;
uniform float u_contrast_$0;
uniform float u_gain_$0;
uniform float u_opacity_$0;
uniform int u_blendMode_$0;
uniform vec2 resolution_$0;
);

static string functions =
STRINGIFY(
	// functions
	vec3 hsv2rgb_smooth(in vec3 c) {
	vec3 rgb = clamp(abs(mod(c.x*6.0 + vec3(0.0, 4.0, 2.0), 6.0) - 3.0) - 1.0, 0.0, 1.0);
	rgb = rgb * rgb*(3.0 - 2.0*rgb);
	return c.z * mix(vec3(1.0), rgb, c.y);
}

vec3 hsv2rgb(in vec3 c) {
	vec3 rgb = clamp(abs(mod(c.x*6.0 + vec3(0.0, 4.0, 2.0), 6.0) - 3.0) - 1.0, 0.0, 1.0);
	return c.z * mix(vec3(1.0), rgb, c.y);
}

mat4 brightnessMatrix(float brightness) {
	return mat4(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		brightness, brightness, brightness, 1);
}

mat4 contrastMatrix(float contrast) {
	float t = (1.0 - contrast) / 2.0;

	return mat4(contrast, 0, 0, 0,
		0, contrast, 0, 0,
		0, 0, contrast, 0,
		t, t, t, 1);

}

mat4 saturationMatrix(float saturation) {
	vec3 luminance = vec3(0.3086, 0.6094, 0.0820);

	float oneMinusSat = 1.0 - saturation;

	vec3 red = vec3(luminance.x * oneMinusSat);
	red += vec3(saturation, 0, 0);

	vec3 green = vec3(luminance.y * oneMinusSat);
	green += vec3(0, saturation, 0);

	vec3 blue = vec3(luminance.z * oneMinusSat);
	blue += vec3(0, 0, saturation);

	return mat4(red, 0,
		green, 0,
		blue, 0,
		0, 0, 0, 1);
}


//  Function from Iñigo Quiles
//  www.iquilezles.org/www/articles/functions/functions.htm
float impulse(float k, float x) {
	float h = k * x;
	return h * exp(1.0 - h);
}

float cubicPulse(float c, float w, float x) {
	x = abs(x - c);
	if (x > w) return 0.0;
	x /= w;
	return 1.0 - x * x*(3.0 - 2.0*x);
}

float mapNum(float s, float a1, float a2, float b1, float b2) {
	return b1 + (s - a1)*(b2 - b1) / (a2 - a1);
}


// BLEND FUNCTIONS FROM https://github.com/jamieowen/glsl-blend

// Add
vec3 blendAdd(vec3 base, vec3 blend) {
	return base + blend; //min( base + blend , vec3(1.0));
}

// Multiply
vec3 blendMultiply(vec3 base, vec3 blend) {
	return base * blend;
}

// Lighten
vec3 blendLighten(vec3 base, vec3 blend) {
	return vec3(max(base.r, blend.r), max(base.g, blend.g), max(base.b, blend.b));
}

// Darken
vec3 blendDarken(vec3 base, vec3 blend) {
	return vec3(min(base.r, blend.r), min(base.g, blend.g), min(base.b, blend.b));
}

// Subtract
float blendSubtract(float base, float blend) {
	return max(base + blend - 1.0, 0.0);
}

vec3 blendSubtract(vec3 base, vec3 blend) {
	return max(base + blend - vec3(1.0), vec3(0.0));
}

// Screen
float blendScreen(float base, float blend) {
	return 1.0 - ((1.0 - base)*(1.0 - blend));
}

vec3 blendScreen(vec3 base, vec3 blend) {
	return vec3(blendScreen(base.r, blend.r), blendScreen(base.g, blend.g), blendScreen(base.b, blend.b));
}

// Average
vec3 blendAverage(vec3 base, vec3 blend) {
	return (base + blend) / 2.0;
}

// SoftLight
float blendSoftLight(float base, float blend) {
	return (blend < 0.5) ? (2.0*base*blend + base * base*(1.0 - 2.0*blend)) : (sqrt(base)*(2.0*blend - 1.0) + 2.0*base*(1.0 - blend));
}

vec3 blendSoftLight(vec3 base, vec3 blend) {
	return vec3(blendSoftLight(base.r, blend.r), blendSoftLight(base.g, blend.g), blendSoftLight(base.b, blend.b));
}

// Overlay
float blendOverlay(float base, float blend) {
	return base < 0.5 ? (2.0*base*blend) : (1.0 - 2.0*(1.0 - base)*(1.0 - blend));
}

vec3 blendOverlay(vec3 base, vec3 blend) {
	return vec3(blendOverlay(base.r, blend.r), blendOverlay(base.g, blend.g), blendOverlay(base.b, blend.b));
}


// BLEND FUNCTION
vec3 blendMode(int mode, vec3 base, vec3 blend, float opacity) {
	if (mode == 1) {
		return blendAdd(base.rgb, blend.rgb) * opacity + base.rgb * (1.0 - opacity);

	}
	else if (mode == 2) {
		return blendMultiply(base.rgb, blend.rgb) * opacity + base.rgb * (1.0 - opacity);

	}
	else if (mode == 3) {
		return blendLighten(base.rgb, blend.rgb) * opacity + base.rgb * (1.0 - opacity);

	}
	else if (mode == 4) {
		return blendDarken(base.rgb, blend.rgb) * opacity + base.rgb * (1.0 - opacity);

	}
	else if (mode == 5) {
		return blendSubtract(base.rgb, blend.rgb) * opacity + base.rgb * (1.0 - opacity);

	}
	else if (mode == 6) {
		return blendScreen(base.rgb, blend.rgb) * opacity + base.rgb * (1.0 - opacity);

	}
	else if (mode == 7) {
		return blendAverage(base.rgb, blend.rgb) * opacity + base.rgb * (1.0 - opacity);

	}
	else if (mode == 8) {
		return blendSoftLight(base.rgb, blend.rgb) * opacity + base.rgb * (1.0 - opacity);

	}
	else if (mode == 9) {
		return blendOverlay(base.rgb, blend.rgb) * opacity + base.rgb * (1.0 - opacity);
	}
}

//  ---

vec3 postProcessing(vec3 image, vec3 hsv, float contrast) {

	//image = mix(vec3(0.5), image, contrast);
	//image = vec4(contrastMatrix( 1.+contrast*2.) * vec4(image,1.)).rgb;
	image *= hsv2rgb(hsv);
	image = ((image - vec3(0.5)) * max(contrast + 0.5, 0.0)) + vec3(0.5);

	return image;
}

vec3 ContrastSaturationBrightness(vec3 color, float brt, float sat, float con) {
	brt *= 2.;
	con *= 2.;
	// Increase or decrease theese values to adjust r, g and b color channels seperately
	const float AvgLumR = 0.5;
	const float AvgLumG = 0.5;
	const float AvgLumB = 0.5;

	const vec3 LumCoeff = vec3(0.2125, 0.7154, 0.0721);

	vec3 AvgLumin = vec3(AvgLumR, AvgLumG, AvgLumB);
	vec3 brtColor = color * brt;
	vec3 intensity = vec3(dot(brtColor, LumCoeff));
	vec3 satColor = mix(intensity, brtColor, sat);
	vec3 conColor = mix(AvgLumin, satColor, con);
	return conColor;
}

);


static string mainHeader =
STRINGIFY(
	void main() {

	vec2 st = gl_FragCoord.xy / iResolution.xy;
	vec4 mixCol = vec4(0.);
	);

static string channel =
STRINGIFY(
	vec4 colTex_$0 = texture2DRect(tex$0, resolution_$0 * st).rgba;

if (u_blendMode_$0 == 10) {
	mixCol.rgb = blendMode(1, mixCol.rgb, colTex_$0.rgb, u_opacity_$0);
}
else {
	colTex_$0.rgb /= colTex_$0.a;

	// Apply gain.
	colTex_$0.rgb *= u_gain_$0;

	// Apply contrast.
	colTex_$0.rgb = ((colTex_$0.rgb - 0.5f) * max(u_contrast_$0, 0)) + 0.5f;

	// Apply Tint
	vec3 rgb_$0 = hsv2rgb_smooth(vec3(u_H_$0, u_S_$0, u_B_$0));
	colTex_$0.rgb = colTex_$0.rgb*(1 - u_tintAmt_$0) + colTex_$0.rgb*vec3(rgb_$0)*u_tintAmt_$0;


	// Return final pixel color.
	colTex_$0.rgb *= colTex_$0.a;

	mixCol.rgb = blendMode(u_blendMode_$0, mixCol.rgb, colTex_$0.rgb*u_opacity_$0, 1);
}
if (colTex_$0.a <= 0.0) {
	mixCol = vec4(0.0, 0.0, 0.0, 0.0);
}
else {
	mixCol = vec4(mixCol.rgb, u_opacity_$0);
}
);

//              vec3 rgb_$0 = vec3(0.);
//              rgb_$0 = clamp( abs(mod(u_H_$0*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0 );
//              rgb_$0 = rgb_$0*rgb_$0*(3.0-2.0*rgb_$0);
//
//              colTex_$0 = ContrastSaturationBrightness(colTex_$0*rgb_$0,u_B_$0, u_S_$0,u_contrast_$0);
//colTex_$0 = postProcessing(colTex_$0,vec3( u_H_$0, u_S_$0, u_B_$0), u_contrast_$0);
//colTex_$0 = contrastMatrix( 1.+u_contrast_$0*2. ) * colTex_$0;


static string output =
STRINGIFY(
	gl_FragColor = mixCol;
}
);

#endif /* MixingShaderSnipets_h */


// --- unused

//              float blendReflect(float base, float blend) {
//                  return (blend==1.0)?blend:min(base*base/(1.0-blend),1.0);
//              }
//              
//              vec3 blendReflect(vec3 base, vec3 blend) {
//                  return vec3(blendReflect(base.r,blend.r),blendReflect(base.g,blend.g),blendReflect(base.b,blend.b));
//              }
//              
//              vec3 blendReflect(vec3 base, vec3 blend, float opacity) {
//                  return (blendReflect(base, blend) * opacity + base * (1.0 - opacity));
//              }
//              
//              
//              vec3 blendExclusion(vec3 base, vec3 blend) {
//                  return base+blend-2.0*base*blend;
//              }
//              
//              vec3 blendExclusion(vec3 base, vec3 blend, float opacity) {
//                  return (blendExclusion(base, blend) * opacity + base * (1.0 - opacity));
//              }
//              

//              
//              
//              float blendColorBurn(float base, float blend) {
//                  return (blend==0.0)?blend:max((1.0-((1.0-base)/blend)),0.0);
//              }
//              
//              vec3 blendColorBurn(vec3 base, vec3 blend) {
//                  return vec3(blendColorBurn(base.r,blend.r),blendColorBurn(base.g,blend.g),blendColorBurn(base.b,blend.b));
//              }
//              
//              vec3 blendColorBurn(vec3 base, vec3 blend, float opacity) {
//                  return (blendColorBurn(base, blend) * opacity + base * (1.0 - opacity));
//              }
//              float blendColorDodge(float base, float blend) {
//                  return (blend==1.0)?blend:min(base/(1.0-blend),1.0);
//              }
//              
//              vec3 blendColorDodge(vec3 base, vec3 blend) {
//                  return vec3(blendColorDodge(base.r,blend.r),blendColorDodge(base.g,blend.g),blendColorDodge(base.b,blend.b));
//              }
//              
//              vec3 blendColorDodge(vec3 base, vec3 blend, float opacity) {
//                  return (blendColorDodge(base, blend) * opacity + base * (1.0 - opacity));
//              }
//              
//              
//              float blendVividLight(float base, float blend) {
//                  return (blend<0.5)?blendColorBurn(base,(2.0*blend)):blendColorDodge(base,(2.0*(blend-0.5)));
//              }
//              
//              vec3 blendVividLight(vec3 base, vec3 blend) {
//                  return vec3(blendVividLight(base.r,blend.r),blendVividLight(base.g,blend.g),blendVividLight(base.b,blend.b));
//              }
//              
//              vec3 blendVividLight(vec3 base, vec3 blend, float opacity) {
//                  return (blendVividLight(base, blend) * opacity + base * (1.0 - opacity));
//              }
//              
