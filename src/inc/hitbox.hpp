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
