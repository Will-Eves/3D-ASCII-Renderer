#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#define _WIN32_WINNT 0x0500
#include <windows.h>

#define CG_WIN_MIN 20
#define CG_WIN_MAX 60

namespace CG {
	struct RenderBuffer {
		int width;
		int height;
		int size;
		char* buffer;
	};

	struct DepthBuffer {
		int width;
		int height;
		int size;
		float* buffer;
	};

	struct Int3 {
		int x;
		int y;
		int z;
	};

	class Vector3 {
	public:
		float x;
		float y;
		float z;

		Vector3() {
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		}

		Vector3(float _x, float _y, float _z) {
			x = _x;
			y = _y;
			z = _z;
		}

		Vector3 operator + (Vector3 other) {
			return Vector3(x + other.x, y + other.y, z + other.z);
		}

		Vector3 operator - (Vector3 other) {
			return Vector3(x - other.x, y - other.y, z - other.z);
		}

		Vector3 operator * (Vector3 other) {
			return Vector3(x * other.x, y * other.y, z * other.z);
		}

		Vector3 operator / (Vector3 other) {
			return Vector3(x / other.x, y / other.y, z / other.z);
		}

		void Rotate(float pitch, float yaw, float roll) {
			float cosa = cos(yaw);
			float sina = sin(yaw);

			float cosb = cos(pitch);
			float sinb = sin(pitch);

			float cosc = cos(roll);
			float sinc = sin(roll);

			float Axx = cosa * cosb;
			float Axy = cosa * sinb * sinc - sina * cosc;
			float Axz = cosa * sinb * cosc + sina * sinc;

			float Ayx = sina * cosb;
			float Ayy = sina * sinb * sinc + cosa * cosc;
			float Ayz = sina * sinb * cosc - cosa * sinc;

			float Azx = -sinb;
			float Azy = cosb * sinc;
			float Azz = cosb * cosc;

			float px = x;
			float py = y;
			float pz = z;

			x = Axx * px + Axy * py + Axz * pz;
			y = Ayx * px + Ayy * py + Ayz * pz;
			z = Azx * px + Azy * py + Azz * pz;
		}
	};

	class Face {
	public:
		Vector3 v1;
		Vector3 v2;
		Vector3 v3;
		char color;

		Face() {}

		Face(Vector3 _v1, Vector3 _v2, Vector3 _v3, char _color) {
			v1 = _v1;
			v2 = _v2;
			v3 = _v3;
			color = _color;
		}
	};

	class Mesh {
	public:
		Face* faces;
		int faceCount;

		Mesh() {}
		
		Mesh(Face* _faces, int _faceCount) {
			faces = _faces;
			faceCount = _faceCount;
		}

		Mesh(std::string filename, char color='#') {
			std::vector<Vector3> verts;
			std::vector<Int3> faces1;

			std::string s;
			std::ifstream fin(filename);
			if (!fin) return;
			while (fin >> s){
				switch (*s.c_str()){
					case 'v':{
						Vector3 v;
						fin >> v.x >> v.y >> v.z;
						verts.push_back(v);
					}
					break;
					case 'f':{
						Int3 f;
						int a;
						fin >> f.x >> a >> a >> f.y >> a >> a >> f.z >> a >> a;
						f.x--;
						f.y--;
						f.z--;
						faces1.push_back(f);
					}
					break;
				}
			}

			int i = faces1.size();

			Vector3* verts1 = new Vector3[i];
			std::copy(verts.begin(), verts.end(), verts1);

			faces = new Face[i];

			for (int a = 0; a < i; a++) {
				Face f(verts1[faces1[a].x], verts1[faces1[a].y], verts1[faces1[a].z], color);
				faces[a] = f;
			}
		}
	};

	void fillTri3d(RenderBuffer buffer, DepthBuffer dbuffer, Vector3 v1, Vector3 v2, Vector3 v3, char color);
	void fillLine3d(RenderBuffer buffer, Vector3 v1, Vector3 v2, char color);

	class MeshRenderer {
	public:
		Mesh mesh;
		RenderBuffer buffer;
		DepthBuffer dbuffer;

		Vector3 position = Vector3(0, 0, 0);
		Vector3 rotation = Vector3(0, 0, 0);
		Vector3 scale = Vector3(1, 1, 1);

