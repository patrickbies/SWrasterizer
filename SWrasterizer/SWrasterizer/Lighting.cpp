struct Light 
{
	Vector3 normal, diffuse, ambient, color;

	Light(Vector3 n, Vector3 d, Vector3 a, Vector3 c) 
	{
		normal = n;
		diffuse = d;
		ambient = a;
		color = c;
	}
};

Vector3 apply_directional_light(Light &l, Vector3 &n, Vector4 &col, Mat3 &rot, Vector3 &pos) 
{
	Vector3 refl = normalize_r(l.normal - 2 * (max(0, dot_product(normalize_r(l.normal), normalize_r(n * rot)))) * (n * rot));
	Vector3 dist = normalize_r(camera.position * camera.rotation - pos * rot);
	Vector3 spec = Vector3(col.x, col.y, col.z) * (float) (powf(dot_product(refl, dist), 32.0) * 0.5);
	return clamp_vec(0, 1, Vector3(col.x, col.y, col.z) * (spec + l.ambient + (l.diffuse * max(0, -dot_product(n * rot, -l.normal)))));
}