#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <math.h>
#include <ctime>
// LINUX
#include <unistd.h>

class FPS {
public:
    clock_t current_ticks, delta_ticks;
    clock_t fps = 0;

    float deltaTime = 0.0f;

    std::vector<float> frameTimes;

    std::vector<std::string> events;

    float averageTime = 0.0f;

    void startUpdate() {
        current_ticks = clock();
    }

    int endUpdate() {
        delta_ticks = clock() - current_ticks;
        if (delta_ticks > 0)
            fps = CLOCKS_PER_SEC / delta_ticks;

        frameTimes.push_back(fps);
        if (frameTimes.size() > 400) {
            frameTimes.erase(frameTimes.begin());
        }

        for (float f : frameTimes) {
            averageTime += f;
        }
        averageTime /= frameTimes.size();

        if (fps < averageTime / 2.0f) {
            events.insert(events.begin(), "Frame Drop!");
        }

        if (fps == 0) return (int)fps;
        deltaTime =  1.0f / fps;

        return (int)fps;
    }
};

class Profiler {
public:
    clock_t current_ticks, delta_ticks;

    float deltaTime = 0.0f;

    std::vector<float> times;

    void startProfile() {
        current_ticks = clock();
    }

    float endProfile() {
        delta_ticks = clock() - current_ticks;

        times.push_back(delta_ticks);
        return delta_ticks;
    }
};

namespace CG {
	class Color{
	public:
		int r;
		int g;
		int b;

		Color(){
			this->r = 0;
			this->g = 0;
			this->b = 0;
		}

		Color(int _a){
			this->r = _a;
			this->g = _a;
			this->b = _a;
		}

		Color(int _r, int _g, int _b){
			this->r = _r;
			this->g = _g;
			this->b = _b;
		}

		std::string GetColorString(){
			return "\033[48;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
		}

		Color operator=(const Color other){
			this->r = other.r;
			this->g = other.g;
			this->b = other.b;

			return *this;
		}

		static Color MakeRandom(){
			return Color(rand() % 255, rand() % 255, rand() % 255);
		}
	};

	struct RenderBuffer {
		int width;
		int height;
		int size;
		Color* buffer;
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
		Color color;

		Face() {}

		Face(Vector3 _v1, Vector3 _v2, Vector3 _v3, Color _color) {
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
	};

	void fillTri3d(RenderBuffer buffer, DepthBuffer dbuffer, Vector3 v1, Vector3 v2, Vector3 v3, Color color);
	void fillLine3d(RenderBuffer buffer, Vector3 v1, Vector3 v2, Color color);

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

		void Render(MeshRenderer& mr) {
			mr.position = mr.position - position;

			Vector3 fixedRot = Vector3(rotation.y, rotation.z, rotation.x);

			mr.position.Rotate(-fixedRot.x, -fixedRot.y, -fixedRot.z);
			mr.rotation = mr.rotation - fixedRot;
			mr.Render();
			mr.rotation = mr.rotation + fixedRot;
			mr.position.Rotate(fixedRot.x, fixedRot.y, rotation.z);
			mr.position = mr.position + position;
		}

