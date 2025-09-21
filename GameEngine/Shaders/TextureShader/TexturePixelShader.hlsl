#include "TextureHeader.hlsli"

Texture2D g_tex : register(t0);
SamplerState g_sampState : register(s0);

float4 TexPS(PSInput input) : SV_TARGET
{
    return g_tex.Sample(g_sampState, input.uv);
}