		MeshRenderer(Mesh _mesh, RenderBuffer _buffer, DepthBuffer _dbuffer) {
			mesh = _mesh;
			buffer = _buffer;
			dbuffer = _dbuffer;
		}

		void Render() {
			for (int i = 0; i < mesh.faceCount; i++) {
				Face face = mesh.faces[i];
				face.v1 = face.v1 * scale;
				face.v2 = face.v2 * scale;
				face.v3 = face.v3 * scale;
				
				face.v1.Rotate(rotation.x, rotation.y, rotation.z);
				face.v2.Rotate(rotation.x, rotation.y, rotation.z);
				face.v3.Rotate(rotation.x, rotation.y, rotation.z);

				fillTri3d(buffer, dbuffer, position + face.v1, position + face.v2, position + face.v3, face.color);

				face.v1.Rotate(-rotation.x, -rotation.y, -rotation.z);
				face.v2.Rotate(-rotation.x, -rotation.y, -rotation.z);
				face.v3.Rotate(-rotation.x, -rotation.y, -rotation.z);

				face.v1 = face.v1 / scale;
				face.v2 = face.v2 / scale;
				face.v3 = face.v3 / scale;
			}
		}

		void WireRender() {
			for (int i = 0; i < mesh.faceCount; i++) {
				Face face = mesh.faces[i];
				face.v1 = face.v1 * scale;
				face.v2 = face.v2 * scale;
				face.v3 = face.v3 * scale;

				face.v1.Rotate(rotation.x, rotation.y, rotation.z);
				face.v2.Rotate(rotation.x, rotation.y, rotation.z);
				face.v3.Rotate(rotation.x, rotation.y, rotation.z);

				fillLine3d(buffer, face.v1, face.v2, face.color);
				fillLine3d(buffer, face.v2, face.v3, face.color);
				fillLine3d(buffer, face.v3, face.v1, face.color);

				face.v1.Rotate(-rotation.x, -rotation.y, -rotation.z);
				face.v2.Rotate(-rotation.x, -rotation.y, -rotation.z);
				face.v3.Rotate(-rotation.x, -rotation.y, -rotation.z);

				face.v1 = face.v1 / scale;
				face.v2 = face.v2 / scale;
				face.v3 = face.v3 / scale;
			}
		}
	};

	class Camera {
	public:
		Vector3 position = Vector3(0, 0, 0);
		Vector3 rotation = Vector3(0, 0, 0);

		RenderBuffer buffer;
		DepthBuffer dbuffer;

		Camera() {}

		Camera(RenderBuffer _buffer, DepthBuffer _dbuffer) {
			buffer = _buffer;
			dbuffer = _dbuffer;
		}

		void Render(MeshRenderer mr) {
			mr.position = mr.position - position;

			Vector3 fixedRot = Vector3(rotation.y, rotation.z, rotation.x);

			mr.position.Rotate(-fixedRot.x, -fixedRot.y, -fixedRot.z);
			mr.rotation = mr.rotation - fixedRot;
			mr.Render();
			mr.rotation = mr.rotation + fixedRot;
			mr.position.Rotate(fixedRot.x, fixedRot.y, rotation.z);
			mr.position = mr.position + position;
		}

		void WireRender(MeshRenderer mr) {
			mr.position = mr.position - position;

			Vector3 fixedRot = Vector3(rotation.y, rotation.z, rotation.x);

			mr.position.Rotate(-fixedRot.x, -fixedRot.y, -fixedRot.z);
			mr.rotation = mr.rotation - fixedRot;
			mr.WireRender();
			mr.rotation = mr.rotation + fixedRot;
			mr.position.Rotate(fixedRot.x, fixedRot.y, rotation.z);
			mr.position = mr.position + position;
		}
	};

	Camera mainCamera;

	int width;
	int height;

	const int res = 13;

	char colors[8] = {
		' ',
		'.',
		',',
		'*',
		'o',
		'f',
		'#',
		'@',
	};

