// Edge struct used with triangle rasteriation: 
struct Edge 
{
	float a, b, c;

	Edge(Vector4 v0, Vector4 v1) 
	{
		a = v0.y - v1.y;
		b = v1.x - v0.x;
		c = -(a * (v0.x + v1.x) + b * (v0.y + v1.y)) / 2;
	}

	Edge()
	{
		a = 0;
		b = 0;
		c = 0;
	}
};

// interpolates values across tri using edge values: 
Vector3 interpolate_tri(float area, float interpolate0, float interpolate1, float interpolate2, Edge e0, Edge e1, Edge e2) 
{
	float fac = 1 / (2 * area);

	return Vector3(fac * (interpolate1 * e2.a + interpolate0 * e1.a + interpolate2 * e0.a),
		fac * (interpolate1 * e2.b + interpolate0 * e1.b + interpolate2 * e0.b),
		fac * (interpolate1 * e2.c + interpolate0 * e1.c + interpolate2 * e0.c));
}

// simple triangle struct to hold edge and pixel color info:
// also holds values to interpolate, depth, lighting, uvs
struct Triangle 
{
	Vector3 normal_x, normal_y, normal_z,
		texture_coordinate_x, texture_coordinate_y,
		depth;

	Edge e0, e1, e2;
	float area;

	Triangle(Vector4 p0, Vector4 p1, Vector4 p2, Vector2 uv0, Vector2 uv1, Vector2 uv2, Vector3 n0, Vector3 n1, Vector3 n2) 
	{
		e0 = Edge(p1, p0);
		e1 = Edge(p2, p1);
		e2 = Edge(p0, p2);

		// Area of tri: 
		area = (e0.c + e1.c + e2.c)/2;

		// Depth interpolation of tri: 
		depth = interpolate_tri(area, p0.z, p1.z, p2.z, e0, e1, e2);

		// Light interpolation of tri: 
		normal_x = interpolate_tri(area, n0.x, n1.x, n2.x, e0, e1, e2);
		normal_y = interpolate_tri(area, n0.y, n1.y, n2.y, e0, e1, e2);
		normal_z = interpolate_tri(area, n0.z, n1.z, n2.z, e0, e1, e2);

		// Interpolates uv values:
		texture_coordinate_x = interpolate_tri(area, uv0.x, uv1.x, uv2.x,  e0, e1, e2);
		texture_coordinate_y = interpolate_tri(area, uv0.y, uv1.y, uv2.y, e0, e1, e2);
	}
};

inline float eval_vec(Vector3 vec, Vector3 p) 
{ return vec.x * p.x + vec.y * p.y + vec.z; }

// tests if pixel is within edge: 
inline bool edge_test(Edge edge, Vector3 vec)
{
	float eval = edge.a * vec.x + edge.b * vec.y + edge.c;
	return eval > 0 || eval == 0 && ((edge.a != 0) ? edge.a > 0 : edge.b > 0);
}

// stores all render functions: 
struct Render 
{
	// render mode and background color: 
	int mode;
	Vector4 bgc;

	// defines depth buffer and alpha buffer: 
	vector<vector<float>> db;
	vector<vector<float>> ab;

	void plot(Vector3 c, Vector4 color, bool ignore_depth)
	{	
		unsigned int* pixel;
		Vector3 prev;

		if ((c.z < db[c.x][c.y] && c.z > near_plane && color.w > 0) || ignore_depth)
		{
			pixel = (unsigned int*)pixel_info.memory + (int)c.x + (int)c.y * pixel_info.w;
			prev = uint_vec(*((unsigned int*)pixel_info.memory + (int)c.x + (int)c.y * pixel_info.w));

			db[c.x][c.y] = c.z;
			ab[c.x][c.y] = color.w;

			Vector3 col = Vector3(color.x, color.y, color.z) * color.w + (prev * (1 - color.w));

			// checks if pixel exists, then colors it
			if (pixel) *pixel = vec_uint(col);
		}
		else if (ab[c.x][c.y] < 1) {
			pixel = (unsigned int*)pixel_info.memory + (int)c.x + (int)c.y * pixel_info.w;
			prev = uint_vec(*((unsigned int*)pixel_info.memory + (int)c.x + (int)c.y * pixel_info.w));

			Vector3 col = Vector3(color.x, color.y, color.z) * ab[c.x][c.y] + Vector3(prev.x, prev.y, prev.z) * (1 - ab[c.x][c.y]);

			if (pixel) *pixel = vec_uint(Vector3(col));
		}
	}

