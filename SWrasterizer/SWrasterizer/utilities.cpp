// Minval and Maxval return the minimum and maximum values of three inputs: 
inline int minval(int a, int b, int c) 
{ return min(a, b) > c ? c : min(a, b); }

inline int maxval(int a, int b, int c) 
{ return max(a, b) < c ? c : max(a, b); }

// Function to clamp values between specified min and max:
inline int clamp(int min, int max, int value) 
{
	if (value > min && value < max) return value;
	else return value > max ? max : min;
}

inline float clamp(float min, float max, float value) 
{
	if (value > min && value < max) return value;
	else return value > max ? max : min;
}

// General Rotation matrix to rotate a point about the z,y,x axes at a,b,c angles: 
inline Mat3 rotation_matrix(float a, float b, float c) 
{
	return Mat3(
		Vector3(cosf(a) * cosf(b), (cosf(a) * sinf(b) * sinf(c)) - (sinf(a) * cosf(c)), (cosf(a) * sinf(b) * cosf(c)) + (sinf(a) * sinf(c))),
		Vector3(sinf(a) * cosf(b), (sinf(a) * sinf(b) * sinf(c)) + (cosf(a) * cosf(c)), (sinf(a) * sinf(b) * cosf(c)) - (cosf(a) * sinf(c))),
		Vector3(-sinf(b), cosf(b) * sinf(c), cosf(b) * cosf(c)));
}