	void setup(int w, int h) {
		width = w;
		height = h;

		if (width < CG_WIN_MIN) width = CG_WIN_MIN;
		else if (width > CG_WIN_MAX) width = CG_WIN_MAX;
		if (height < CG_WIN_MIN) height = CG_WIN_MIN;
		else if (height > CG_WIN_MAX) height = CG_WIN_MAX;

		//Set Font Size
		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = res;
		cfi.dwFontSize.Y = res;
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;
		SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

		//Change Window Size
		HWND console = GetConsoleWindow();
		RECT r;
		GetWindowRect(console, &r);
		MoveWindow(console, r.left, r.top, width * res, height * res + res + 20, TRUE);

		//Remove Scrollbar
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO scrBufferInfo;
		GetConsoleScreenBufferInfo(hOut, &scrBufferInfo);
		short winWidth = scrBufferInfo.srWindow.Right - scrBufferInfo.srWindow.Left + 1;
		short winHeight = scrBufferInfo.srWindow.Bottom - scrBufferInfo.srWindow.Top + 1;
		short scrBufferWidth = scrBufferInfo.dwSize.X;
		short scrBufferHeight = scrBufferInfo.dwSize.Y;
		COORD newSize;
		newSize.X = scrBufferWidth;
		newSize.Y = winHeight;
		SetConsoleScreenBufferSize(hOut, newSize);
	}

	RenderBuffer createBuffer(int w, int h) {
		RenderBuffer out;
		out.width = h;
		out.height = w;

		if (out.width < CG_WIN_MIN) out.width = CG_WIN_MIN;
		else if (out.width > CG_WIN_MAX) out.width = CG_WIN_MAX;
		if (out.height < CG_WIN_MIN) out.height = CG_WIN_MIN;
		else if (out.height > CG_WIN_MAX) out.height = CG_WIN_MAX;

		out.size = w * h;
		out.buffer = new char[out.size];
		return out;
	}

	DepthBuffer createDepthBuffer(int w, int h) {
		DepthBuffer out;
		out.width = h;
		out.height = w;

		if (out.width < CG_WIN_MIN) out.width = CG_WIN_MIN;
		else if (out.width > CG_WIN_MAX) out.width = CG_WIN_MAX;
		if (out.height < CG_WIN_MIN) out.height = CG_WIN_MIN;
		else if (out.height > CG_WIN_MAX) out.height = CG_WIN_MAX;

		out.size = w * h;
		out.buffer = new float[out.size];

		for (int i = 0; i < out.size; i++) {
			out.buffer[i] = 1000.1f;
		}

		return out;
	}

	void clear(RenderBuffer buffer) {
		int size = buffer.size;
		while (size--) {
			buffer.buffer[size] = colors[0];
		}
	}

	void clear(DepthBuffer buffer) {
		int size = buffer.size;
		while (size--) {
			buffer.buffer[size] = 1000.1f;
		}
	}

	void fill(RenderBuffer buffer, char fillColor) {
		int size = buffer.size;
		while (size--) {
			buffer.buffer[size] = fillColor;
		}
	}

	void fill(DepthBuffer buffer, float fillColor) {
		int size = buffer.size;
		while (size--) {
			buffer.buffer[size] = fillColor;
		}
	}

	char getColor(float a) {
		return colors[(int)round(a / 255 * 7)];
	}

	void setPixel(RenderBuffer buffer, int x, int y, char color) {
		int pos = x + (y * buffer.height);
		buffer.buffer[pos] = color;
	}

	void setPixel(DepthBuffer buffer, int x, int y, float color) {
		int pos = x + (y * buffer.height);
		buffer.buffer[pos] = color;
	}

	void setPixelSafe(RenderBuffer buffer, int x, int y, char color) {
		if (x < 0 || x >= buffer.height || y < 0 || y >= buffer.width) return;
		int pos = x + (y * buffer.height);
		buffer.buffer[pos] = color;
	}

	void setPixelSafe(DepthBuffer buffer, int x, int y, float color) {
		if (x < 0 || x >= buffer.height || y < 0 || y >= buffer.width) return;
		int pos = x + (y * buffer.height);
		buffer.buffer[pos] = color;
	}