		void WireRender(MeshRenderer& mr) {
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

	//const int res = 13;
	const int res = 16;

	void setup(int w, int h) {
		width = w;
		height = h;

		// LINUX
		std::cout << "\033[?25l" << std::flush;
	}

	void end(){
		// LINUX
		std::cout << "\033[?25h" << std::flush;
	}

	RenderBuffer createBuffer(int w, int h) {
		RenderBuffer out;
		out.width = h;
		out.height = w;

		out.size = w * h;
		out.buffer = new Color[out.size];
		return out;
	}

	DepthBuffer createDepthBuffer(int w, int h) {
		DepthBuffer out;
		out.width = h;
		out.height = w;

		out.size = w * h;
		out.buffer = new float[out.size];

		for (int i = 0; i < out.size; i++) {
			out.buffer[i] = 1000.1f;
		}

		return out;
	}

	void clearScreen(){
		// LINUX
		system("clear");
	}

	void clear(RenderBuffer buffer, Color color) {
		int size = buffer.size;
		while (size--) {
			buffer.buffer[size] = color;
		}
	}

	void clear(DepthBuffer buffer) {
		int size = buffer.size;
		while (size--) {
			buffer.buffer[size] = 1000.1f;
		}
	}

	void fill(RenderBuffer buffer, Color fillColor) {
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

	void setPixel(RenderBuffer buffer, int x, int y, Color color) {
		int pos = x + (y * buffer.height);
		buffer.buffer[pos] = color;
	}

	void setPixel(DepthBuffer buffer, int x, int y, float color) {
		int pos = x + (y * buffer.height);
		buffer.buffer[pos] = color;
	}

	void setPixelSafe(RenderBuffer buffer, int x, int y, Color color) {
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

	void fillLine(RenderBuffer buffer, int x1, int y1, int x2, int y2, Color color) {
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

	void fillRect(RenderBuffer buffer, int x, int y, int w, int h, Color color) {
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

	void fillTri(RenderBuffer buffer, float x1, float y1, float x2, float y2, float x3, float y3, Color color) {
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

	void fillLine3d(RenderBuffer buffer, Vector3 v1, Vector3 v2, Color color) {
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

    float cg_min(float a, float b){
        if(a < b) return a;
        return b;
    }

    float cg_max(float a, float b){
        if(a > b) return a;
        return b;
    }

	void fillTri3d(RenderBuffer buffer, DepthBuffer dbuffer, Vector3 v1, Vector3 v2, Vector3 v3, Color color) {
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

        int minx = cg_max(floor(cg_min(cg_min(x1, x2), x3)), 0);
        int miny = cg_max(floor(cg_min(cg_min(y1, y2), y3)), 0);
        int maxx = cg_min(ceil(cg_max(cg_max(x1, x2), x3)), width);
        int maxy = cg_min(ceil(cg_max(cg_max(y1, y2), y3)), height);
        int renderWidth = maxx - minx + 1;
        int renderHeight = maxy - miny + 1;

		int i;
		for (int y = miny; y < maxy; y++) {
            i = y * width + minx;
			for (int x = minx; x < maxx; x++) {
				float A = abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2);
				float A1 = abs((x * (y2 - y3) + x2 * (y3 - y) + x3 * (y - y2)) / 2);
				float A2 = abs((x1 * (y - y3) + x * (y3 - y1) + x3 * (y1 - y)) / 2);
				float A3 = abs((x1 * (y2 - y) + x2 * (y - y1) + x * (y1 - y2)) / 2);

				float z = (A1 / A) * z1 + (A2 / A) * z2 + (A3 / A) * z3;

				if (z < dbuffer.buffer[i] && A1 + A2 + A3 == A && z > 0.01f && z < 1000.0f) {
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
				ss << buffer.buffer[i].GetColorString() << "  ";

				i++;
			}
			ss << "\033[0m" << "|\n";
		}

		for (int i = 0; i < buffer.width; i++) {
			ss << "--";
		}

		// LINUX
		ss << "\033[" << 0 << ";" << 0 << "H" << std::flush;

		std::string ssOut = ss.str();
		std::cout << ssOut;
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
			Color color(a, a, a);
			buffer.buffer[i] = color;
		}

		int w = buffer.width;
		int h = buffer.height;

		std::stringstream ss;

		int i = 0;
		for (int x = 0; x < buffer.width; x++) {
			for (int y = 0; y < buffer.height; y++) {
				ss << buffer.buffer[i].GetColorString();

				i++;
			}
			ss << "|\n";
		}

		for (int i = 0; i < buffer.width; i++) {
			ss << "-";
		}

		// LINUX
		ss << "\033[" << 0 << ";" << 0 << "H" << std::flush;

		std::cout << ss.str();
	}

	void renderToFile(RenderBuffer buffer, std::string filename) {
		int w = buffer.width;
		int h = buffer.height;

		std::stringstream ss;

		int i = 0;
		for (int x = 0; x < buffer.width; x++) {
			for (int y = 0; y < buffer.height; y++) {
				ss << buffer.buffer[i].GetColorString();

				i++;
			}
			ss << "|\n";
		}

		for (int i = 0; i < buffer.width; i++) {
			ss << "-";
		}

		std::ofstream file(filename.c_str());
		file << ss.str();
		file.close();
	}

	void renderToFile(DepthBuffer dbuffer, std::string filename) {
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
			Color color(a, a, a);
			buffer.buffer[i] = color;
		}

		int w = buffer.width;
		int h = buffer.height;

		std::stringstream ss;

		int i = 0;
		for (int x = 0; x < buffer.width; x++) {
			for (int y = 0; y < buffer.height; y++) {
				ss << buffer.buffer[i].GetColorString();

				i++;
			}
			ss << "|\n";
		}

		for (int i = 0; i < buffer.width; i++) {
			ss << "-";
		}

		std::ofstream file(filename.c_str());
		file << ss.str();
		file.close();
	}
}

#define Vector3 CG::Vector3

namespace BasicShapes {
	CG::Face CubeFaces[12] = {
		CG::Face(Vector3(-0.5, -0.5, -0.5), Vector3(0.5, -0.5, -0.5), Vector3(0.5, 0.5, -0.5), CG::Color(255)),
		CG::Face(Vector3(0.5, 0.5, -0.5), Vector3(-0.5, 0.5, -0.5), Vector3(-0.5, -0.5, -0.5), CG::Color(255)),
		CG::Face(Vector3(-0.5, -0.5, -0.5), Vector3(-0.5, 0.5, 0.5), Vector3(-0.5, -0.5, 0.5), CG::Color(200)),
		CG::Face(Vector3(-0.5, -0.5, -0.5), Vector3(-0.5, 0.5, -0.5), Vector3(-0.5, 0.5, 0.5), CG::Color(200)),
		CG::Face(Vector3(-0.5, -0.5, -0.5), Vector3(0.5, -0.5, -0.5), Vector3(0.5, -0.5, 0.5), CG::Color(145)),
		CG::Face(Vector3(-0.5, -0.5, -0.5), Vector3(0.5, -0.5, 0.5), Vector3(-0.5, -0.5, 0.5), CG::Color(145)),
		CG::Face(Vector3(-0.5, -0.5, 0.5), Vector3(-0.5, 0.5, 0.5), Vector3(0.5, -0.5, 0.5), CG::Color(90)),
		CG::Face(Vector3(-0.5, 0.5, 0.5), Vector3(0.5, -0.5, 0.5), Vector3(0.5, 0.5, 0.5), CG::Color(90)),
		CG::Face(Vector3(0.5, -0.5, -0.5), Vector3(0.5, -0.5, 0.5), Vector3(0.5, 0.5, 0.5), CG::Color(35)),
		CG::Face(Vector3(0.5, -0.5, -0.5), Vector3(0.5, 0.5, 0.5), Vector3(0.5, 0.5, -0.5), CG::Color(35)),
		CG::Face(Vector3(-0.5, 0.5, -0.5), Vector3(0.5, 0.5, -0.5), Vector3(0.5, 0.5, 0.5), CG::Color(15)),
		CG::Face(Vector3(0.5, 0.5, 0.5), Vector3(-0.5, 0.5, 0.5), Vector3(-0.5, 0.5, -0.5), CG::Color(15))
	};

	CG::Mesh Cube(CubeFaces, 12);

	CG::Face PlaneFaces[2] = {
		CG::Face(Vector3(-1, -1, 0), Vector3(1, -1, 0), Vector3(1, 1, 0), CG::Color(50)),
		CG::Face(Vector3(-1, -1, 0), Vector3(1, 1, 0), Vector3(-1, 1, 0), CG::Color(50))
	};

	CG::Mesh Plane(PlaneFaces, 2);
}
