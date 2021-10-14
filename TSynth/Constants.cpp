#include "Constants.h"

const char* VERSION = "V2.31";
const float PROGMEM FILTERFREQS256[256] = {20, 21, 22, 23, 24, 26, 27, 28, 29, 31, 32, 33, 34, 35, 36, 38, 39, 40, 41, 43, 44, 46, 47, 49, 50, 52, 53, 55, 57, 59, 61, 63, 65, 68, 70, 73, 76, 78, 81, 84, 87, 91, 94, 98, 102, 106, 110, 114, 119, 123, 128, 133, 138, 144, 149, 155, 161, 167, 174, 181, 187, 195, 202, 210, 218, 226, 234, 243, 252, 261, 271, 281, 291, 301, 312, 323, 334, 346, 358, 370, 383, 395, 409, 422, 436, 451, 465, 480, 496, 512, 528, 544, 561, 578, 596, 614, 633, 652, 671, 691, 711, 732, 753, 774, 796, 818, 841, 865, 888, 913, 937, 962, 988, 1014, 1041, 1068, 1096, 1124, 1152, 1182, 1211, 1242, 1272, 1304, 1335, 1368, 1401, 1434, 1468, 1503, 1538, 1574, 1610, 1647, 1684, 1722, 1761, 1800, 1840, 1881, 1922, 1964, 2006, 2049, 2093, 2137, 2182, 2227, 2274, 2320, 2368, 2416, 2465, 2515, 2565, 2616, 2668, 2720, 2773, 2827, 2881, 2936, 2992, 3049, 3106, 3164, 3223, 3283, 3343, 3404, 3466, 3529, 3592, 3656, 3721, 3787, 3853, 3921, 3989, 4058, 4127, 4198, 4269, 4341, 4414, 4488, 4563, 4638, 4715, 4792, 4870, 4949, 5029, 5109, 5191, 5273, 5357, 5441, 5526, 5612, 5699, 5787, 5876, 5965, 6056, 6147, 6240, 6333, 6427, 6523, 6619, 6716, 6814, 6913, 7013, 7114, 7216, 7319, 7424, 7529, 7635, 7742, 7850, 7959, 8069, 8180, 8292, 8405, 8519, 8634, 8751, 8868, 8986, 9106, 9226, 9348, 9471, 9594, 9719, 9845, 9972, 10100, 10230, 10360, 10491, 10624, 10758, 10893, 11029, 11166, 11304, 11443, 11584, 11726, 11869, 12000};
const float POWER[128] = {0.0f, 0.00009f, 0.0003f, 0.0006f, 0.001f, 0.0016f, 0.0022f, 0.003f, 0.004f, 0.005f, 0.0062f, 0.0075f, 0.0089f, 0.0105f, 0.0122f, 0.014f, 0.016f, 0.018f, 0.02f, 0.022f, 0.025f, 0.027f, 0.03f, 0.033f, 0.036f, 0.039f, 0.042f, 0.045f, 0.049f, 0.052f, 0.056f, 0.06f, 0.063f, 0.068f, 0.072f, 0.076f, 0.08f, 0.085f, 0.09f, 0.094f, 0.099f, 0.104f, 0.109f, 0.115f, 0.12f, 0.126f, 0.131f, 0.137f, 0.143f, 0.149f, 0.155f, 0.161f, 0.168f, 0.174f, 0.181f, 0.188f, 0.194f, 0.201f, 0.209f, 0.216f, 0.223f, 0.231f, 0.238f, 0.246f, 0.254f, 0.262f, 0.27f, 0.278f, 0.287f, 0.295f, 0.304f, 0.313f, 0.321f, 0.33f, 0.34f, 0.349f, 0.358f, 0.368f, 0.377f, 0.387f, 0.397f, 0.407f, 0.417f, 0.427f, 0.437f, 0.448f, 0.459f, 0.469f, 0.48f, 0.491f, 0.502f, 0.513f, 0.525f, 0.536f, 0.548f, 0.56f, 0.571f, 0.583f, 0.595f, 0.608f, 0.62f, 0.632f, 0.645f, 0.658f, 0.671f, 0.684f, 0.697f, 0.71f, 0.723f, 0.737f, 0.75f, 0.764f, 0.778f, 0.792f, 0.806f, 0.82f, 0.834f, 0.849f, 0.863f, 0.878f, 0.893f, 0.908f, 0.923f, 0.938f, 0.953f, 0.969f, 0.984f, 1.0f};
const  float NOTEFREQS[128] = {8.176f, 8.662f, 9.177f, 9.723f, 10.301f, 10.913f, 11.562f, 12.25f, 12.978f, 13.75f, 14.568f, 15.434f, 16.352f, 17.324f, 18.354f, 19.445f, 20.602f, 21.827f, 23.125f, 24.5f, 25.957f, 27.5f, 29.135f, 30.868f, 32.703f, 34.648f, 36.708f, 38.891f, 41.203f, 43.654f, 46.249f, 48.999f, 51.913f, 55.0f, 58.27f, 61.735f, 65.406f, 69.296f, 73.416f, 77.782f, 82.407f, 87.307f, 92.499f, 97.999f, 103.826f, 110.0f, 116.541f, 123.471f, 130.813f, 138.591f, 146.832f, 155.563f, 164.814f, 174.614f, 184.997f, 195.998f, 207.652f, 220.0f, 233.082f, 246.942f, 261.626f, 277.183f, 293.665f, 311.127f, 329.628f, 349.228f, 369.994f, 391.995f, 415.305f, 440.0f, 466.164f, 493.883f, 523.251f, 554.365f, 587.33f, 622.254f, 659.255f, 698.456f, 739.989f, 783.991f, 830.609f, 880.0f, 932.328f, 987.767f, 1046.502f, 1108.731f, 1174.659f, 1244.508f, 1318.51f, 1396.913f, 1479.978f, 1567.982f, 1661.219f, 1760.0f, 1864.655f, 1975.533f, 2093.005f, 2217.461f, 2349.318f, 2489.016f, 2637.02f, 2793.826f, 2959.955f, 3135.963f, 3322.438f, 3520.0f, 3729.31f, 3951.066f, 4186.009f, 4434.922f, 4698.636f, 4978.032f, 5274.041f, 5587.652f, 5919.911f, 6271.927f, 6644.875f, 7040.0f, 7458.62f, 7902.133f, 8372.018f, 8869.844f, 9397.273f, 9956.063f, 10548.08f, 11175.3f, 11839.82f, 12543.85f};
const uint32_t ENVTIMES[128] = {1, 2, 4, 6, 9, 14, 20, 26, 33, 41, 49, 58, 67, 78, 89, 99, 111, 124, 136, 150, 164, 178, 192, 209, 224, 241, 258, 276, 295, 314, 333, 353, 374, 395, 418, 440, 464, 489, 513, 539, 565, 592, 621, 650, 680, 710, 742, 774, 808, 843, 878, 915, 952, 991, 1031, 1073, 1114, 1158, 1202, 1250, 1297, 1346, 1396, 1448, 1502, 1558, 1614, 1676, 1735, 1794, 1864, 1923, 1994, 2065, 2136, 2207, 2289, 2360, 2443, 2525, 2620, 2702, 2797, 2891, 2985, 3092, 3186, 3292, 3410, 3516, 3634, 3752, 3882, 4012, 4142, 4272, 4413, 4567, 4708, 4862, 5027, 5180, 5357, 5522, 5699, 5888, 6077, 6278, 6478, 6691, 6903, 7127, 7351, 7587, 7835, 8083, 8343, 8614, 8885, 9169, 9464, 9770, 10077, 10408, 10738, 11080, 11434, 11700};
const float LFOTEMPO[128] = {8.0f, 8.0f, 8.0f, 8.0f, 8.0f, 8.0f, 8.0f, 8.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 6.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.75f, 0.75f, 0.75f, 0.75f, 0.75f, 0.75f, 0.75f, 0.75f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.333f, 0.333f, 0.333f, 0.333f, 0.333f, 0.333f, 0.333f, 0.333f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.167f, 0.167f, 0.167f, 0.167f, 0.167f, 0.167f, 0.167f, 0.167f, 0.125f, 0.125f, 0.125f, 0.125f, 0.125f, 0.125f, 0.125f, 0.125f, 0.083f, 0.083f, 0.083f, 0.083f, 0.083f, 0.083f, 0.083f, 0.083f, 0.063f, 0.063f, 0.063f, 0.063f, 0.063f, 0.063f, 0.063f, 0.063f, 0.047f, 0.047f, 0.047f, 0.047f, 0.047f, 0.047f, 0.047f, 0.047f};
const String LFOTEMPOSTR[128] = {"1/32", "1/32", "1/32", "1/32", "1/32", "1/32", "1/32", "1/32", "3/64", "3/64", "3/64", "3/64", "3/64", "3/64", "3/64", "3/64", "1/16", "1/16", "1/16", "1/16", "1/16", "1/16", "1/16", "1/16", "3/32", "3/32", "3/32", "3/32", "3/32", "3/32", "3/32", "3/32", "1/8", "1/8", "1/8", "1/8", "1/8", "1/8", "1/8", "1/8", "3/16", "3/16", "3/16", "3/16", "3/16", "3/16", "3/16", "3/16", "1/4", "1/4", "1/4", "1/4", "1/4", "1/4", "1/4", "1/4", "3/8", "3/8", "3/8", "3/8", "3/8", "3/8", "3/8", "3/8", "1/2", "1/2", "1/2", "1/2", "1/2", "1/2", "1/2", "1/2", "3/4", "3/4", "3/4", "3/4", "3/4", "3/4", "3/4", "3/4", "1", "1", "1", "1", "1", "1", "1", "1", "3/2", "3/2", "3/2", "3/2", "3/2", "3/2", "3/2", "3/2", "2", "2", "2", "2", "2", "2", "2", "2", "3", "3", "3", "3", "3", "3", "3", "3", "4", "4", "4", "4", "4", "4", "4", "4", "6", "6", "6", "6", "6", "6", "6", "6"};
const uint32_t OSCMIXA[128] = {127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 125, 123, 120, 118, 116, 114, 112, 110, 108, 106, 104, 102, 100, 98, 96, 94, 92, 90, 88, 86, 84, 82, 80, 78, 76, 74, 72, 70, 68, 66, 64, 62, 60, 58, 56, 54, 52, 50, 48, 46, 44, 42, 40, 38, 36, 34, 32, 30, 28, 26, 24, 22, 20, 18, 16, 14, 12, 10, 8, 6, 4, 2, 0};
const uint32_t OSCMIXB[128] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74, 76, 78, 80, 82, 84, 86, 88, 90, 92, 94, 96, 98, 100, 102, 104, 106, 108, 110, 112, 114, 116, 118, 120, 123, 125, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127};
const int PITCH[128] = { -24, -24, -24, -24, -24, -12, -12, -12, -12, -12, -11, -11, -11, -11, -11, -10, -10, -10, -10, -9, -9, -9, -9, -9, -8, -8, -8, -8, -8, -7, -7, -7, -7, -7, -6, -6, -6, -6, -5, -5, -5, -5, -5, -4, -4, -4, -4, -4, -3, -3, -3, -3, -3, -2, -2, -2, -2, -1, -1, -1, -1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 10, 10, 10, 10, 11, 11, 11, 11, 12, 12, 12, 12, 12, 24, 24, 24, 24, 24};
const float KEYTRACKINGAMT[128] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1, 1, 1, 1, 1, 1, 1};
const  float  DIV8192 = 1.0f / 8192.0f;//For pitchbend
const float  DIV127 = 1.0f / 127.0f;
const  float  DIV12 = 1.0f / 12.0f;
const  float  DIV24 = 1.0f / 24.0f;
const  float  DIV1270 = 1.0f / 1270.0f;//Velocity
const float LINEAR[128] = {0, 0.008, 0.016, 0.024, 0.031, 0.039, 0.047, 0.055, 0.063, 0.071, 0.079, 0.087, 0.094, 0.102, 0.11, 0.118, 0.126, 0.134, 0.142, 0.15, 0.157, 0.165, 0.173, 0.181, 0.189, 0.197, 0.205, 0.213, 0.22, 0.228, 0.236, 0.244, 0.252, 0.26, 0.268, 0.276, 0.283, 0.291, 0.299, 0.307, 0.315, 0.323, 0.331, 0.339, 0.346, 0.354, 0.362, 0.37, 0.378, 0.386, 0.394, 0.402, 0.409, 0.417, 0.425, 0.433, 0.441, 0.449, 0.457, 0.465, 0.472, 0.48, 0.488, 0.496, 0.504, 0.512, 0.52, 0.528, 0.535, 0.543, 0.551, 0.559, 0.567, 0.575, 0.583, 0.591, 0.598, 0.606, 0.614, 0.622, 0.63, 0.638, 0.646, 0.654, 0.661, 0.669, 0.677, 0.685, 0.693, 0.701, 0.709, 0.717, 0.724, 0.732, 0.74, 0.748, 0.756, 0.764, 0.772, 0.78, 0.787, 0.795, 0.803, 0.811, 0.819, 0.827, 0.835, 0.843, 0.85, 0.858, 0.866, 0.874, 0.882, 0.89, 0.898, 0.906, 0.913, 0.921, 0.929, 0.937, 0.945, 0.953, 0.961, 0.969, 0.976, 0.984, 0.992, 1.00};
const float LINEARCENTREZERO[128] = { -1, -0.98, -0.97, -0.95, -0.93, -0.92, -0.9, -0.88, -0.87, -0.85, -0.83, -0.82, -0.8, -0.78, -0.77, -0.75, -0.73, -0.72, -0.7, -0.68, -0.67, -0.65, -0.63, -0.62, -0.6, -0.58, -0.57, -0.55, -0.53, -0.52, -0.5, -0.48, -0.47, -0.45, -0.43, -0.42, -0.4, -0.38, -0.37, -0.35, -0.33, -0.32, -0.3, -0.28, -0.27, -0.25, -0.23, -0.22, -0.2, -0.18, -0.17, -0.15, -0.13, -0.12, -0.1, -0.08, -0.07, -0.05, -0.03, -0.02, -0.01, 0, 0, 0, 0, 0, 0, 0.01, 0.02, 0.03, 0.05, 0.07, 0.08, 0.1, 0.12, 0.13, 0.15, 0.17, 0.18, 0.2, 0.22, 0.23, 0.25, 0.27, 0.28, 0.3, 0.32, 0.33, 0.35, 0.37, 0.38, 0.4, 0.42, 0.43, 0.45, 0.47, 0.48, 0.5, 0.52, 0.53, 0.55, 0.57, 0.58, 0.6, 0.62, 0.63, 0.65, 0.67, 0.68, 0.7, 0.72, 0.73, 0.75, 0.77, 0.78, 0.8, 0.82, 0.83, 0.85, 0.87, 0.88, 0.9, 0.92, 0.93, 0.95, 0.97, 0.98, 1};
const float BANDPASS = -99;
const float LOWPASS = 0;
const float HIGHPASS = 1.0;
const float LINEAR_FILTERMIXER[128] = {LOWPASS, 0.008, 0.016, 0.024, 0.031, 0.039, 0.047, 0.055, 0.063, 0.071, 0.079, 0.087, 0.094, 0.102, 0.11, 0.118, 0.126, 0.134, 0.142, 0.15, 0.157, 0.165, 0.173, 0.181, 0.189, 0.197, 0.205, 0.213, 0.22, 0.228, 0.236, 0.244, 0.252, 0.26, 0.268, 0.276, 0.283, 0.291, 0.299, 0.307, 0.315, 0.323, 0.331, 0.339, 0.346, 0.354, 0.362, 0.37, 0.378, 0.386, 0.394, 0.402, 0.409, 0.417, 0.425, 0.433, 0.441, 0.449, 0.457, 0.465, 0.472, 0.48, 0.488, 0.496, 0.504, 0.512, 0.52, 0.528, 0.535, 0.543, 0.551, 0.559, 0.567, 0.575, 0.583, 0.591, 0.598, 0.606, 0.614, 0.622, 0.63, 0.638, 0.646, 0.654, 0.661, 0.669, 0.677, 0.685, 0.693, 0.701, 0.709, 0.717, 0.724, 0.732, 0.74, 0.748, 0.756, 0.764, 0.772, 0.78, 0.787, 0.795, 0.803, 0.811, 0.819, 0.827, 0.835, 0.843, 0.85, 0.858, 0.866, 0.874, 0.882, 0.89, 0.898, 0.906, 0.913, 0.921, 0.929, 0.937, 0.945, 0.953, 0.961, 0.976, 0.988, HIGHPASS, BANDPASS, BANDPASS};//{LP...HP,BP,BP}
const int16_t PROGMEM PARABOLIC_WAVE[256] = { -26092, -26053, -25939, -25748, -25486, -25153, -24753, -24289, -23768, -23192, -22570, -21905, -21204, -20472, -19715, -18940, -18153, -17356, -16558, -15761, -14969, -14186, -13415, -12658, -11916, -11192, -10485, -9795, -9123, -8466, -7824, -7196, -6579, -5972, -5373, -4779, -4191, -3602, -3015, -2426, -1836, -1240, -642, -38, 569, 1182, 1798, 2417, 3039, 3661, 4283, 4903, 5520, 6133, 6740, 7339, 7928, 8509, 9078, 9637, 10183, 10717, 11238, 11746, 12242, 12726, 13199, 13662, 14114, 14558, 14994, 15423, 15846, 16263, 16676, 17085, 17490, 17893, 18292, 18689, 19082, 19473, 19859, 20241, 20619, 20990, 21356, 21714, 22065, 22406, 22737, 23058, 23369, 23668, 23953, 24227, 24489, 24738, 24974, 25198, 25410, 25610, 25799, 25978, 26146, 26305, 26456, 26599, 26733, 26861, 26982, 27098, 27207, 27311, 27410, 27503, 27591, 27673, 27749, 27820, 27884, 27942, 27992, 28000, 28000, 28000, 28000, 28000, 28000, 28000, 28000, 28000, 28000, 28000, 27992, 27942, 27884, 27820, 27749, 27673, 27591, 27503, 27410, 27311, 27207, 27098, 26982, 26861, 26733, 26599, 26456, 26305, 26146, 25978, 25799, 25610, 25410, 25198, 24974, 24738, 24489, 24227, 23953, 23668, 23369, 23058, 22737, 22406, 22065, 21714, 21356, 20990, 20619, 20241, 19859, 19473, 19082, 18689, 18292, 17893, 17490, 17085, 16676, 16263, 15846, 15423, 14994, 14558, 14114, 13662, 13199, 12726, 12242, 11746, 11238, 10717, 10183, 9637, 9078, 8509, 7928, 7339, 6740, 6133, 5520, 4903, 4283, 3661, 3039, 2417, 1798, 1182, 569, -38, -642, -1240, -1836, -2426, -3015, -3602, -4191, -4779, -5373, -5972, -6579, -7196, -7824, -8466, -9123, -9795, -10485, -11192, -11916, -12658, -13415, -14186, -14969, -15761, -16558, -17356, -18153, -18940, -19715, -20472, -21204, -21905, -22570, -23192, -23768, -24289, -24753, -25153, -25486, -25748, -25939, -26053};
const int16_t PROGMEM HARMONIC_WAVE[256] = { 0, 3934, 7773, 11428, 14813, 17853, 20484, 22655, 24332, 25493, 26137, 26276, 25937, 25162, 24004, 22525, 20793, 18882, 16865, 14816, 12801, 10882, 9113, 7536, 6183, 5074, 4218, 3612, 3244, 3092, 3127, 3314, 3617, 3996, 4414, 4834, 5226, 5563, 5826, 6003, 6087, 6082, 5994, 5838, 5631, 5395, 5151, 4923, 4732, 4595, 4527, 4538, 4633, 4808, 5059, 5372, 5730, 6113, 6497, 6858, 7171, 7413, 7564, 7606, 7529, 7325, 6993, 6539, 5974, 5314, 4581, 3797, 2992, 2191, 1423, 714, 87, -440, -852, -1141, -1303, -1344, -1271, -1101, -851, -546, -209, 132, 451, 724, 929, 1048, 1067, 980, 783, 482, 85, -393, -932, -1511, -2104, -2685, -3228, -3710, -4109, -4407, -4592, -4655, -4595, -4415, -4124, -3736, -3270, -2747, -2192, -1628, -1080, -571, -122, 252, 537, 727, 820, 819, 735, 579, 370, 127, -127, -370, -579, -735, -819, -820, -727, -537, -252, 122, 571, 1080, 1628, 2192, 2747, 3270, 3736, 4124, 4415, 4595, 4655, 4592, 4407, 4109, 3710, 3228, 2685, 2104, 1511, 932, 393, -85, -482, -783, -980, -1067, -1048, -929, -724, -451, -132, 209, 546, 851, 1101, 1271, 1344, 1303, 1141, 852, 440, -87, -714, -1423, -2191, -2992, -3797, -4581, -5314, -5974, -6539, -6993, -7325, -7529, -7606, -7564, -7413, -7171, -6858, -6497, -6113, -5730, -5372, -5059, -4808, -4633, -4538, -4527, -4595, -4732, -4923, -5151, -5395, -5631, -5838, -5994, -6082, -6087, -6003, -5826, -5563, -5226, -4834, -4414, -3996, -3617, -3314, -3127, -3092, -3244, -3612, -4218, -5074, -6183, -7536, -9113, -10882, -12801, -14816, -16865, -18882, -20793, -22525, -24004, -25162, -25937, -26276, -26137, -25493, -24332, -22655, -20484, -17853, -14813, -11428, -7773, -3934, 0};
const int16_t PROGMEM PPG_WAVE[256] = {455, 4257, 12654, 21524, 27042, 29297, 30527, 30599, 28691, 25352, 22613, 20841, 19570, 18729, 19317, 21097, 23149, 24638, 25735, 26000, 24846, 21902, 17751, 12607, 6711, 399, -5419, -10409, -14393, -17262, -18601, -18616, -17616, -15827, -13201, -10328, -7929, -6346, -5257, -4661, -4831, -5865, -7209, -8331, -9023, -9326, -9135, -8295, -6692, -4347, -1469, 1531, 4149, 5977, 6679, 6172, 4685, 2771, 785, -1134, -3014, -4655, -5872, -6344, -5928, -4655, -3053, -1457, 104, 1848, 3452, 4607, 5265, 5768, 5967, 5738, 5238, 5046, 5120, 5251, 5386, 5936, 6781, 7676, 8446, 9285, 9879, 9979, 9593, 9073, 8135, 6588, 4663, 2955, 1254, -665, -2465, -3371, -3660, -3996, -4518, -4612, -4385, -4300, -4440, -4253, -3895, -3830, -4099, -4091, -3857, -3754, -3768, -3370, -2749, -2310, -1941, -1073, -66, 429, 542, 1154, 2163, 2759, 2616, 2285, 1759, 648, -904, -2015, -2541, -2872, -3015, -2419, -1410, -798, -685, -190, 817, 1685, 2054, 2493, 3114, 3512, 3498, 3601, 3835, 3843, 3574, 3639, 3997, 4184, 4044, 4129, 4356, 4262, 3740, 3404, 3115, 2209, 409, -1510, -3211, -4919, -6844, -8391, -9329, -9849, -10235, -10135, -9541, -8702, -7932, -7037, -6192, -5642, -5507, -5376, -5302, -5494, -5994, -6223, -6024, -5521, -4863, -3708, -2104, -360, 1201, 2797, 4399, 5672, 6088, 5616, 4399, 2758, 878, -1041, -3027, -4941, -6428, -6935, -6233, -4405, -1787, 1213, 4091, 6436, 8039, 8879, 9070, 8767, 8075, 6953, 5609, 4575, 4405, 5001, 6090, 7673, 10072, 12945, 15571, 17360, 18360, 18345, 17006, 14137, 10153, 5163, -655, -6966, -12863, -18007, -22158, -25102, -26256, -25991, -24894, -23405, -21353, -19573, -18985, -19826, -21097, -22869, -25608, -28947, -19538, -22187, -22187, -22383, -22054, -17988, -12202, 252};
const float AWFREQ = 172.0f;//Arbitrary waveform max frequency - NOT CURRENTLY USED
const float PWMRATE_PW_MODE = -10.0;
const float PWMRATE_SOURCE_FILTER_ENV = -5.0;
const float PWMRATE[128] = { PWMRATE_PW_MODE, PWMRATE_PW_MODE, PWMRATE_SOURCE_FILTER_ENV, PWMRATE_SOURCE_FILTER_ENV, PWMRATE_SOURCE_FILTER_ENV, PWMRATE_SOURCE_FILTER_ENV, PWMRATE_SOURCE_FILTER_ENV, 0.02f, 0.03f, 0.05f, 0.062f, 0.075f, 0.089f, 0.105f, 0.122f, 0.14f, 0.16f, 0.18f, 0.2f, 0.22f, 0.25f, 0.27f, 0.3f, 0.33f, 0.36f, 0.39f, 0.42f, 0.45f, 0.49f, 0.52f, 0.56f, 0.6f, 0.63f, 0.68f, 0.72f, 0.76f, 0.8f, 0.85f, 0.9f, 0.94f, 0.99f, 1.04f, 1.09f, 1.15f, 1.2f, 1.26f, 1.31f, 1.37f, 1.43f, 1.49f, 1.55f, 1.61f, 1.68f, 1.74f, 1.81f, 1.88f, 1.94f, 2.01f, 2.09f, 2.16f, 2.23f, 2.31f, 2.38f, 2.46f, 2.54f, 2.62f, 2.7f, 2.78f, 2.87f, 2.95f, 3.04f, 3.13f, 3.21f, 3.3f, 3.4f, 3.49f, 3.58f, 3.68f, 3.77f, 3.87f, 3.97f, 4.07f, 4.17f, 4.27f, 4.37f, 4.48f, 4.59f, 4.69f, 4.8f, 4.91f, 5.02f, 5.13f, 5.25f, 5.36f, 5.48f, 5.6f, 5.71f, 5.83f, 5.95f, 6.08f, 6.2f, 6.32f, 6.45f, 6.58f, 6.71f, 6.84f, 6.97f, 7.1f, 7.23f, 7.37f, 7.5f, 7.64f, 7.78f, 7.92f, 8.06f, 8.2f, 8.34f, 8.49f, 8.63f, 8.78f, 8.93f, 9.08f, 9.23f, 9.38f, 9.53f, 9.69f, 9.84f, 10.0f};
const float PITCHLFOOCTAVERANGE = 2.0f;//2 Oct range
const uint32_t MINUNISONVOICES = 3;
const  float  LFOMAXRATE = 40.0f;//40Hz
const uint8_t PWMSOURCELFO = 0;
const uint8_t PWMSOURCEFENV = 1;
const float  ONE = 1.0f;
const  float  SGTL_MAXVOLUME = 0.9f;
const  float  WAVEFORMLEVEL = ONE;
const float  VOICEMIXERLEVEL = 0.28f;
const float UNISONVOICEMIXERLEVEL = 0.17f;
const float UNISONNOISEMIXERLEVEL = 0.25f;
const float  OSCMODMIXERMAX = ONE;
const float  FILTERMODMIXERMAX = ONE;
const float  GLIDEFACTOR = 5000.0f;//Maximum glide time
const uint32_t NO_OF_PARAMS = 52;
const char* INITPATCHNAME = "Initial Patch";
const uint32_t HOLD_DURATION = 1000;
const uint32_t CLICK_DURATION = 250;
const float ENSEMBLE_LFO[128] = {2.0f, 2.1f, 2.3f, 2.4f, 2.6f, 2.7f, 2.9f, 3.0f, 3.1f, 3.3f, 3.4f, 3.6f, 3.7f, 3.8f, 4.0f, 4.1f, 4.3f, 4.4f, 4.6f, 4.7f, 4.8f, 5.0f, 5.1f, 5.3f, 5.4f, 5.6f, 5.7f, 5.8f, 6.0f, 6.1f, 6.3f, 6.4f, 6.5f, 6.7f, 6.8f, 7.0f, 7.1f, 7.3f, 7.4f, 7.5f, 7.7f, 7.8f, 8.0f, 8.1f, 8.2f, 8.4f, 8.5f, 8.7f, 8.8f, 9.0f, 9.1f, 9.2f, 9.4f, 9.5f, 9.7f, 9.8f, 10.0f, 10.1f, 10.2f, 10.4f, 10.5f, 10.7f, 10.8f, 10.9f, 11.1f, 11.2f, 11.4f, 11.5f, 11.7f, 11.8f, 11.9f, 12.1f, 12.2f, 12.4f, 12.5f, 12.7f, 12.8f, 12.9f, 13.1f, 13.2f, 13.4f, 13.5f, 13.6f, 13.8f, 13.9f, 14.1f, 14.2f, 14.4f, 14.5f, 14.6f, 14.8f, 14.9f, 15.1f, 15.2f, 15.3f, 15.5f, 15.6f, 15.8f, 15.9f, 16.1f, 16.2f, 16.3f, 16.5f, 16.6f, 16.8f, 16.9f, 17.1f, 17.2f, 17.3f, 17.5f, 17.6f, 17.8f, 17.9f, 18.0f, 18.2f, 18.3f, 18.5f, 18.6f, 18.8f, 18.9f, 19.0f, 19.2f, 19.3f, 19.5f, 19.6f, 19.8f, 19.9f, 20.0f};
const char* INITPATCH = "Solina,1.00,0.43,0.00,0,0,0.99,1.00,0.00,1.00,0.47,0.0,12,-12,12,12,0,0.83,0.70,0.16,0.00,0.00,1.10,282.00,0.00,0.70,0.00,7.24,0,0,0,10.48,0,0,0.00,1,4.00,1448.00,0.22,1864.00,41.00,808.00,0.92,991.00,5.60,0.83,0.00,0.0,0.0,0.0,0.0,0.0";