	void combineBuffer(RenderBuffer buffer1, RenderBuffer buffer2, int _x = 0, int _y = 0) {
		int w = buffer2.width;
		int h = buffer2.height;
		int i = 0;
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				setPixelSafe(buffer1, x + _x, y + _y, buffer2.buffer[i]);
				i++;
			}
		}
	}

	void combineBuffer(DepthBuffer buffer1, DepthBuffer buffer2, int _x = 0, int _y = 0) {
		int w = buffer2.width;
		int h = buffer2.height;
		int i = 0;
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				setPixelSafe(buffer1, x + _x, y + _y, buffer2.buffer[i]);
				i++;
			}
		}
	}

	void fillText(RenderBuffer buffer, int x, int y, std::string text) {
		int len = text.length();
		int pos = x + (y * buffer.height);
		for (int i = 0; i < len; i++) {
			if(i < buffer.size) buffer.buffer[pos + i] = text[i];
		}
	}

	void fillLine(RenderBuffer buffer, int x1, int y1, int x2, int y2, char color) {
		float xdiff = (x2 - x1);
		float ydiff = (y2 - y1);

		if (xdiff == 0.0f && ydiff == 0.0f) {
			setPixelSafe(buffer, x1, y1, color);
			return;
		}

		if (fabs(xdiff) > fabs(ydiff)) {
			float xmin, xmax;

			if (x1 < x2) {
				xmin = x1;
				xmax = x2;
			}
			else {
				xmin = x2;
				xmax = x1;
			}

			float slope = ydiff / xdiff;
			for (float x = xmin; x <= xmax; x += 1.0f) {
				float y = y1 + ((x - x1) * slope);
				setPixelSafe(buffer, x, y, color);
			}
		}else {
			float ymin, ymax;

			if (y1 < y2) {
				ymin = y1;
				ymax = y2;
			}
			else {
				ymin = y2;
				ymax = y1;
			}

			float slope = xdiff / ydiff;
			for (float y = ymin; y <= ymax; y += 1.0f) {
				float x = x1 + ((y - y1) * slope);
				setPixelSafe(buffer, x, y, color);
			}
		}
	}

	void fillLine(DepthBuffer buffer, int x1, int y1, int x2, int y2, float color) {
		float xdiff = (x2 - x1);
		float ydiff = (y2 - y1);

		if (xdiff == 0.0f && ydiff == 0.0f) {
			setPixelSafe(buffer, x1, y1, color);
			return;
		}

		if (fabs(xdiff) > fabs(ydiff)) {
			float xmin, xmax;

			if (x1 < x2) {
				xmin = x1;
				xmax = x2;
			}
			else {
				xmin = x2;
				xmax = x1;
			}

			float slope = ydiff / xdiff;
			for (float x = xmin; x <= xmax; x += 1.0f) {
				float y = y1 + ((x - x1) * slope);
				setPixelSafe(buffer, x, y, color);
			}
		}
		else {
			float ymin, ymax;

			if (y1 < y2) {
				ymin = y1;
				ymax = y2;
			}
			else {
				ymin = y2;
				ymax = y1;
			}

			float slope = xdiff / ydiff;
			for (float y = ymin; y <= ymax; y += 1.0f) {
				float x = x1 + ((y - y1) * slope);
				setPixelSafe(buffer, x, y, color);
			}
		}
	}

	void fillRect(RenderBuffer buffer, int x, int y, int w, int h, char color) {
		for (int xp = 0; xp < w; xp++) {
			for (int yp = 0; yp < h; yp++) {
				setPixelSafe(buffer, xp + x, yp + y, color);
			}
		}
	}

	void fillRect(DepthBuffer buffer, int x, int y, int w, int h, float color) {
		for (int xp = 0; xp < w; xp++) {
			for (int yp = 0; yp < h; yp++) {
				setPixelSafe(buffer, xp + x, yp + y, color);
			}
		}
	}

	void fillTri(RenderBuffer buffer, float x1, float y1, float x2, float y2, float x3, float y3, char color) {
		int i = 0;
		int point[2] = { 0, 0 };
		for (int y = 0; y < buffer.height; y++) {
			for (int x = 0; x < buffer.width; x++) {
				float A = abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2);
				float A1 = abs((x * (y2 - y3) + x2 * (y3 - y) + x3 * (y - y2)) / 2);
				float A2 = abs((x1 * (y - y3) + x * (y3 - y1) + x3 * (y1 - y)) / 2);
				float A3 = abs((x1 * (y2 - y) + x2 * (y - y1) + x * (y1 - y2)) / 2);
				if (A == A1 + A2 + A3) buffer.buffer[i] = color;
				i++;
			}
		}
	}

	void fillTri(DepthBuffer buffer, float x1, float y1, float x2, float y2, float x3, float y3, float color) {
		int i = 0;
		int point[2] = { 0, 0 };
		for (int y = 0; y < buffer.height; y++) {
			for (int x = 0; x < buffer.width; x++) {
				float A = abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2);
				float A1 = abs((x * (y2 - y3) + x2 * (y3 - y) + x3 * (y - y2)) / 2);
				float A2 = abs((x1 * (y - y3) + x * (y3 - y1) + x3 * (y1 - y)) / 2);
				float A3 = abs((x1 * (y2 - y) + x2 * (y - y1) + x * (y1 - y2)) / 2);
				if (A == A1 + A2 + A3) buffer.buffer[i] = color;
				i++;
			}
		}
	}

	void fillLine3d(RenderBuffer buffer, Vector3 v1, Vector3 v2, char color) {
		float hbw = buffer.width / 2;
		float hbh = buffer.height / 2;

		float x1 = ((v1.x + 1) * hbw - hbw) / v1.z + hbw;
		float x2 = ((v2.x + 1) * hbw - hbw) / v2.z + hbw;
		float y1 = ((v1.y + 1) * hbh - hbh) / v1.z + hbh;
		float y2 = ((v2.y + 1) * hbh - hbh) / v2.z + hbh;

		fillLine(buffer, x1, y1, x2, y2, color);
	}

	void fillLine3d(DepthBuffer buffer, Vector3 v1, Vector3 v2, float color) {
		float hbw = buffer.width / 2;
		float hbh = buffer.height / 2;

		float x1 = ((v1.x + 1) * hbw - hbw) / v1.z + hbw;
		float x2 = ((v2.x + 1) * hbw - hbw) / v2.z + hbw;
		float y1 = ((v1.y + 1) * hbh - hbh) / v1.z + hbh;
		float y2 = ((v2.y + 1) * hbh - hbh) / v2.z + hbh;

		fillLine(buffer, x1, y1, x2, y2, color);
	}

	void fillTri3d(RenderBuffer buffer, DepthBuffer dbuffer, Vector3 v1, Vector3 v2, Vector3 v3, char color) {
		float hbw = buffer.width / 2;
		float hbh = buffer.height / 2;

		float z1 = v1.z;
		float z2 = v2.z;
		float z3 = v3.z;
		float x1 = ((v1.x + 1) * hbw - hbw) / z1 + hbw;
		float x2 = ((v2.x + 1) * hbw - hbw) / z2 + hbw;
		float x3 = ((v3.x + 1) * hbw - hbw) / z3 + hbw;
		float y1 = ((v1.y + 1) * hbh - hbh) / z1 + hbh;
		float y2 = ((v2.y + 1) * hbh - hbh) / z2 + hbh;
		float y3 = ((v3.y + 1) * hbh - hbh) / z3 + hbh;

		x1 = round(x1);
		x2 = round(x2);
		x3 = round(x3);
		y1 = round(y1);
		y2 = round(y2);
		y3 = round(y3);

		int i = 0;
		for (int y = 0; y < buffer.height; y++) {
			for (int x = 0; x < buffer.width; x++) {
				float A = abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2);
				float A1 = abs((x * (y2 - y3) + x2 * (y3 - y) + x3 * (y - y2)) / 2);
				float A2 = abs((x1 * (y - y3) + x * (y3 - y1) + x3 * (y1 - y)) / 2);
				float A3 = abs((x1 * (y2 - y) + x2 * (y - y1) + x * (y1 - y2)) / 2);

				float z = (A1 / A) * z1 + (A2 / A) * z2 + (A3 / A) * z3;

				if (A1 + A2 + A3 == A && z < dbuffer.buffer[i] && z > 0.01f && z < 1000.0f) {
					dbuffer.buffer[i] = z;
					buffer.buffer[i] = color;
				}
				i++;
			}
		}
	}

	void render(RenderBuffer buffer) {
		int w = buffer.width;
		int h = buffer.height;

		std::stringstream ss;

		int i = 0;
		for (int x = 0; x < buffer.width; x++) {
			for (int y = 0; y < buffer.height; y++) {
				ss << buffer.buffer[i];

				i++;
			}
			ss << "|\n";
		}

		for (int i = 0; i < buffer.width; i++) {
			ss << "-";
		}

		COORD coord;
		coord.X = 0;
		coord.Y = 0;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);

		std::cout << ss.str();
	}

	void render(DepthBuffer dbuffer) {
		float min = 1000.1f;
		float max = 0.0f;

		for (int i = 0; i < dbuffer.size; i++) {
			if (dbuffer.buffer[i] < min) min = dbuffer.buffer[i];
			if (dbuffer.buffer[i] > max) max = dbuffer.buffer[i];
		}

		float mmax = max - min;

		RenderBuffer buffer = CG::createBuffer(dbuffer.width, dbuffer.height);

		for (int i = 0; i < buffer.size; i++) {
			float a = (dbuffer.buffer[i] - min) / mmax * 255;
			char color = getColor(a);
			buffer.buffer[i] = color;
		}

		int w = buffer.width;
		int h = buffer.height;

		std::stringstream ss;

		int i = 0;
		for (int x = 0; x < buffer.width; x++) {
			for (int y = 0; y < buffer.height; y++) {
				ss << buffer.buffer[i];

				i++;
			}
			ss << "|\n";
		}

		for (int i = 0; i < buffer.width; i++) {
			ss << "-";
		}

		COORD coord;
		coord.X = 0;
		coord.Y = 0;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

		std::cout << ss.str();
	}
}

