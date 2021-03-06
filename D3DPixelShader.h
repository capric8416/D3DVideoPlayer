#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim      HLSL Bind  Count
// ------------------------------ ---------- ------- ----------- -------------- ------
// state                             sampler      NA          NA             s0      1 
// luminanceChannel                  texture   float          2d             t0      1 
// chrominanceChannel                texture  float2          2d             t1      1 
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// TexCoord                 0   xy          0     NONE   float   xy  
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_Target                0   xyzw        0   TARGET   float   xyzw
//
//
// Sampler/Resource to DX9 shader sampler mappings:
//
// Target Sampler Source Sampler  Source Resource
// -------------- --------------- ----------------
// s0             s0              t0               
// s1             s0              t1               
//
//
// Level9 shader bytecode:
//
    ps_2_x
    def c0, -0.0627449974, -0.50195998, 1, 0
    def c1, 1.16438305, 1.59602702, 0, 2.01723194
    def c2, 1.16438305, -0.391761988, -0.812968016, 0
    dcl t0.xy
    dcl_2d s0
    dcl_2d s1
    texld r0, t0, s1
    texld r1, t0, s0
    mov r1.yz, r0.xxyw
    add r0.xyz, r1, c0.xyyw
    dp2add_sat r1.x, r0.xzzw, c1, c1.z
    dp3_sat r1.y, r0, c2
    dp2add_sat r1.z, r0, c1.xwzw, c1.z
    mov r1.w, c0.z
    mov oC0, r1

// approximately 11 instruction slots used (2 texture, 9 arithmetic)
ps_4_0
dcl_sampler s0, mode_default
dcl_resource_texture2d (float,float,float,float) t0
dcl_resource_texture2d (float,float,float,float) t1
dcl_input_ps linear v0.xy
dcl_output o0.xyzw
dcl_temps 2
sample r0.xyzw, v0.xyxx, t0.xyzw, s0
sample r1.xyzw, v0.xyxx, t1.xyzw, s0
mov r0.yz, r1.xxyx
add r0.xyz, r0.xyzx, l(-0.062745, -0.501960, -0.501960, 0.000000)
dp2_sat o0.x, r0.xzxx, l(1.164383, 1.596027, 0.000000, 0.000000)
dp3_sat o0.y, r0.xyzx, l(1.164383, -0.391762, -0.812968, 0.000000)
dp2_sat o0.z, r0.xyxx, l(1.164383, 2.017232, 0.000000, 0.000000)
mov o0.w, l(1.000000)
ret 
// Approximately 9 instruction slots used
#endif

