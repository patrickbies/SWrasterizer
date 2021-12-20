#define _CRT_SECURE_NO_DEPRECATE

#include <Windows.h>
#include <math.h>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using std::vector;

bool running = true;

// Values of near and far plane for depth buffer: 
float far_plane = 25, near_plane = 0.1;

// Stores bitmap pixel info, width and height of the screen: 
struct Render_State 
{
	int h, w;
	void* memory;

	BITMAPINFO bmp_info;
};

Render_State pixel_info;

#include "vec.cpp"
#include "utilities.cpp"

#include "camera.cpp"

// New Camera: 
Camera camera(Vector3(0, 0, 0), Vector3(0, 0, -10));

#include "Lighting.cpp"

#include "Object.cpp"

// stores loaded objs, textures and objects:
vector<Object> objects{};
vector<Mesh> loaded_mesh{};
vector<Texture> loaded_textures{};

#include "output_merger.cpp"

#include "render.cpp"

Render g;

#include "object_handler.cpp"

#include "load_tex.cpp"
#include "load_obj.cpp"
#include "key_input.cpp"


// Defines window callback functions: 
LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	LRESULT result = 0;

	// reads window messages and handles them
	switch (uMsg) 
	{
		// stops main loop on destroy/close
		case WM_CLOSE:
		case WM_DESTROY: running = false;
			break;

		// updates rs values to respond with window resizing:
		case WM_SIZE: 
		{
			RECT rect;
			
			// Sets new width and height after screen resize:
			GetClientRect(hwnd, &rect);
			pixel_info.w = rect.right - rect.left;
			pixel_info.h = rect.bottom - rect.top;

			int size = pixel_info.w * pixel_info.h * sizeof(unsigned long int);

			// Reset depth buffer to fit new screen size:
			g.db = vector<vector<float>>(pixel_info.w, vector<float>(pixel_info.h, far_plane));
			g.ab = vector<vector<float>>(pixel_info.w, vector<float>(pixel_info.h, 1));

			// Allocates enough memory to store resized window to store pixel value:
			if (pixel_info.memory) VirtualFree(pixel_info.memory, 0, MEM_RELEASE);
			pixel_info.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

			// Defines new bmp info accordingly to work with resize:
			pixel_info.bmp_info.bmiHeader.biSize = sizeof(pixel_info.bmp_info.bmiHeader);
			pixel_info.bmp_info.bmiHeader.biWidth = pixel_info.w;
			pixel_info.bmp_info.bmiHeader.biHeight = pixel_info.h;
			pixel_info.bmp_info.bmiHeader.biPlanes = 1;
			pixel_info.bmp_info.bmiHeader.biBitCount = 32;
			pixel_info.bmp_info.bmiHeader.biCompression = BI_RGB;

		} break;

		// Handles key input: 
		case WM_KEYDOWN: key_down(wParam); 
			break;
		case WM_KEYUP: key_up(wParam);
			break;
		default: result = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) 
{
	// Defines Window class:
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = L"Game Window Class";
	window_class.lpfnWndProc = window_callback;

	// Registers the window class:
	RegisterClass(&window_class);

	// 'f' for no light interpolation, flat shading, 's' for light interpolation (smooth shading)
	// 'v' to only render verticies, 'w' for wireframe only
	// 'n' for normal buffer, 'd' for depth buffer, 'a' for alpha buffer
	g.mode = 's';

	// background color: 
	g.bgc = Vector4(172, 222, 238, 1);

	// temp example textured object adding and loading: 
	// parse_obj("path", (bool) generate_vertex_normals);
	// load_texture("path", mesh_id, texture_type);
	// addObject(id, loaded_index, scale, Vector3 position, Vector3 angles, Vector4 color);

	// Creates the window:
	HWND window = CreateWindow(window_class.lpszClassName, L"3D Render", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window);

	// loops until running is false: 
	while (running) 
	{
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		// Updates Scene:
		g.clear();
			
		update_objects();
		camera.update_cam();

		// Renders BMP pixel info to window: 
		StretchDIBits(hdc, 0, 0, pixel_info.w, pixel_info.h, 0, 0, pixel_info.w, pixel_info.h, pixel_info.memory, &pixel_info.bmp_info, DIB_RGB_COLORS, SRCCOPY);
	}
}
/*

Help from Dan Zaidan on youtube to create window and render graphics using cpp, most window related code is adapted from his tutorial.

*/