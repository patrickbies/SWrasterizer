// Calculates vertex normals using face normals of connected faces: 
void generate_vertex_normals(vector<Vector3> &vertex,  vector<vector<int>> &vertex_index, vector<Vector3> &normal_list) 
{
	// clears the list of normals and replaces all values with empty Vector3s: 
	normal_list = vector<Vector3>(vertex.size(), Vector3());

	// loops through all triangles: 
	for (auto i : vertex_index) 
	{
		Vector3& p0 = vertex[i[0] - 1];
		Vector3& p1 = vertex[i[1] - 1];
		Vector3& p2 = vertex[i[2] - 1];

		// calculates face normal of the triangle: 
		Vector3 normal = cross_product(p0 - p1, p0 - p2);
		
		// adds face normal to total normal value of each vertex of the triangle: 
		normal_list[i[0] - 1] = normal_list[i[0] - 1] + normal;
		normal_list[i[1] - 1] = normal_list[i[1] - 1] + normal;
		normal_list[i[2] - 1] = normal_list[i[2] - 1] + normal;
	}
}

int parse_obj (const char *filename, bool smooth) 
{
	// opens .obj file: 
	FILE * f = fopen(filename, "r");
	if (!f) return 0;

	// defines vectors to store .obj file info in:
	vector<Vector3> vertex;
	vector<Vector2> uv;
	vector<Vector3> normal;
	
	vector<vector<int>> vertex_index;
	vector<vector<int>> uv_index;
	vector<vector<int>> normal_index;

	// defines new mesh to store all .obj values in: 
	Mesh new_mesh;

	// loops until the end of file has been reached: 
	while (true) 
	{
		char fword[128];
		int res = fscanf(f, "%s", fword);

		if (res == EOF) break;

		// checks first word of line: 
		if (strcmp(fword, "v") == 0) 
		{
			Vector3 temp_vertex;
			
			// scans line for floating point values: 
			fscanf(f, "%f %f %f\n", &temp_vertex.x, &temp_vertex.y, &temp_vertex.z);
			vertex.push_back(temp_vertex);
		}
		else if (strcmp(fword, "vt") == 0) 
		{
			Vector2 temp_uv;
			fscanf(f, "%f %f\n", &temp_uv.x, &temp_uv.y);
			uv.push_back(temp_uv);
		}
		else if (strcmp(fword, "vn") == 0) 
		{
			Vector3 temp_normal;
			fscanf(f, "%f %f %f\n", &temp_normal.x, &temp_normal.y, &temp_normal.z);
			normal.push_back(temp_normal);
		}
		
		// ***only works if faces are triangles and obj file includes uv, normal and face indicies*** : 
		else if (strcmp(fword, "f") == 0) 
		{
			vector<int> temp_face_index = { 0,0,0 }, temp_uv_index = { 0,0,0 }, temp_normal_index = { 0,0,0 };
			int t = fscanf_s(f, "%i/%i/%i %i/%i/%i %i/%i/%i\n",
				&temp_face_index[0], &temp_uv_index[0], &temp_normal_index[0],
				&temp_face_index[1], &temp_uv_index[1], &temp_normal_index[1],
				&temp_face_index[2], &temp_uv_index[2], &temp_normal_index[2]);

			vertex_index.push_back(temp_face_index);
			uv_index.push_back(temp_uv_index);
			normal_index.push_back(temp_normal_index);
		}
	}

	fclose(f);

	// sets new mesh info to scanned .obj file info: 
	new_mesh.uv = uv;
	new_mesh.uv_index = uv_index;

	new_mesh.vertex = vertex;
	new_mesh.vertex_index = vertex_index;

	// generates new vertex normals if smooth shading is enabled: 
	if (smooth) 
	{
		new_mesh.normal_index = vertex_index;
		generate_vertex_normals(vertex, vertex_index, new_mesh.normal);

		for (auto& i : new_mesh.normal) normalize(i);
	}
	else
	{
		new_mesh.normal_index = normal_index;
		new_mesh.normal = normal;
	}

	// fill 9 texture ids with empty texture ids: 
	new_mesh.texture_id = { -1, -1, -1, -1, -1, -1, -1, -1, -1 };

	// add new mesh to loaded_objs: 
	loaded_mesh.push_back(new_mesh);

	return 1;
}

/*

Basis of parser implementation is from: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/

*/