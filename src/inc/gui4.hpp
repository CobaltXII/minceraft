int gui4_mx;
int gui4_my;
bool gui4_lbp;
bool gui4_rbp;

void gui4_init_frame(int x_res, int y_res, int mx, int my, bool lbp, bool rbp) {
	gui_init_frame(x_res, y_res);
	gui3_init_frame(x_res, y_res);
	gui4_mx = mx;
	gui4_my = my;
	gui4_lbp = lbp;
	gui4_rbp = rbp;
}

enum button_result {
	br_nothing,
	br_hovered,
	br_pressed_left,
	br_pressed_right
};

bool button_pressed(button_result x) {
	return x == br_pressed_left || x == br_pressed_right;
}

button_result gui4_button(std::string text, float x, float y, float w, float scale) {
	float voff = GUI_BUTTON_DEFAULT;
	button_result o = br_nothing;
	if (gui4_mx >= x + scale && gui4_mx <= x + w * scale + scale &&
		gui4_my >= y + scale && gui4_my <= y + GUI_BUTTON_H * scale + scale) {
		o = br_hovered;
		voff = GUI_BUTTON_HOVERED;
		if (gui4_lbp) {
			o = br_pressed_left;
			voff = GUI_BUTTON_PRESSED;
		} else if (gui4_rbp) {
			o = br_pressed_right;
			voff = GUI_BUTTON_PRESSED;
		}
	}
	gui_button(x, y, w, voff, scale);
	gui3_shadowed_string(x + w / 2.0f * scale - gui3_measure(text) / 2.0f * scale, y + GUI_BUTTON_H / 2.0f * scale - 4.0f * scale, scale, text);
	return o;
}

void gui4_draw_all(gui_texture& buttons, gui_texture& font, GLuint qp, GLuint tp) {
	glUseProgram(qp);
	gui_draw_all(buttons);
	glUseProgram(tp);
	gui3_draw_all(font);
}