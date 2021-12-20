// takes uv value of pixel, texture, lighting at pixel: 
Vector4 color_at_pixel(Vector3 normal, vector<int> &tex, Vector2 uv, Light &l, Vector4 &col, Mat3 &rot, Vector3 &pos) 
{
	// counts number of textures applied to model: 
	int count = 0;
	Vector3 color = Vector3(1, 1, 1);

	for (int i = 0; i < 9; i++) 
	{
		if (tex[i] != -1) 
		{
			Texture& tex = loaded_textures[i];
			unsigned char* p = tex.img + ((int)(uv.y * tex.h) * tex.w + (int)(uv.x * tex.w)) * 4;
			Vector3 pixel = Vector3(p[0], p[1], p[2]);

			count++;

			switch (i)
			{
				// applies albedo map: 
				case 0: 
				{
					color = color * pixel;
				} break;

				// applies normal map: 
				case 1:
				{
					// normal = cross_product(normal, normalize_r((pixel * 2) - 1));
				} break;

				// applies other texture maps: 
				case 2:
				{

				} break;

				case 3:
				{

				} break;

				case 4:
				{

				} break;

				case 5:
				{

				} break;

				case 6:
				{

				} break;

				case 7:
				{

				} break;

				case 8:
				{

				} break;
			}
		}
	}

	// applies lighting value using per-pixel lighting: 
	Vector3 lighting = apply_directional_light(l, normal, col, rot, pos);

	// if there is no texture applied, return lighting value at pixel: 
	if (tex[0] == -1) color = color * 255;
	return Vector4(lighting * color, col.w);
}