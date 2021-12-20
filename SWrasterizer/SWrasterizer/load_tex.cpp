// loads a texture at a path to a mesh for the type of texture: 
void load_texture(const char* path, int mesh_id, int tex_type)
{
	Mesh &m = loaded_mesh[mesh_id];

	stbi_set_flip_vertically_on_load(true);
	Texture texture;
	texture.img = stbi_load(path, &texture.w, &texture.h, &texture.c, 4);
	loaded_textures.push_back(texture);

	m.texture_id[tex_type] = loaded_textures.size() - 1;
}