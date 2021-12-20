// Texture struct to store image rgba values, width, height and number of channels: 
struct Texture
{
	unsigned char* img;
	int w, h, c;
};

// Mesh struct to share mesh data: 
struct Mesh
{
	vector<Vector3>vertex;
	vector<Vector2>uv;
	vector<Vector3>normal;

	// how different mesh textures are stored:
	// index 0: Albedo
	// index 1: Normal
	// index 2:	Roughness
	// index 3: Metalness
	// index 4: Specular
	// index 5:	Height
	// index 6: Opacity
	// index 7: Ambient occlusion
	// index 8: Refraction
	// index 9: Self-illumination
	vector<int>texture_id;

	vector<vector<int>>vertex_index;
	vector<vector<int>>uv_index;
	vector<vector<int>>normal_index;
};

// Object struct to store object info: 
struct Object
{
	Vector3 p;
	int id;
	float scale;

	Mat3 rotation;
	Vector3 angles;

	Vector4 color;

	int mesh_id;
};