namespace BasicShapes {

#define Vector3 CG::Vector3

	CG::Face CubeFaces[12] = {
		CG::Face(Vector3(-0.5, -0.5, -0.5), Vector3(0.5, -0.5, -0.5), Vector3(0.5, 0.5, -0.5), CG::getColor(255)),
		CG::Face(Vector3(0.5, 0.5, -0.5), Vector3(-0.5, 0.5, -0.5), Vector3(-0.5, -0.5, -0.5), CG::getColor(255)),
		CG::Face(Vector3(-0.5, -0.5, -0.5), Vector3(-0.5, 0.5, 0.5), Vector3(-0.5, -0.5, 0.5), CG::getColor(200)),
		CG::Face(Vector3(-0.5, -0.5, -0.5), Vector3(-0.5, 0.5, -0.5), Vector3(-0.5, 0.5, 0.5), CG::getColor(200)),
		CG::Face(Vector3(-0.5, -0.5, -0.5), Vector3(0.5, -0.5, -0.5), Vector3(0.5, -0.5, 0.5), CG::getColor(145)),
		CG::Face(Vector3(-0.5, -0.5, -0.5), Vector3(0.5, -0.5, 0.5), Vector3(-0.5, -0.5, 0.5), CG::getColor(145)),
		CG::Face(Vector3(-0.5, -0.5, 0.5), Vector3(-0.5, 0.5, 0.5), Vector3(0.5, -0.5, 0.5), CG::getColor(90)),
		CG::Face(Vector3(-0.5, 0.5, 0.5), Vector3(0.5, -0.5, 0.5), Vector3(0.5, 0.5, 0.5), CG::getColor(90)),
		CG::Face(Vector3(0.5, -0.5, -0.5), Vector3(0.5, -0.5, 0.5), Vector3(0.5, 0.5, 0.5), CG::getColor(35)),
		CG::Face(Vector3(0.5, -0.5, -0.5), Vector3(0.5, 0.5, 0.5), Vector3(0.5, 0.5, -0.5), CG::getColor(35)),
		CG::Face(Vector3(-0.5, 0.5, -0.5), Vector3(0.5, 0.5, -0.5), Vector3(0.5, 0.5, 0.5), CG::getColor(0)),
		CG::Face(Vector3(0.5, 0.5, 0.5), Vector3(-0.5, 0.5, 0.5), Vector3(-0.5, 0.5, -0.5), CG::getColor(0))
	};

	CG::Mesh Cube(CubeFaces, 12);

	CG::Face PlaneFaces[2] = {
		CG::Face(Vector3(-1, -1, 0), Vector3(1, -1, 0), Vector3(1, 1, 0), CG::getColor(50)),
		CG::Face(Vector3(-1, -1, 0), Vector3(1, 1, 0), Vector3(-1, 1, 0), CG::getColor(50))
	};

	CG::Mesh Plane(PlaneFaces, 2);
}