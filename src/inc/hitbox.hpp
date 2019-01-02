#include <vector>

// An axis-aligned bounding box.

struct hitbox
{
	float x;
	float y;
	float z;

	float xr;
	float yr;
	float zr;

	hitbox
	(
		float _x, 
		float _y, 
		float _z, 

		float _xr, 
		float _yr, 
		float _zr
	)
	{
		x = _x;
		y = _y;
		z = _z;

		xr = _xr;
		yr = _yr;
		zr = _zr;
	}	
};

// Check if two hitboxes intersect.

inline bool hitbox_intersect(hitbox a, hitbox b)
{
	return
	(
		(a.x <= b.x + b.xr && a.x + a.xr >= b.x) &&
		(a.y <= b.y + b.yr && a.y + a.yr >= b.y) &&
		(a.z <= b.z + b.zr && a.z + a.zr >= b.z)
	);
}

// Get the X depth of two intersecting hitboxes.

inline float hitbox_x_depth(hitbox a, hitbox b, float eps = 0.00128f)
{
	if (a.x + a.xr > b.x + b.xr)
	{
		return (b.x + b.xr) - a.x + eps;
	}
	else
	{
		return b.x - (a.x + a.xr) - eps;
	}
}

// Get the Y depth of two intersecting hitboxes.

inline float hitbox_y_depth(hitbox a, hitbox b, float eps = 0.00128f)
{
	if (a.y + a.yr > b.y + b.yr)
	{
		return (b.y + b.yr) - a.y + eps;
	}
	else
	{
		return b.y - (a.y + a.yr) - eps;
	}
}

// Get the Z depth of two intersecting hitboxes.

inline float hitbox_z_depth(hitbox a, hitbox b, float eps = 0.00128f)
{
	if (a.z + a.zr > b.z + b.zr)
	{
		return (b.z + b.zr) - a.z + eps;
	}
	else
	{
		return b.z - (a.z + a.zr) - eps;
	}
}

// Do collision detection and response on one dynamic hitbox with an array of 
// several static hitboxes while applying velocity to the dynamic hitbox.

void do_collision_detection_and_response(hitbox& object, std::vector<hitbox> obstacles, float& vx, float& vy, float& vz, int precision = 64)
{
	for (int p = 0; p < precision; p++)
	{
		object.x += vx / precision;

		for (int i = 0; i < obstacles.size(); i++)
		{
			hitbox obstacle = obstacles[i];

			if (hitbox_intersect(object, obstacle))
			{
				object.x += hitbox_x_depth(object, obstacle);

				vx = 0.0f;
			}
		}

		object.y += vy / precision;

		for (int i = 0; i < obstacles.size(); i++)
		{
			hitbox obstacle = obstacles[i];

			if (hitbox_intersect(object, obstacle))
			{
				object.y += hitbox_y_depth(object, obstacle);

				vy = 0.0f;
			}
		}

		object.z += vz / precision;

		for (int i = 0; i < obstacles.size(); i++)
		{
			hitbox obstacle = obstacles[i];

			if (hitbox_intersect(object, obstacle))
			{
				object.z += hitbox_z_depth(object, obstacle);

				vz = 0.0f;
			}
		}
	}
}