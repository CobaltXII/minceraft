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

#define eps 0.00128f

float hitbox_x_depth(hitbox a, hitbox b)
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

float hitbox_y_depth(hitbox a, hitbox b)
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

