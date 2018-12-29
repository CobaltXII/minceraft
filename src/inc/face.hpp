// A struct of type face_info represents the layer (w coordinate) of each face
// of a block. It is used to map the correct texture on to each face of a 
// block.

struct face_info
{
	float l_top;

	float l_bottom;

	float l_left;

	float l_right;

	float l_front;

	float l_back;

	face_info(float _l_top, float _l_bottom, float _l_left, float _l_right, float _l_front, float _l_back)
	{
		l_top = _l_top;

		l_bottom = _l_bottom;

		l_left = _l_left;

		l_right = _l_right;

		l_front = _l_front;

		l_back = _l_back;
	}
};
// Generates a face_info* where each all of the six faces correspond to the
// same layer.

face_info* make_face_info_all(float all)
{
	return new face_info(all, all, all, all, all, all);
}
