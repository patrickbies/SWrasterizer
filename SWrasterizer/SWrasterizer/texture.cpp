struct BMP {
	BITMAPFILEHEADER BMPfh[sizeof(BITMAPFILEHEADER)];
	BITMAPINFOHEADER BMPih[sizeof(BITMAPINFOHEADER)];
	vector<Vector3> bmp_data;

	void load_bmp(const char* path) {
        FILE* f = fopen(path, "rb");

        fread(BMPfh, sizeof(BITMAPFILEHEADER), 1, f);
        fread(BMPih, sizeof(BITMAPINFOHEADER), 1, f);

        int size = 3 * BMPih->biWidth * BMPih->biHeight;
        unsigned char* data = new unsigned char[size];

        fread(data, sizeof(unsigned char), size, f);
        fclose(f);

        for (int i = 0; i < size; i += 3)
            bmp_data.push_back(Vector3(data[i + 2], data[i + 1], data[i]));
    }
};	

struct Texture {
	BMP texture, bumb_map, normal_map, ao_map;
};