const unsigned int char_widths[] = {
	0, 8, 8, 7, 7, 7, 7, 6, 8, 7, 8, 8, 7, 8, 8, 8,
	7, 7, 7, 7, 8, 8, 7, 8, 7, 7, 7, 7, 7, 8, 8, 8,
	5, 1, 4, 5, 5, 5, 5, 2, 4, 4, 4, 5, 1, 5, 1, 5,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1, 1, 4, 5, 4, 5,
	6, 5, 5, 5, 5, 5, 5, 5, 5, 3, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 3, 5, 3, 5, 5,
	2, 5, 5, 5, 5, 5, 4, 5, 5, 1, 5, 4, 2, 5, 5, 5,
	5, 5, 5, 5, 3, 5, 5, 5, 5, 5, 5, 4, 1, 4, 6, 5,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 3, 5, 2, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 3, 5,
	5, 2, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5, 5, 1, 5, 5,
	7, 8, 8, 5, 5, 5, 7, 7, 5, 7, 7, 7, 7, 7, 5, 5,
	8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8, 8, 5, 8, 8, 8, 4, 8, 8,
	7, 6, 6, 7, 6, 7, 7, 7, 6, 7, 7, 6, 8, 8, 5, 6,
	6, 6, 6, 6, 8, 5, 6, 7, 6, 5, 5, 8, 6, 5, 6, 0
};

#define MAX_GUI3 4096
#define GUI3_MEMORY (6 * 7 * MAX_GUI3)

float* gui3_data;
GLuint gui3_vao;
GLuint gui3_vbo;

void gui3_init() {
	gui3_data = new float[GUI3_MEMORY];
	glGenVertexArrays(1, &gui3_vao);
	glGenBuffers(1, &gui3_vbo);
	glBindVertexArray(gui3_vao);
	glBindBuffer(GL_ARRAY_BUFFER, gui3_vbo);
	glBufferData(GL_ARRAY_BUFFER, GUI3_MEMORY * sizeof(float), gui3_data, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(4 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
}

float gui3_x_res;
float gui3_y_res;
unsigned int gui3_pointer;

void gui3_init_frame(int x_res, int y_res) {
	gui3_x_res = x_res;
	gui3_y_res = y_res;
	gui3_pointer = 0;
}

void gui3_vtx(float x, float y, float u, float v, float r, float g, float b) {
	gui3_data[gui3_pointer++] = x;
	gui3_data[gui3_pointer++] = y;
	gui3_data[gui3_pointer++] = u;
	gui3_data[gui3_pointer++] = 1.0f - v;
	gui3_data[gui3_pointer++] = r;
	gui3_data[gui3_pointer++] = g;
	gui3_data[gui3_pointer++] = b;
}

void gui3_char(float x_, float y_, float scale, char c, float r, float g, float b) {
	float i = float(c % 16);
	float j = float(c / 16);
	float u0 = i / 16.0f;
	float v0 = j / 16.0f;
	float u1 = (i * 8.0f + float(char_widths[c])) / 128.0f;
	float v1 = (j + 1.0f) / 16.0f;

	y_ = gui3_y_res - y_;
	float x = x_ / gui3_x_res * 2.0f - 1.0f;
	float y = y_ / gui3_y_res * 2.0f - 1.0f;
	float w = float(char_widths[c]) / gui3_x_res * 2.0f * scale;
	float h = -8.0f / gui3_y_res * 2.0f * scale;

	gui3_vtx(x, y, u0, v0, r, g, b);
	gui3_vtx(x + w, y, u1, v0, r, g, b);
	gui3_vtx(x, y + h, u0, v1, r, g, b);
	gui3_vtx(x, y + h, u0, v1, r, g, b);
	gui3_vtx(x + w, y, u1, v0, r, g, b);
	gui3_vtx(x + w, y + h, u1, v1, r, g, b);
}

void gui3_char(float x_, float y_, float scale, char c, float coeff) {
	gui3_char(x_, y_, scale, c, coeff, coeff, coeff);
}

void gui3_string(float x, float y, float scale, std::string text) {
	for (int i = 0; i < text.length(); i++) {
		gui3_char(x, y, scale, text[i], 1.0f);
		x += float(char_widths[text[i]] + 1) * scale;
	}
}

void gui3_shadowed_string(float x, float y, float scale, std::string text) {
	float p = x;
	for (int i = 0; i < text.length(); i++) {
		gui3_char(p + scale, y + scale, scale, text[i], 0.219607843f);
		p += float(char_widths[text[i]] + 1) * scale;
	}
	float q = x;
	for (int i = 0; i < text.length(); i++) {
		gui3_char(q, y, scale, text[i], 1.0f);
		q += float(char_widths[text[i]] + 1) * scale;
	}
}

float gui3_measure(std::string text) {
	float o = 0.0f;
	for (int i = 0; i < text.length(); i++) {
		o += float(char_widths[text[i]] + 1);
	}
	o -= 1.0f;
	if (o < 0.0f) {
		o = 0.0f;
	}
	return o;
}

void gui3_draw_all(gui_texture& font) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, font.id);
	glBindVertexArray(gui3_vao);
	glBindBuffer(GL_ARRAY_BUFFER, gui3_vbo);
	glBufferData(GL_ARRAY_BUFFER, GUI3_MEMORY * sizeof(float), gui3_data, GL_DYNAMIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, gui3_pointer / 7);
	glBindVertexArray(0);
}