int gui4_mx;
int gui4_my;
bool gui4_lbp;
bool gui4_rbp;
bool gui4_lbd;
bool gui4_rbd;

void gui4_init_frame(int x_res, int y_res, int mx, int my, bool lbp, bool rbp, bool lbd, bool rbd) {
	gui_init_frame(x_res, y_res);
	gui3_init_frame(x_res, y_res);
	gui4_mx = mx;
	gui4_my = my;
	gui4_lbp = lbp;
	gui4_rbp = rbp;
	gui4_lbd = lbd;
	gui4_rbd = rbd;
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
	if (gui4_mx >= x && gui4_mx <= x + w * scale &&
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
	if (o != br_nothing) {
		gui3_active_string(x + w / 2.0f * scale - std::round(gui3_measure(text) / 2.0f) * scale, y + GUI_BUTTON_H / 2.0f * scale - 4.0f * scale, scale, text);
	} else {
		gui3_shadowed_string(x + w / 2.0f * scale - std::round(gui3_measure(text) / 2.0f) * scale, y + GUI_BUTTON_H / 2.0f * scale - 4.0f * scale, scale, text);
	}
	return o;
}

button_result gui4_toggle(std::string text, float x, float y, float w, float scale, bool* toggle) {
	float voff = GUI_BUTTON_DEFAULT;
	button_result o = br_nothing;
	if (gui4_mx >= x && gui4_mx <= x + w * scale &&
		gui4_my >= y + scale && gui4_my <= y + GUI_BUTTON_H * scale + scale) {
		o = br_hovered;
		voff = GUI_BUTTON_HOVERED;
		if (gui4_lbp) {
			o = br_pressed_left;
			voff = GUI_BUTTON_PRESSED;
			*toggle = !*toggle;
		} else if (gui4_rbp) {
			o = br_pressed_right;
			voff = GUI_BUTTON_PRESSED;
			*toggle = !*toggle;
		}
	}
	text = text + ": " + (*toggle ? "ON" : "OFF");
	gui_button(x, y, w, voff, scale);
	if (o != br_nothing) {
		gui3_active_string(x + w / 2.0f * scale - std::round(gui3_measure(text) / 2.0f) * scale, y + GUI_BUTTON_H / 2.0f * scale - 4.0f * scale, scale, text);
	} else {
		gui3_shadowed_string(x + w / 2.0f * scale - std::round(gui3_measure(text) / 2.0f) * scale, y + GUI_BUTTON_H / 2.0f * scale - 4.0f * scale, scale, text);
	}
	return o;
}

button_result gui4_slider(std::string text, float x, float y, float w, float scale, float* slider) {
	float slider_width = 8.0f;
	float voff = GUI_BUTTON_DEFAULT;
	button_result o = br_nothing;
	if (gui4_mx >= x && gui4_mx <= x + w * scale &&
		gui4_my >= y + scale && gui4_my <= y + GUI_BUTTON_H * scale + scale) {
		o = br_hovered;
		voff = GUI_BUTTON_HOVERED;
		if (gui4_lbd) {
			o = br_pressed_left;
		} else if (gui4_rbd) {
			o = br_pressed_right;
		}
		if (o != br_hovered) {
			float q = float(gui4_mx) - x - slider_width / 2.0f * scale;
			if (q < 0.0f) {
				q = 0.0f;
			} else if (q > (w - slider_width) * scale) {
				q = (w - slider_width) * scale;
			}
			*slider = q / ((w - slider_width) * scale);
		}
	}
	gui_button(x, y, w, GUI_BUTTON_PRESSED, scale);
	gui_button(x + std::round((w - slider_width) * *slider) * scale, y, slider_width, voff, scale);
	if (o != br_nothing) {
		gui3_active_string(x + w / 2.0f * scale - std::round(gui3_measure(text) / 2.0f) * scale, y + GUI_BUTTON_H / 2.0f * scale - 4.0f * scale, scale, text);
	} else {
		gui3_shadowed_string(x + w / 2.0f * scale - std::round(gui3_measure(text) / 2.0f) * scale, y + GUI_BUTTON_H / 2.0f * scale - 4.0f * scale, scale, text);
	}
	return o;
}

button_result gui4_button_disabled(std::string text, float x, float y, float w, float scale) {
	gui_button(x, y, w, GUI_BUTTON_PRESSED, scale);
	gui3_disabled_string(x + w / 2.0f * scale - std::round(gui3_measure(text) / 2.0f) * scale, y + GUI_BUTTON_H / 2.0f * scale - 4.0f * scale, scale, text);
	return br_nothing;
}

void gui4_draw_all(gui_texture& buttons, gui_texture& font, GLuint qp, GLuint tp) {
	glUseProgram(qp);
	gui_draw_all(buttons);
	glUseProgram(tp);
	gui3_draw_all(font);
}