// States of buttons.

enum button_state
{
	button_disabled,

	button_default,

	button_hover
};

// Generate, draw and destroy a button.

void draw_button(int x, int y, int x_res, int y_res, int width, int factor, button_state state)
{
	gpu_sprite* btn_0;
	gpu_sprite* btn_1;
	gpu_sprite* btn_2;

	// The left bumper. The left bumper is a 2 by 20 rectangle that reflects 
	// the state of the button and is not affected by the button's width.

	btn_0 = generate_rectangle_gpu_sprite(x, y, x + 2 * factor, y + 20 * factor, 0.0f, float(state) * 20.0f / 60.0f, 2.0f / 200.0f, (float(state) + 1.0f) * 20.0f / 60.0f, x_res, y_res, interface_textures.at("buttons")->storage);

	// The right bumper. The right bumper is a 2 by 20 rectangle that reflects 
	// the state of the button and is not affected by the button's width.

	btn_1 = generate_rectangle_gpu_sprite(x + (width - 2) * factor, y, x + width * factor, y + 20 * factor, 198.0f / 200.0f, float(state) * 20.0f / 60.0f, 1.0f, (float(state) + 1.0f) * 20.0f / 60.0f, x_res, y_res, interface_textures.at("buttons")->storage);

	// The center block. The center block is a width - 4 by 20 rectangle that
	// reflects the state of the button and is affected by the button's width.

	btn_2 = generate_rectangle_gpu_sprite(x + 2 * factor, y, x + (width - 2) * factor, y + 20 * factor, 2.0f / 200.0f, float(state) * 20.0f / 60.0f, (float(width) - 2) / 200.0f, (float(state) + 1.0f) * 20.0f / 60.0f, x_res, y_res, interface_textures.at("buttons")->storage);

	draw_destroy_gpu_sprite(btn_0);
	draw_destroy_gpu_sprite(btn_1);
	draw_destroy_gpu_sprite(btn_2);
}