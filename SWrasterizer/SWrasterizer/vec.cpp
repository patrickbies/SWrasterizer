// vector arithmetic and operators:
// Vector3 struct to hold 3 float values: 
struct Vector3
{
	float x, y, z;

	Vector3(float x0, float y0, float z0)
	{
		x = x0;
		y = y0;
		z = z0;
	}

	Vector3()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	// Perspective divide for perspective projection of 3d points:
	void perspective_divide()
	{
		x = ((pixel_info.w * x) / z) + pixel_info.w / 2;
		y = ((pixel_info.w * y) / z) + pixel_info.h / 2;
	}
};

// Vector4 struct to hold 4 floats
struct Vector4
{
	float x, y, z, w;

	Vector4(float x0, float y0, float z0, float w0)
	{
		x = x0;
		y = y0;
		z = z0;
		w = w0;
	}

	Vector4(Vector3 a, float w0)
	{
		x = a.x;
		y = a.y;
		z = a.z;
		w = w0;
	}

	Vector4()
	{
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}
};

// Vector2 struct to hold 2 floats: 
struct Vector2 
{
	float x, y;

	Vector2(float x0, float y0) 
	{
		x = x0;
		y = y0;
	}
	Vector2()
	{
		x = 0;
		y = 0;
	}
};

// Mat3 struct to hold 3x3 Matrix (generally for transformations): 
struct Mat3 
{
	Vector3 r0, r1, r2;

	Mat3(Vector3 rr0, Vector3 rr1, Vector3 rr2)
	{
		r0 = rr0;
		r1 = rr1;
		r2 = rr2;
	}

	Mat3()
	{
		r0 = Vector3(0, 0, 0);
		r1 = Vector3(0, 0, 0);
		r2 = Vector3(0, 0, 0);
	}
};

// Vector3 subtraction and addition:
Vector3 operator- (Vector3 p, Vector3 n)
{ return Vector3(p.x - n.x, p.y - n.y, p.z - n.z); }

Vector3 operator- (Vector3 p) 
{ return Vector3(-p.x, -p.y, -p.z); }

Vector3 operator+ (Vector3 p, Vector3 n) 
{ return Vector3(p.x + n.x, p.y + n.y, p.z + n.z); }

// multiplying vector3 and matrix:
Vector3 operator* (Vector3 p, Mat3 n)
{
	return Vector3(
		(p.x * n.r0.x) + (p.y * n.r1.x) + (p.z * n.r2.x),
		(p.x * n.r0.y) + (p.y * n.r1.y) + (p.z * n.r2.y),
		(p.x * n.r0.z) + (p.y * n.r1.z) + (p.z * n.r2.z));
}

// Hadamard multiplication of Vector3s:
Vector3 operator* (Vector3 p, Vector3 n) 
{ return Vector3(p.x * n.x, p.y * n.y, p.z * n.z); }

// Vector3 multiply by integer: 
Vector3 operator* (Vector3 p, int n) 
{ return Vector3(p.x * n, p.y * n, p.z * n); }

// Vector3 multiply by float: 
Vector3 operator* (Vector3 p, float n)
{ return Vector3(p.x * n, p.y * n, p.z * n); }

Vector3 operator* (float n, Vector3 p)
{ return Vector3(p.x * n, p.y * n, p.z * n); }

// Vector3 divide by integer:
Vector3 operator/ (Vector3 p, int n) 
{ return Vector3(p.x/n, p.y/n, p.z/n); }

Vector3 operator/ (Vector3 p, float n)
{ return Vector3(p.x / n, p.y / n, p.z / n); }

// Vector2 multiply by integer:
Vector2 operator* (Vector2 p, int n) 
{ return Vector2(p.x * n, p.y * n); }

Vector3 operator+ (Vector3 p, int n)
{ return Vector3(p.x + n, p.y + n, p.z + n); }

Vector3 operator/ (int n, Vector3 p)
{ return Vector3(n / p.x, n / p.y, n / p.z); }

Vector3 operator- (Vector3 n, int p) 
{ return Vector3(n.x - p, n.y - p, n.z - p); }

Vector2 operator- (Vector2 n, Vector2 p)
{ return Vector2(n.x - p.x, n.y - p.y); }

// Cross product and Dot product of Vector3s: 
inline Vector3 cross_product(Vector3 p, Vector3 n)
{
	return Vector3(
		p.y * n.z - p.z * n.y,
		p.z * n.x - p.x * n.z,
		p.x * n.y - p.y * n.x);
}

inline float dot_product(Vector3 p, Vector3 n) 
{ return (p.x * n.x) + (p.y * n.y) + (p.z * n.z); }

// Clamps all values of a vector between two values:
inline Vector3 clamp_vec(float min, float max, Vector3 val)
{
	return Vector3(
		(val.x > max) ? max : (val.x < min) ? min : val.x,
		(val.y > max) ? max : (val.y < min) ? min : val.y,
		(val.z > max) ? max : (val.z < min) ? min : val.z);
}

unsigned int vec_uint(Vector3 val)
{ return (unsigned int) (((int)val.x << 8) + (int) val.y << 8) + (int) val.z; }

Vector3 uint_vec(unsigned int col)
{ return Vector3((col & 0xff0000) >> 16, (col & 0xff00) >> 8, col & 0xff); }

// Normalizes the vector: 
void normalize(Vector3& normal) 
{ 
	normal = normal / (float) sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));
}

Vector3 normalize_r(Vector3 normal)
{
	return normal / (float) sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));
}