#pragma once

//0 - Off, maximum velocity
//1 - Linear (0-127)
//2 - Curve up - (0-127)^2
//3 - Curve to flatten at 127 - (0-127)^0.5
//4 - Curve to flatten at 70, then maximum to 127 - - (0-70)^0.5
//
extern const float VELOCITY[5][128];
