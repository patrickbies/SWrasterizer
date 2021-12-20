// Handles key presses: 
void key_down(int key) 
{
	switch (key) 
	{
		// Camera movement:
		case 'W': camera.w = true; break;
		case 'A': camera.a = true; break;
		case 'S': camera.s = true; break;
		case 'D': camera.d = true; break;
		case VK_SPACE: camera.velocity.y = 0.25; break;
		case VK_SHIFT: camera.velocity.y = -0.25; break;

		// Camera rotation: 
		case VK_UP: camera.ang_vel.x = -0.01; break;
		case VK_DOWN: camera.ang_vel.x = 0.01; break;
		case VK_LEFT: camera.ang_vel.y = -0.01; break;
		case VK_RIGHT: camera.ang_vel.y = 0.01; break;

		// Light source movement: 
		case 'I': al += 0.1; break;
		case 'J': al -= 0.1; break;
		case 'K': bl += 0.1; break;
		case 'L': bl -= 0.1; break;
		case 'O': cl += 0.1; break;
		case 'P': cl -= 0.1; break;

		// Object rotation: 
		case 'T': xa += 0.1; break;
		case 'G': xa -= 0.1; break;
		case 'F': ya += 0.1; break;
		case 'H': ya -= 0.1; break;
		case 'Y': za += 0.1; break;
		case 'U': za -= 0.1; break;
	}
}

// Handles key releases: 
void key_up(int key) 
{
	switch (key) 
	{
		// camera movement: 
		case 'S': camera.velocity.z = 0; camera.velocity.x = 0; camera.s = false; break;
		case 'W': camera.velocity.z = 0; camera.velocity.x = 0; camera.w = false; break;
		case 'A': camera.velocity.z = 0; camera.velocity.x = 0; camera.a = false; break;
		case 'D': camera.velocity.z = 0; camera.velocity.x = 0; camera.d = false; break;
		case VK_SPACE: camera.velocity.y = 0; break;
		case VK_SHIFT: camera.velocity.y = 0; break;

		// camera rotation: 
		case VK_UP: camera.ang_vel.x = 0; break;
		case VK_DOWN: camera.ang_vel.x = 0; break;
		case VK_LEFT: camera.ang_vel.y = 0;break;
		case VK_RIGHT: camera.ang_vel.y = 0; break;
	};
}