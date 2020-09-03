#pragma once

#define OFX_GPUMIXER_BEGIN_NAMESPACE namespace ofx { namespace GpuMixer {
#define OFX_GPUMIXER_END_NAMESPACE } }

OFX_GPUMIXER_BEGIN_NAMESPACE

enum BlendModes_Mixer : int {
    BLEND_PASS = 0,
    BLEND_ADD = 1,
    BLEND_MULTIPLY = 2,
    BLEND_LIGHTEN = 3,
    BLEND_DARKEN = 4,
    BLEND_SUBTRACT = 5,
    BLEND_SCREEN = 6,
    BLEND_AVERAGE = 7,
    BLEND_SOFT_LIGHT = 8,
    BLEND_OVERLAY = 9,
    PASS_THROUGH = 10
};

enum ShaderTypes : int{
    SHADER_SHADERTOY,
    SHADER_ISF,
    SHADER_MADMATERIAL
};

OFX_GPUMIXER_END_NAMESPACE

namespace ofxGpuMixer = ofx::GpuMixer;
