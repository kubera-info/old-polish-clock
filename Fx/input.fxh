struct VS_INPUT
{
    float4 Pos          : POSITION; // vertex position 
    float3 Normal       : NORMAL;   // this normal comes in per-vertex
};

struct VS_INPUT_TEX
{
    float4 Pos          : POSITION; // vertex position 
    float3 Normal       : NORMAL;   // this normal comes in per-vertex
    float2 Texture      : TEXCOORD0;// vertex texture coords 
};