	void clear()
	{
		for (int y = 0; y < pixel_info.h; y++)
		{
			for (int x = 0; x < pixel_info.w; x++)
			{
				if (mode == 'd') plot(Vector3(x, y, far_plane), Vector4(0, 0, 0, 1), true);
				else plot(Vector3(x, y, far_plane), bgc, true);
			}
		}
	}

	void draw_tri(Vector4 p0, Vector4 p1, Vector4 p2,
		Vector3 n0, Vector3 n1, Vector3 n2,
		Vector2 u0, Vector2 u1, Vector2 u2, vector<int>texture_id, Vector4 col, Mat3 rot, Light l)
	{
		// defines new triangle and checks for valid area: 
		Triangle tri = Triangle(p0, p1, p2, u0, u1, u2, n0, n1, n2);
		if (tri.area < 0) return;

		// bounding box and screen clipping of tri:
		float min_x = clamp(0, pixel_info.w - 1, minval(p0.x, p1.x, p2.x)),
			min_y = clamp(0, pixel_info.h - 1, minval(p0.y, p1.y, p2.y)),
			max_x = clamp(0, pixel_info.w - 1, maxval(p0.x, p1.x, p2.x)),
			max_y = clamp(0, pixel_info.h - 1, maxval(p0.y, p1.y, p2.y));

		// defines lighting if flat shaded:
		Vector3 normal = (n0 + n1 + n2) / 3;

		// loops through all positions in bounding box:
		Vector3 t;
		for (t.x = min_x + 0.5; t.x <= max_x + 0.5; t.x++)
		{
			for (t.y = min_y + 0.5; t.y <= max_y + 0.5; t.y++)
			{
				// tests if t is within the triangle: 
				if (edge_test(tri.e0, t) && edge_test(tri.e1, t) && edge_test(tri.e2, t))
				{
					// interpolates the z value at pixel t: 
					t.z = eval_vec(tri.depth, t);

					switch (mode) 
					{
						// interpolates light values across the triangle: 
						case 's': 
							normal = Vector3(eval_vec(tri.normal_x, t), eval_vec(tri.normal_y, t), eval_vec(tri.normal_z, t));
							break;

						case 'v':
						{
							plot(t, Vector4(1, 1, 1, 1), false); return;
						} break;

						case 'd':
						{
							float depth_color = (1 - ((t.z - near_plane) / (far_plane - near_plane))) * 255;
							plot(t, Vector4(depth_color, depth_color, depth_color, col.w), false);
							continue;
						} break;

						case 'a':
						{
							float alpha = col.w * 255;
							plot(t, Vector4(alpha, alpha, alpha, col.w), false);
							continue;
						} break;

						case 'n':
						{
							plot(t, Vector4((Vector3(0.5, 0.5, 0.5) + Vector3(eval_vec(tri.normal_x, t), eval_vec(tri.normal_y, t), eval_vec(tri.normal_z, t)) / 2) * 255, 1), false);
							continue;
						} break;
					}
					if (mode == 'w') 
					{
						plot(t, Vector4(1, 1, 1, 1), false); break;
					}
					// find texture pixel color at uv coordinates: 
					plot(t, color_at_pixel(normal, texture_id, Vector2(eval_vec(tri.texture_coordinate_x, t), eval_vec(tri.texture_coordinate_y, t)), l, col, rot, t), false);
				}
			}
		}
	}
};


/*

triangle rasterization is my implementation of an algorithm on the 'scratchapixel' website
help from https://trenki2.github.io/blog/2017/06/06/developing-a-software-renderer-part1/ as well
and https://www.researchgate.net/publication/286441992_Accelerated_Half-Space_Triangle_Rasterization

*/
