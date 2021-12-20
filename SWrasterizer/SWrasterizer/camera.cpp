// Camera struct to hold info relatating to the Camera: 
struct Camera 
{
	// x,y,z position and velocity of camera: 
	Vector3 position;
	Vector3 velocity;

	// Rotation matrix of the Camera: 
	Mat3 rotation;

	// Bool values to keep track of key input: 
	bool w = false, a = false, s = false, d = false;

	// Angle and angle change of Camera:
	Vector3 angle;
	Vector3 ang_vel;

	Camera(Vector3 ang, Vector3 pos) 
	{
		angle = ang;
		position = pos;
	}

	void update_cam()
	{
		// Updates camera position based on velocity: 
		position = position + velocity;

		// Updates camera angle based on angle change values: 
		angle.x += (angle.x - ang_vel.x < -3.14 / 2 || angle.x + ang_vel.x > 3.14 / 2) ? 0 : ang_vel.x;
		angle.y += ang_vel.y;

		// key input for combined input: 
		if (a && s)
		{
			velocity.z = 0.05 * cos(angle.y - 135 * 3.14 / 180);
			velocity.x = 0.05 * sin(angle.y - 135 * 3.14 / 180);
		}
		else if (a && w)
		{
			velocity.z = 0.05 * cos(angle.y - 45 * 3.14 / 180);
			velocity.x = 0.05 * sin(angle.y - 45 * 3.14 / 180);
		}
		else if (d && s)
		{
			velocity.z = 0.05 * cos(angle.y + 135 * 3.14 / 180);
			velocity.x = 0.05 * sin(angle.y + 135 * 3.14 / 180);
		}
		else if (d && w)
		{
			velocity.z = 0.05 * cos(angle.y + 45 * 3.14 / 180);
			velocity.x = 0.05 * sin(angle.y + 45 * 3.14 / 180);
		}

		// general key input for camera x,z movement: 
		else if (s)
		{
			velocity.z = -0.05 * cos(angle.y);
			velocity.x = -0.05 * sin(angle.y);
		}
		else if (w)
		{
			velocity.z = 0.05 * cos(angle.y);
			velocity.x = 0.05 * sin(angle.y);
		}
		else if (a)
		{
			velocity.z = 0.05 * cos(angle.y - 90 * 3.14 / 180);
			velocity.x = 0.05 * sin(angle.y - 90 * 3.14 / 180);
		}
		else if (d)
		{
			velocity.z = 0.05 * cos(angle.y + 90 * 3.14 / 180);
			velocity.x = 0.05 * sin(angle.y + 90 * 3.14 / 180);
		}
	}

};