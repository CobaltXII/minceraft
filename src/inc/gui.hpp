#define MAX_GUI 4096
#define GUI_MEMORY (6 * 4 * MAX_GUI)

#define GUI_BUTTON_W 200.0f
#define GUI_BUTTON_H 20.0f

#define GUI_BUTTON_PRESSED (0.0f / 3.0f)
#define GUI_BUTTON_DEFAULT (1.0f / 3.0f)
#define GUI_BUTTON_HOVERED (2.0f / 3.0f)

float* gui_data;
GLuint gui_vao;
GLuint gui_vbo;

void gui_init() {
	gui_data = new float[GUI_MEMORY];
	glGenVertexArrays(1, &gui_vao);
	glGenBuffers(1, &gui_vbo);
	glBindVertexArray(gui_vao);
	glBindBuffer(GL_ARRAY_BUFFER, gui_vbo);
	glBufferData(GL_ARRAY_BUFFER, GUI_MEMORY * sizeof(float), gui_data, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}

float gui_x_res;
float gui_y_res;
unsigned int gui_pointer;

void gui_init_frame(int x_res, int y_res) {
	gui_x_res = x_res;
	gui_y_res = y_res;
	gui_pointer = 0;
}

void gui_vtx(float x, float y, float u, float v) {
	gui_data[gui_pointer++] = x;
	gui_data[gui_pointer++] = y;
	gui_data[gui_pointer++] = u;
	gui_data[gui_pointer++] = v;
}

void gui(float x_, float y_, float scale, gui_texture& tex) {
	y_ = gui_y_res - y_;
	float x = x_ / gui_x_res * 2.0f - 1.0f;
	float y = y_ / gui_y_res * 2.0f - 1.0f;
	float w = tex.w / gui_x_res * 2.0f * scale;
	float h = -tex.h / gui_y_res * 2.0f * scale;

	// top-left
	gui_vtx(x, y, 0.0f, 1.0f);
	// top-right
	gui_vtx(x + w, y, 1.0f, 1.0f);
	// bottom-left
	gui_vtx(x, y + h, 0.0f, 0.0f);
	// bottom-left
	gui_vtx(x, y + h, 0.0f, 0.0f);
	// top-right
	gui_vtx(x + w, y, 1.0f, 1.0f);
	// bottom-right
	gui_vtx(x + w, y + h, 1.0f, 0.0f);
}

void gui(float x_, float y_, float w_, float h_, float scale) {
	y_ = gui_y_res - y_;
	float x = x_ / gui_x_res * 2.0f - 1.0f;
	float y = y_ / gui_y_res * 2.0f - 1.0f;
	float w = w_ / gui_x_res * 2.0f * scale;
	float h = -h_ / gui_y_res * 2.0f * scale;

	// top-left
	gui_vtx(x, y, 0.0f, 1.0f);
	// top-right
	gui_vtx(x + w, y, 1.0f, 1.0f);
	// bottom-left
	gui_vtx(x, y + h, 0.0f, 0.0f);
	// bottom-left
	gui_vtx(x, y + h, 0.0f, 0.0f);
	// top-right
	gui_vtx(x + w, y, 1.0f, 1.0f);
	// bottom-right
	gui_vtx(x + w, y + h, 1.0f, 0.0f);
}

void gui(float x_, float y_, float w_, float h_, float u0, float v0, float u1, float v1, float scale) {
	y_ = gui_y_res - y_;
	v0 = 1.0f - v0;
	v1 = 1.0f - v1;
	float x = x_ / gui_x_res * 2.0f - 1.0f;
	float y = y_ / gui_y_res * 2.0f - 1.0f;
	float w = w_ / gui_x_res * 2.0f * scale;
	float h = -h_ / gui_y_res * 2.0f * scale;

	// top-left
	gui_vtx(x, y, u0, v0);
	// top-right
	gui_vtx(x + w, y, u1, v0);
	// bottom-left
	gui_vtx(x, y + h, u0, v1);
	// bottom-left
	gui_vtx(x, y + h, u0, v1);
	// top-right
	gui_vtx(x + w, y, u1, v0);
	// bottom-right
	gui_vtx(x + w, y + h, u1, v1);
}

void gui_button(float x_, float y_, float w_, float voff, float scale) {
	// left-segment
	//     start: 0px
	//     end: 2px
	//     ustart: 0px
	//     uend: 2px
	// middle-segment
	//     start: 2px
	//     end: 2px + w_ - 4px
	//     ustart: 2px
	//     uend: 2px + w_ - 4px
	// end-segment
	//     start: 2px + w_ - 4px
	//     end: 2px + w_ - 2px
	//     ustart: w_ - 2px
	//     uend: w_

	// left-segment
	gui(x_, y_, 2.0f, GUI_BUTTON_H, 0.0f, voff, 2.0f / GUI_BUTTON_W, voff + GUI_BUTTON_DEFAULT, scale);
	// middle-segment
	gui(x_ + 2.0f * scale, y_, w_ - 4.0f, GUI_BUTTON_H, 2.0f / GUI_BUTTON_W, voff, (w_ - 2.0f) / GUI_BUTTON_W, voff + GUI_BUTTON_DEFAULT, scale);
	// right-segment
	gui(x_ + (w_ - 2.0f) * scale, y_, 2.0f, GUI_BUTTON_H, 1.0f - 2.0f / GUI_BUTTON_W, voff, 1.0f, voff + GUI_BUTTON_DEFAULT, scale);
}

void gui_draw_all(gui_texture& tex) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex.id);
	glBindVertexArray(gui_vao);
	glBindBuffer(GL_ARRAY_BUFFER, gui_vbo);
	glBufferData(GL_ARRAY_BUFFER, GUI_MEMORY * sizeof(float), gui_data, GL_DYNAMIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, gui_pointer / 4);
	glBindVertexArray(0);
}