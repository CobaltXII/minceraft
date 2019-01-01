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
