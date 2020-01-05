#define MAX_GUI2 4096
#define GUI2_MEMORY (6 * 6 * MAX_GUI)

float* gui2_data;
GLuint gui2_vao;
GLuint gui2_vbo;

void gui2_init() {
	gui2_data = new float[GUI2_MEMORY];
	glGenVertexArrays(1, &gui2_vao);
	glGenBuffers(1, &gui2_vbo);
	glBindVertexArray(gui2_vao);
	glBindBuffer(GL_ARRAY_BUFFER, gui2_vbo);
	glBufferData(GL_ARRAY_BUFFER, GUI2_MEMORY * sizeof(float), gui2_data, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
}

float gui2_x_res;
float gui2_y_res;
unsigned int gui2_pointer;

void gui2_init_frame(int x_res, int y_res) {
	gui2_x_res = x_res;
	gui2_y_res = y_res;
	gui2_pointer = 0;
}

void gui2_vtx(float x, float y, float u, float v, float w, float coeff) {
	gui2_data[gui2_pointer++] = x;
	gui2_data[gui2_pointer++] = y;
	gui2_data[gui2_pointer++] = u;
	gui2_data[gui2_pointer++] = 1.0f - v;
	gui2_data[gui2_pointer++] = w;
	gui2_data[gui2_pointer++] = coeff;
}

void gui2(float x_, float y_, float scale, block_id id) {
	if (is_crop(id) || is_cross(id) || is_fire(id)) {
		float index = block_face_info[id]->l_top;
		if (id == id_water || id == id_fire) {
			index *= -1.0f;
		}

		y_ = gui2_y_res - y_;
		float x = x_ / gui2_x_res * 2.0f - 1.0f;
		float y = y_ / gui2_y_res * 2.0f - 1.0f;
		float w = 16.0f / gui2_x_res * 2.0f * scale;
		float h = -16.0f / gui2_y_res * 2.0f * scale;

		// top-left
		gui2_vtx(x, y, 0.0f, 1.0f, index, 1.0f);
		// top-right
		gui2_vtx(x + w, y, 1.0f, 1.0f, index, 1.0f);
		// bottom-left
		gui2_vtx(x, y + h, 0.0f, 0.0f, index, 1.0f);
		// bottom-left
		gui2_vtx(x, y + h, 0.0f, 0.0f, index, 1.0f);
		// top-right
		gui2_vtx(x + w, y, 1.0f, 1.0f, index, 1.0f);
		// bottom-right
		gui2_vtx(x + w, y + h, 1.0f, 0.0f, index, 1.0f);
	} else {
		// Oh no, it's three-dimensional time. Don't try to understand this code.
		y_ = gui2_y_res - y_;
		float ndcx = x_ / gui2_x_res * 2.0f - 1.0f;
		float ndcy = y_ / gui2_y_res * 2.0f - 1.0f;
		float ndcw = 16.0f / gui2_x_res * 2.0f * scale;
		float ndch = -16.0f / gui2_y_res * 2.0f * scale;

		#define ISOVTX(x) (ndcx + x * ndcw)
		#define ISOVTY(y) (ndcy + y * ndch)
		float vtl_x = ISOVTX(16.0f / 300.0f);
		float vtl_y = ISOVTY(68.0f / 300.0f);
		float vtr_x = ISOVTX(284.0f / 300.0f);
		float vtr_y = ISOVTY(68.0f / 300.0f);
		float vbl_x = ISOVTX(16.0f / 300.0f);
		float vbl_y = ISOVTY(232.0f / 300.0f);
		float vbr_x = ISOVTX(284.0f / 300.0f);
		float vbr_y = ISOVTY(232.0f / 300.0f);
		float vtc_x = ISOVTX(150.0f / 300.0f);
		float vtc_y = ISOVTY(0.0f / 300.0f);
		float vcc_x = ISOVTX(150.0f / 300.0f);
		float vcc_y = ISOVTY(136.0f / 300.0f);
		float vbc_x = ISOVTX(150.0f / 300.0f);
		float vbc_y = ISOVTY(300.0f / 300.0f);

		float hiy = 1.0f;
		if (is_slab(id)) {
			#define ISOSHIFT(y) y += 82.0f / 300.0f * ndch
			ISOSHIFT(vtl_y);
			ISOSHIFT(vtr_y);
			ISOSHIFT(vtc_y);
			ISOSHIFT(vcc_y);
			hiy = 0.5f;
		}

		float sign = 1.0f;
		if (id == id_water || id == id_fire) {
			sign = -1.0f;
		}

		#define ISOV(x, y, u, v, i, l) gui2_vtx(x, y, u, 1.0f - v, sign * i, l)

		// q-top
		float qtl = 1.0f;
		float qti = block_face_info[id]->l_top;
		ISOV(vtl_x, vtl_y, 0.0f, 1.0f, qti, qtl);
		ISOV(vtc_x, vtc_y, 0.0f, 0.0f, qti, qtl);
		ISOV(vtr_x, vtr_y, 1.0f, 0.0f, qti, qtl);
		ISOV(vtl_x, vtl_y, 0.0f, 1.0f, qti, qtl);
		ISOV(vcc_x, vcc_y, 1.0f, 1.0f, qti, qtl);
		ISOV(vtr_x, vtr_y, 1.0f, 0.0f, qti, qtl);

		// q-left
		float qll = 0.9f;
		float qli = block_face_info[id]->l_front;
		ISOV(vtl_x, vtl_y, 0.0f, 0.0f, qli, qll);
		ISOV(vcc_x, vcc_y, 1.0f, 0.0f, qli, qll);
		ISOV(vbl_x, vbl_y, 0.0f, hiy, qli, qll);
		ISOV(vbl_x, vbl_y, 0.0f, hiy, qli, qll);
		ISOV(vcc_x, vcc_y, 1.0f, 0.0f, qli, qll);
		ISOV(vbc_x, vbc_y, 1.0f, hiy, qli, qll);

		// q-right
		float qrl = 0.7f;
		float qri = block_face_info[id]->l_right;
		ISOV(vcc_x, vcc_y, 0.0f, 0.0f, qri, qrl);
		ISOV(vtr_x, vtr_y, 1.0f, 0.0f, qri, qrl);
		ISOV(vbc_x, vbc_y, 0.0f, hiy, qri, qrl);
		ISOV(vbc_x, vbc_y, 0.0f, hiy, qri, qrl);
		ISOV(vtr_x, vtr_y, 1.0f, 0.0f, qri, qrl);
		ISOV(vbr_x, vbr_y, 1.0f, hiy, qri, qrl);
	}
}

void gui2_draw_all(GLuint array2d) {
	GLint id;
	glGetIntegerv(GL_CURRENT_PROGRAM, &id);
	glUniform1f(glGetUniformLocation(id, "time_in_seconds"), SDL_GetTicks() / 1000.0f);

	glEnable(GL_TEXTURE_2D_ARRAY);
	glBindTexture(GL_TEXTURE_2D_ARRAY, array2d);
	glBindVertexArray(gui2_vao);
	glBindBuffer(GL_ARRAY_BUFFER, gui2_vbo);
	glBufferData(GL_ARRAY_BUFFER, GUI2_MEMORY * sizeof(float), gui2_data, GL_DYNAMIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, gui2_pointer / 6);
	glBindVertexArray(0);
}