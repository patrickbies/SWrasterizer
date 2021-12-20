// depth buffer of objects on screen: 
vector<vector<float>> depth_buffer;

// Lighting values of scene: 
Light l(
	Vector3(0, 0, 0),
	Vector3(0.75, 0.75, 0.75),
	Vector3(0.25, 0.25, 0.25), 
	Vector3(1, 1, 1));

// angles of light and objects: 
float xa = 0, ya = 0, za = 0;
float al = 0, bl = 0, cl = 0;

Mesh vertex_shader(Object &obj) 
{
	// loop through all verticies: 
	Mesh m = loaded_mesh[obj.mesh_id];
	for (int i = 0; i < m.vertex.size(); i++) 
	{
		// apply camera rotation and object translation to object:
		m.vertex[i] = Vector3(m.vertex[i].x * obj.scale, m.vertex[i].y * obj.scale, m.vertex[i].z * obj.scale);
		m.vertex[i] = ((m.vertex[i] * obj.rotation) + obj.p - camera.position) * camera.rotation;
	}
	// returns new mesh with updated verticies: 
	return m;
}

// draws triangles using screen positions from vertex shader and face connections: 
void rasterize(Mesh m, Object &obj) 
{
	// loops through all triangles: 
	for (int i = 0; i < m.vertex_index.size(); i++)
	{
		// stores verticies of the triangle: 
		Vector3 p0 = m.vertex[m.vertex_index[i][0] - 1], p1 = m.vertex[m.vertex_index[i][1] - 1], p2 = m.vertex[m.vertex_index[i][2] - 1];

		// checks if face is back face: 
		if (dot_product(cross_product(p1 - p0, p2 - p0), p0) <= 0) 
		{

			// project points in 3D space with perspective divide:
			p0.perspective_divide();
			p1.perspective_divide();
			p2.perspective_divide();

			// calculates lighting values to interpolate across the triangle: 
			Vector3 n0 = m.normal[m.normal_index[i][0] - 1],
				n1 = m.normal[m.normal_index[i][1] - 1],
				n2 = m.normal[m.normal_index[i][2] - 1];
			
			// rasterizes triangle from points p0, p1, p2 interpolation uv values and lighting values: 
			g.draw_tri(
				Vector4(p0.x, p0.y, p0.z, 1 / p0.z), Vector4(p1.x, p1.y, p1.z, 1 / p1.z), Vector4(p2.x, p2.y, p2.z, 1 / p2.z),
				n0, n1, n2,
				m.uv[m.uv_index[i][0] - 1], m.uv[m.uv_index[i][1] - 1], m.uv[m.uv_index[i][2] - 1],
				m.texture_id, obj.color, obj.rotation, l
			);
		}
	}
}

// sends objects through graphics pipeline: 
void update_objects() 
{
	// defines camera rotation matrix: 
	camera.rotation = Mat3(rotation_matrix(camera.angle.z, camera.angle.y, camera.angle.x));

	// calculates light vector using al, bl, cl angles: 
	l.normal = normalize_r(Vector3(-cos(al)*sin(bl)*sin(cl) - sin(al)*cos(cl),
		-sin(al)*sin(bl)*sin(cl) + cos(al)*cos(cl),
		cos(bl)*sin(cl)));

	// loops through each object: 
	for (Object i : objects) 
	{
		// sets the rotation matrix of the object ( temporarily increasing each objet by xa, ya, za ): 
		i.rotation = Mat3(rotation_matrix(i.angles.x + xa, i.angles.y + ya, i.angles.z + za));

		// rasterizes object
		rasterize(vertex_shader(i), i);
	}
}

// adds object of a certain type at an id, x, y, z, scale and orientation: 
void addObject(int id, int index, float scale, Vector3 pos, Vector3 angles, Vector4 color) 
{
	Object n;
			
	n.mesh_id = index;
	n.p = pos;
	n.id = id; 
	n.scale = scale;
	n.angles = angles;
	n.color = color;

	objects.push_back(n);
}