const BYTE g_main_PS[] =
{
     68,  88,  66,  67, 169, 188, 
    252, 239, 107, 163, 126,  55, 
     61, 203,  83,  16, 154, 196, 
     43,  11,   1,   0,   0,   0, 
    152,   4,   0,   0,   6,   0, 
      0,   0,  56,   0,   0,   0, 
    108,   1,   0,   0, 220,   2, 
      0,   0,  88,   3,   0,   0, 
     48,   4,   0,   0, 100,   4, 
      0,   0,  65, 111, 110,  57, 
     44,   1,   0,   0,  44,   1, 
      0,   0,   0,   2, 255, 255, 
      0,   1,   0,   0,  44,   0, 
      0,   0,   0,   0,  44,   0, 
      0,   0,  44,   0,   0,   0, 
     44,   0,   2,   0,  36,   0, 
      0,   0,  44,   0,   0,   0, 
      0,   0,   1,   0,   1,   0, 
      1,   2, 255, 255,  81,   0, 
      0,   5,   0,   0,  15, 160, 
    115, 128, 128, 189, 115, 128, 
      0, 191,   0,   0, 128,  63, 
      0,   0,   0,   0,  81,   0, 
      0,   5,   1,   0,  15, 160, 
    129,  10, 149,  63, 157,  74, 
    204,  63,   0,   0,   0,   0, 
     84,  26,   1,  64,  81,   0, 
      0,   5,   2,   0,  15, 160, 
    129,  10, 149,  63,   7, 149, 
    200, 190, 172,  30,  80, 191, 
      0,   0,   0,   0,  31,   0, 
      0,   2,   0,   0,   0, 128, 
      0,   0,   3, 176,  31,   0, 
      0,   2,   0,   0,   0, 144, 
      0,   8,  15, 160,  31,   0, 
      0,   2,   0,   0,   0, 144, 
      1,   8,  15, 160,  66,   0, 
      0,   3,   0,   0,  15, 128, 
      0,   0, 228, 176,   1,   8, 
    228, 160,  66,   0,   0,   3, 
      1,   0,  15, 128,   0,   0, 
    228, 176,   0,   8, 228, 160, 
      1,   0,   0,   2,   1,   0, 
      6, 128,   0,   0, 208, 128, 
      2,   0,   0,   3,   0,   0, 
      7, 128,   1,   0, 228, 128, 
      0,   0, 212, 160,  90,   0, 
      0,   4,   1,   0,  17, 128, 
      0,   0, 232, 128,   1,   0, 
    228, 160,   1,   0, 170, 160, 
      8,   0,   0,   3,   1,   0, 
     18, 128,   0,   0, 228, 128, 
      2,   0, 228, 160,  90,   0, 
      0,   4,   1,   0,  20, 128, 
      0,   0, 228, 128,   1,   0, 
    236, 160,   1,   0, 170, 160, 
      1,   0,   0,   2,   1,   0, 
      8, 128,   0,   0, 170, 160, 
      1,   0,   0,   2,   0,   8, 
     15, 128,   1,   0, 228, 128, 
    255, 255,   0,   0,  83,  72, 
     68,  82, 104,   1,   0,   0, 
     64,   0,   0,   0,  90,   0, 
      0,   0,  90,   0,   0,   3, 
      0,  96,  16,   0,   0,   0, 
      0,   0,  88,  24,   0,   4, 
      0, 112,  16,   0,   0,   0, 
      0,   0,  85,  85,   0,   0, 
     88,  24,   0,   4,   0, 112, 
     16,   0,   1,   0,   0,   0, 
     85,  85,   0,   0,  98,  16, 
      0,   3,  50,  16,  16,   0, 
      0,   0,   0,   0, 101,   0, 
      0,   3, 242,  32,  16,   0, 
      0,   0,   0,   0, 104,   0, 
      0,   2,   2,   0,   0,   0, 
     69,   0,   0,   9, 242,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  16,  16,   0,   0,   0, 
      0,   0,  70, 126,  16,   0, 
      0,   0,   0,   0,   0,  96, 
     16,   0,   0,   0,   0,   0, 
     69,   0,   0,   9, 242,   0, 
     16,   0,   1,   0,   0,   0, 
     70,  16,  16,   0,   0,   0, 
      0,   0,  70, 126,  16,   0, 
      1,   0,   0,   0,   0,  96, 
     16,   0,   0,   0,   0,   0, 
     54,   0,   0,   5,  98,   0, 
     16,   0,   0,   0,   0,   0, 
      6,   1,  16,   0,   1,   0, 
      0,   0,   0,   0,   0,  10, 
    114,   0,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0, 115, 128, 128, 189, 
    115, 128,   0, 191, 115, 128, 
      0, 191,   0,   0,   0,   0, 
     15,  32,   0,  10,  18,  32, 
     16,   0,   0,   0,   0,   0, 
    134,   0,  16,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
    129,  10, 149,  63, 157,  74, 
    204,  63,   0,   0,   0,   0, 
      0,   0,   0,   0,  16,  32, 
      0,  10,  34,  32,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
      2,  64,   0,   0, 129,  10, 
    149,  63,   7, 149, 200, 190, 
    172,  30,  80, 191,   0,   0, 
      0,   0,  15,  32,   0,  10, 
     66,  32,  16,   0,   0,   0, 
      0,   0,  70,   0,  16,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0, 129,  10, 149,  63, 
     84,  26,   1,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     54,   0,   0,   5, 130,  32, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
    128,  63,  62,   0,   0,   1, 
     83,  84,  65,  84, 116,   0, 
      0,   0,   9,   0,   0,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   0,   2,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  82,  68, 
     69,  70, 208,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
     28,   0,   0,   0,   0,   4, 
    255, 255,   0,   1,   0,   0, 
    166,   0,   0,   0, 124,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
    130,   0,   0,   0,   2,   0, 
      0,   0,   5,   0,   0,   0, 
      4,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0, 147,   0,   0,   0, 
      2,   0,   0,   0,   5,   0, 
      0,   0,   4,   0,   0,   0, 
    255, 255, 255, 255,   1,   0, 
      0,   0,   1,   0,   0,   0, 
      4,   0,   0,   0, 115, 116, 
     97, 116, 101,   0, 108, 117, 
    109, 105, 110,  97, 110,  99, 
    101,  67, 104,  97, 110, 110, 
    101, 108,   0,  99, 104, 114, 
    111, 109, 105, 110,  97, 110, 
     99, 101,  67, 104,  97, 110, 
    110, 101, 108,   0,  77, 105, 
     99, 114, 111, 115, 111, 102, 
    116,  32,  40,  82,  41,  32, 
     72,  76,  83,  76,  32,  83, 
    104,  97, 100, 101, 114,  32, 
     67, 111, 109, 112, 105, 108, 
    101, 114,  32,  49,  48,  46, 
     49,   0, 171, 171,  73,  83, 
     71,  78,  44,   0,   0,   0, 
      1,   0,   0,   0,   8,   0, 
      0,   0,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   3,   3, 
      0,   0,  84, 101, 120,  67, 
    111, 111, 114, 100,   0, 171, 
    171, 171,  79,  83,  71,  78, 
     44,   0,   0,   0,   1,   0, 
      0,   0,   8,   0,   0,   0, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,  15,   0,   0,   0, 
     83,  86,  95,  84,  97, 114, 
    103, 101, 116,   0, 171, 171
};
