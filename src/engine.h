#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <math.h>
#include <ctime>
#include <Windows.h>

namespace Input {
	bool GetKeyDown(char key) {
        return false;
	}
}

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

typedef struct _CONSOLE_FONT_INFOEX {
  ULONG cbSize;
  DWORD nFont;
  COORD dwFontSize;
  UINT  FontFamily;
  UINT  FontWeight;
  WCHAR FaceName[LF_FACESIZE];
} CONSOLE_FONT_INFOEX, *PCONSOLE_FONT_INFOEX;

#ifdef __cplusplus
extern "C" {
#endif
BOOL WINAPI SetCurrentConsoleFontEx(HANDLE hConsoleOutput, BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX
lpConsoleCurrentFontEx);
#ifdef __cplusplus
}
#endif

//Vendor
// OBJ_Loader.h - A Single Header OBJ Model Loader

#pragma once

// Iostream - STD I/O Library
#include <iostream>

// Vector - STD Vector/Array Library
#include <vector>

// String - STD String Library
#include <string>

// fStream - STD File I/O Library
#include <fstream>

// Math.h - STD math Library
#include <math.h>

// Namespace: OBJL
//
// Description: The namespace that holds eveyrthing that
//	is needed and used for the OBJ Model Loader
namespace objl
{
	// Structure: Vector2
	//
	// Description: A 2D Vector that Holds Positional Data
	struct Vector2
	{
		// Default Constructor
		Vector2()
		{
			X = 0.0f;
			Y = 0.0f;
		}
		// Variable Set Constructor
		Vector2(float X_, float Y_)
		{
			X = X_;
			Y = Y_;
		}
		// Bool Equals Operator Overload
		bool operator==(const Vector2& other) const
		{
			return (this->X == other.X && this->Y == other.Y);
		}
		// Bool Not Equals Operator Overload
		bool operator!=(const Vector2& other) const
		{
			return !(this->X == other.X && this->Y == other.Y);
		}
		// Addition Operator Overload
		Vector2 operator+(const Vector2& right) const
		{
			return Vector2(this->X + right.X, this->Y + right.Y);
		}
		// Subtraction Operator Overload
		Vector2 operator-(const Vector2& right) const
		{
			return Vector2(this->X - right.X, this->Y - right.Y);
		}
		// Float Multiplication Operator Overload
		Vector2 operator*(const float& other) const
		{
			return Vector2(this->X * other, this->Y * other);
		}

		// Positional Variables
		float X;
		float Y;
	};

	// Structure: Vector3
	//
	// Description: A 3D Vector that Holds Positional Data
	struct Vector3
	{
		// Default Constructor
		Vector3()
		{
			X = 0.0f;
			Y = 0.0f;
			Z = 0.0f;
		}
		// Variable Set Constructor
		Vector3(float X_, float Y_, float Z_)
		{
			X = X_;
			Y = Y_;
			Z = Z_;
		}
		// Bool Equals Operator Overload
		bool operator==(const Vector3& other) const
		{
			return (this->X == other.X && this->Y == other.Y && this->Z == other.Z);
		}
		// Bool Not Equals Operator Overload
		bool operator!=(const Vector3& other) const
		{
			return !(this->X == other.X && this->Y == other.Y && this->Z == other.Z);
		}
		// Addition Operator Overload
		Vector3 operator+(const Vector3& right) const
		{
			return Vector3(this->X + right.X, this->Y + right.Y, this->Z + right.Z);
		}
		// Subtraction Operator Overload
		Vector3 operator-(const Vector3& right) const
		{
			return Vector3(this->X - right.X, this->Y - right.Y, this->Z - right.Z);
		}
		// Float Multiplication Operator Overload
		Vector3 operator*(const float& other) const
		{
			return Vector3(this->X * other, this->Y * other, this->Z * other);
		}
		// Float Division Operator Overload
		Vector3 operator/(const float& other) const
		{
			return Vector3(this->X / other, this->Y / other, this->Z / other);
		}

		// Positional Variables
		float X;
		float Y;
		float Z;
	};

	// Structure: Vertex
	//
	// Description: Model Vertex object that holds
	//	a Position, Normal, and Texture Coordinate
	struct Vertex
	{
		// Position Vector
		Vector3 Position;

		// Normal Vector
		Vector3 Normal;

		// Texture Coordinate Vector
		Vector2 TextureCoordinate;
	};

	struct Material
	{
		Material()
		{
			name;
			Ns = 0.0f;
			Ni = 0.0f;
			d = 0.0f;
			illum = 0;
		}

		// Material Name
		std::string name;
		// Ambient Color
		Vector3 Ka;
		// Diffuse Color
		Vector3 Kd;
		// Specular Color
		Vector3 Ks;
		// Specular Exponent
		float Ns;
		// Optical Density
		float Ni;
		// Dissolve
		float d;
		// Illumination
		int illum;
		// Ambient Texture Map
		std::string map_Ka;
		// Diffuse Texture Map
		std::string map_Kd;
		// Specular Texture Map
		std::string map_Ks;
		// Specular Hightlight Map
		std::string map_Ns;
		// Alpha Texture Map
		std::string map_d;
		// Bump Map
		std::string map_bump;
	};

	// Structure: Mesh
	//
	// Description: A Simple Mesh Object that holds
	//	a name, a vertex list, and an index list
	struct Mesh
	{
		// Default Constructor
		Mesh()
		{

		}
		// Variable Set Constructor
		Mesh(std::vector<Vertex>& _Vertices, std::vector<unsigned int>& _Indices)
		{
			Vertices = _Vertices;
			Indices = _Indices;
		}
		// Mesh Name
		std::string MeshName;
		// Vertex List
		std::vector<Vertex> Vertices;
		// Index List
		std::vector<unsigned int> Indices;

		// Material
		Material MeshMaterial;
	};

	// Namespace: Math
	//
	// Description: The namespace that holds all of the math
	//	functions need for OBJL
	namespace math
	{
		// Vector3 Cross Product
		Vector3 CrossV3(const Vector3 a, const Vector3 b)
		{
			return Vector3(a.Y * b.Z - a.Z * b.Y,
				a.Z * b.X - a.X * b.Z,
				a.X * b.Y - a.Y * b.X);
		}

		// Vector3 Magnitude Calculation
		float MagnitudeV3(const Vector3 in)
		{
			return (sqrtf(powf(in.X, 2) + powf(in.Y, 2) + powf(in.Z, 2)));
		}

		// Vector3 DotProduct
		float DotV3(const Vector3 a, const Vector3 b)
		{
			return (a.X * b.X) + (a.Y * b.Y) + (a.Z * b.Z);
		}

		// Angle between 2 Vector3 Objects
		float AngleBetweenV3(const Vector3 a, const Vector3 b)
		{
			float angle = DotV3(a, b);
			angle /= (MagnitudeV3(a) * MagnitudeV3(b));
			return angle = acosf(angle);
		}

		// Projection Calculation of a onto b
		Vector3 ProjV3(const Vector3 a, const Vector3 b)
		{
			Vector3 bn = b / MagnitudeV3(b);
			return bn * DotV3(a, bn);
		}
	}

	// Namespace: Algorithm
	//
	// Description: The namespace that holds all of the
	// Algorithms needed for OBJL
	namespace algorithm
	{
		// Vector3 Multiplication Opertor Overload
		Vector3 operator*(const float& left, const Vector3& right)
		{
			return Vector3(right.X * left, right.Y * left, right.Z * left);
		}

		// A test to see if P1 is on the same side as P2 of a line segment ab
		bool SameSide(Vector3 p1, Vector3 p2, Vector3 a, Vector3 b)
		{
			Vector3 cp1 = math::CrossV3(b - a, p1 - a);
			Vector3 cp2 = math::CrossV3(b - a, p2 - a);

			if (math::DotV3(cp1, cp2) >= 0)
				return true;
			else
				return false;
		}

		// Generate a cross produect normal for a triangle
		Vector3 GenTriNormal(Vector3 t1, Vector3 t2, Vector3 t3)
		{
			Vector3 u = t2 - t1;
			Vector3 v = t3 - t1;

			Vector3 normal = math::CrossV3(u, v);

			return normal;
		}

		// Check to see if a Vector3 Point is within a 3 Vector3 Triangle
		bool inTriangle(Vector3 point, Vector3 tri1, Vector3 tri2, Vector3 tri3)
		{
			// Test to see if it is within an infinite prism that the triangle outlines.
			bool within_tri_prisim = SameSide(point, tri1, tri2, tri3) && SameSide(point, tri2, tri1, tri3)
				&& SameSide(point, tri3, tri1, tri2);

			// If it isn't it will never be on the triangle
			if (!within_tri_prisim)
				return false;

			// Calulate Triangle's Normal
			Vector3 n = GenTriNormal(tri1, tri2, tri3);

			// Project the point onto this normal
			Vector3 proj = math::ProjV3(point, n);

			// If the distance from the triangle to the point is 0
			//	it lies on the triangle
			if (math::MagnitudeV3(proj) == 0)
				return true;
			else
				return false;
		}

		// Split a String into a string array at a given token
		inline void split(const std::string& in,
			std::vector<std::string>& out,
			std::string token)
		{
			out.clear();

			std::string temp;

			for (int i = 0; i < int(in.size()); i++)
			{
				std::string test = in.substr(i, token.size());

				if (test == token)
				{
					if (!temp.empty())
					{
						out.push_back(temp);
						temp.clear();
						i += (int)token.size() - 1;
					}
					else
					{
						out.push_back("");
					}
				}
				else if (i + token.size() >= in.size())
				{
					temp += in.substr(i, token.size());
					out.push_back(temp);
					break;
				}
				else
				{
					temp += in[i];
				}
			}
		}

		// Get tail of string after first token and possibly following spaces
		inline std::string tail(const std::string& in)
		{
			size_t token_start = in.find_first_not_of(" \t");
			size_t space_start = in.find_first_of(" \t", token_start);
			size_t tail_start = in.find_first_not_of(" \t", space_start);
			size_t tail_end = in.find_last_not_of(" \t");
			if (tail_start != std::string::npos && tail_end != std::string::npos)
			{
				return in.substr(tail_start, tail_end - tail_start + 1);
			}
			else if (tail_start != std::string::npos)
			{
				return in.substr(tail_start);
			}
			return "";
		}

		// Get first token of string
		inline std::string firstToken(const std::string& in)
		{
			if (!in.empty())
			{
				size_t token_start = in.find_first_not_of(" \t");
				size_t token_end = in.find_first_of(" \t", token_start);
				if (token_start != std::string::npos && token_end != std::string::npos)
				{
					return in.substr(token_start, token_end - token_start);
				}
				else if (token_start != std::string::npos)
				{
					return in.substr(token_start);
				}
			}
			return "";
		}

		// Get element at given index position
		template <class T>
		inline const T& getElement(const std::vector<T>& elements, std::string& index)
		{
			int idx = std::stoi(index);
			if (idx < 0)
				idx = int(elements.size()) + idx;
			else
				idx--;
			return elements[idx];
		}
	}

	// Class: Loader
	//
	// Description: The OBJ Model Loader
	class Loader
	{
	public:
		// Default Constructor
		Loader()
		{

		}
		~Loader()
		{
			LoadedMeshes.clear();
		}

		// Load a file into the loader
		//
		// If file is loaded return true
		//
		// If the file is unable to be found
		// or unable to be loaded return false
		bool LoadFile(std::string Path)
		{
			// If the file is not an .obj file return false
			if (Path.substr(Path.size() - 4, 4) != ".obj")
				return false;


			std::ifstream file(Path);

			if (!file.is_open())
				return false;

			LoadedMeshes.clear();
			LoadedVertices.clear();
			LoadedIndices.clear();

			std::vector<Vector3> Positions;
			std::vector<Vector2> TCoords;
			std::vector<Vector3> Normals;

			std::vector<Vertex> Vertices;
			std::vector<unsigned int> Indices;

			std::vector<std::string> MeshMatNames;

			bool listening = false;
			std::string meshname;

			Mesh tempMesh;

#ifdef OBJL_CONSOLE_OUTPUT
			const unsigned int outputEveryNth = 1000;
			unsigned int outputIndicator = outputEveryNth;
#endif

			std::string curline;
			while (std::getline(file, curline))
			{
#ifdef OBJL_CONSOLE_OUTPUT
				if ((outputIndicator = ((outputIndicator + 1) % outputEveryNth)) == 1)
				{
					if (!meshname.empty())
					{
						std::cout
							<< "\r- " << meshname
							<< "\t| vertices > " << Positions.size()
							<< "\t| texcoords > " << TCoords.size()
							<< "\t| normals > " << Normals.size()
							<< "\t| triangles > " << (Vertices.size() / 3)
							<< (!MeshMatNames.empty() ? "\t| material: " + MeshMatNames.back() : "");
					}
				}
#endif

				// Generate a Mesh Object or Prepare for an object to be created
				if (algorithm::firstToken(curline) == "o" || algorithm::firstToken(curline) == "g" || curline[0] == 'g')
				{
					if (!listening)
					{
						listening = true;

						if (algorithm::firstToken(curline) == "o" || algorithm::firstToken(curline) == "g")
						{
							meshname = algorithm::tail(curline);
						}
						else
						{
							meshname = "unnamed";
						}
					}
					else
					{
						// Generate the mesh to put into the array

						if (!Indices.empty() && !Vertices.empty())
						{
							// Create Mesh
							tempMesh = Mesh(Vertices, Indices);
							tempMesh.MeshName = meshname;

							// Insert Mesh
							LoadedMeshes.push_back(tempMesh);

							// Cleanup
							Vertices.clear();
							Indices.clear();
							meshname.clear();

							meshname = algorithm::tail(curline);
						}
						else
						{
							if (algorithm::firstToken(curline) == "o" || algorithm::firstToken(curline) == "g")
							{
								meshname = algorithm::tail(curline);
							}
							else
							{
								meshname = "unnamed";
							}
						}
					}
#ifdef OBJL_CONSOLE_OUTPUT
					std::cout << std::endl;
					outputIndicator = 0;
#endif
				}
				// Generate a Vertex Position
				if (algorithm::firstToken(curline) == "v")
				{
					std::vector<std::string> spos;
					Vector3 vpos;
					algorithm::split(algorithm::tail(curline), spos, " ");

					vpos.X = std::stof(spos[0]);
					vpos.Y = std::stof(spos[1]);
					vpos.Z = std::stof(spos[2]);

					Positions.push_back(vpos);
				}
				// Generate a Vertex Texture Coordinate
				if (algorithm::firstToken(curline) == "vt")
				{
					std::vector<std::string> stex;
					Vector2 vtex;
					algorithm::split(algorithm::tail(curline), stex, " ");

					vtex.X = std::stof(stex[0]);
					vtex.Y = std::stof(stex[1]);

					TCoords.push_back(vtex);
				}
				// Generate a Vertex Normal;
				if (algorithm::firstToken(curline) == "vn")
				{
					std::vector<std::string> snor;
					Vector3 vnor;
					algorithm::split(algorithm::tail(curline), snor, " ");

					vnor.X = std::stof(snor[0]);
					vnor.Y = std::stof(snor[1]);
					vnor.Z = std::stof(snor[2]);

					Normals.push_back(vnor);
				}
				// Generate a Face (vertices & indices)
				if (algorithm::firstToken(curline) == "f")
				{
					// Generate the vertices
					std::vector<Vertex> vVerts;
					GenVerticesFromRawOBJ(vVerts, Positions, TCoords, Normals, curline);

					// Add Vertices
					for (int i = 0; i < int(vVerts.size()); i++)
					{
						Vertices.push_back(vVerts[i]);

						LoadedVertices.push_back(vVerts[i]);
					}

					std::vector<unsigned int> iIndices;

					VertexTriangluation(iIndices, vVerts);

					// Add Indices
					for (int i = 0; i < int(iIndices.size()); i++)
					{
						unsigned int indnum = (unsigned int)((Vertices.size()) - vVerts.size()) + iIndices[i];
						Indices.push_back(indnum);

						indnum = (unsigned int)((LoadedVertices.size()) - vVerts.size()) + iIndices[i];
						LoadedIndices.push_back(indnum);

					}
				}
				// Get Mesh Material Name
				if (algorithm::firstToken(curline) == "usemtl")
				{
					MeshMatNames.push_back(algorithm::tail(curline));

					// Create new Mesh, if Material changes within a group
					if (!Indices.empty() && !Vertices.empty())
					{
						// Create Mesh
						tempMesh = Mesh(Vertices, Indices);
						tempMesh.MeshName = meshname;
						int i = 2;
						while (1) {
							tempMesh.MeshName = meshname + "_" + std::to_string(i);

							for (auto& m : LoadedMeshes)
								if (m.MeshName == tempMesh.MeshName)
									continue;
							break;
						}

						// Insert Mesh
						LoadedMeshes.push_back(tempMesh);

						// Cleanup
						Vertices.clear();
						Indices.clear();
					}

#ifdef OBJL_CONSOLE_OUTPUT
					outputIndicator = 0;
#endif
				}
				// Load Materials
				if (algorithm::firstToken(curline) == "mtllib")
				{
					// Generate LoadedMaterial

					// Generate a path to the material file
					std::vector<std::string> temp;
					algorithm::split(Path, temp, "/");

					std::string pathtomat = "";

					if (temp.size() != 1)
					{
						for (int i = 0; i < temp.size() - 1; i++)
						{
							pathtomat += temp[i] + "/";
						}
					}


					pathtomat += algorithm::tail(curline);

#ifdef OBJL_CONSOLE_OUTPUT
					std::cout << std::endl << "- find materials in: " << pathtomat << std::endl;
#endif

					// Load Materials
					LoadMaterials(pathtomat);
				}
			}

#ifdef OBJL_CONSOLE_OUTPUT
			std::cout << std::endl;
#endif

			// Deal with last mesh

			if (!Indices.empty() && !Vertices.empty())
			{
				// Create Mesh
				tempMesh = Mesh(Vertices, Indices);
				tempMesh.MeshName = meshname;

				// Insert Mesh
				LoadedMeshes.push_back(tempMesh);
			}

			file.close();

			// Set Materials for each Mesh
			for (int i = 0; i < MeshMatNames.size(); i++)
			{
				std::string matname = MeshMatNames[i];

				// Find corresponding material name in loaded materials
				// when found copy material variables into mesh material
				for (int j = 0; j < LoadedMaterials.size(); j++)
				{
					if (LoadedMaterials[j].name == matname)
					{
						LoadedMeshes[i].MeshMaterial = LoadedMaterials[j];
						break;
					}
				}
			}

			if (LoadedMeshes.empty() && LoadedVertices.empty() && LoadedIndices.empty())
			{
				return false;
			}
			else
			{
				return true;
			}
		}

		// Loaded Mesh Objects
		std::vector<Mesh> LoadedMeshes;
		// Loaded Vertex Objects
		std::vector<Vertex> LoadedVertices;
		// Loaded Index Positions
		std::vector<unsigned int> LoadedIndices;
		// Loaded Material Objects
		std::vector<Material> LoadedMaterials;

	private:
		// Generate vertices from a list of positions, 
		//	tcoords, normals and a face line
		void GenVerticesFromRawOBJ(std::vector<Vertex>& oVerts,
			const std::vector<Vector3>& iPositions,
			const std::vector<Vector2>& iTCoords,
			const std::vector<Vector3>& iNormals,
			std::string icurline)
		{
			std::vector<std::string> sface, svert;
			Vertex vVert;
			algorithm::split(algorithm::tail(icurline), sface, " ");

			bool noNormal = false;

			// For every given vertex do this
			for (int i = 0; i < int(sface.size()); i++)
			{
				// See What type the vertex is.
				int vtype;

				algorithm::split(sface[i], svert, "/");

				// Check for just position - v1
				if (svert.size() == 1)
				{
					// Only position
					vtype = 1;
				}

				// Check for position & texture - v1/vt1
				if (svert.size() == 2)
				{
					// Position & Texture
					vtype = 2;
				}

				// Check for Position, Texture and Normal - v1/vt1/vn1
				// or if Position and Normal - v1//vn1
				if (svert.size() == 3)
				{
					if (svert[1] != "")
					{
						// Position, Texture, and Normal
						vtype = 4;
					}
					else
					{
						// Position & Normal
						vtype = 3;
					}
				}

				// Calculate and store the vertex
				switch (vtype)
				{
				case 1: // P
				{
					vVert.Position = algorithm::getElement(iPositions, svert[0]);
					vVert.TextureCoordinate = Vector2(0, 0);
					noNormal = true;
					oVerts.push_back(vVert);
					break;
				}
				case 2: // P/T
				{
					vVert.Position = algorithm::getElement(iPositions, svert[0]);
					vVert.TextureCoordinate = algorithm::getElement(iTCoords, svert[1]);
					noNormal = true;
					oVerts.push_back(vVert);
					break;
				}
				case 3: // P//N
				{
					vVert.Position = algorithm::getElement(iPositions, svert[0]);
					vVert.TextureCoordinate = Vector2(0, 0);
					vVert.Normal = algorithm::getElement(iNormals, svert[2]);
					oVerts.push_back(vVert);
					break;
				}
				case 4: // P/T/N
				{
					vVert.Position = algorithm::getElement(iPositions, svert[0]);
					vVert.TextureCoordinate = algorithm::getElement(iTCoords, svert[1]);
					vVert.Normal = algorithm::getElement(iNormals, svert[2]);
					oVerts.push_back(vVert);
					break;
				}
				default:
				{
					break;
				}
				}
			}

			// take care of missing normals
			// these may not be truly acurate but it is the 
			// best they get for not compiling a mesh with normals	
			if (noNormal)
			{
				Vector3 A = oVerts[0].Position - oVerts[1].Position;
				Vector3 B = oVerts[2].Position - oVerts[1].Position;

				Vector3 normal = math::CrossV3(A, B);

				for (int i = 0; i < int(oVerts.size()); i++)
				{
					oVerts[i].Normal = normal;
				}
			}
		}

		// Triangulate a list of vertices into a face by printing
		//	inducies corresponding with triangles within it
		void VertexTriangluation(std::vector<unsigned int>& oIndices,
			const std::vector<Vertex>& iVerts)
		{
			// If there are 2 or less verts,
			// no triangle can be created,
			// so exit
			if (iVerts.size() < 3)
			{
				return;
			}
			// If it is a triangle no need to calculate it
			if (iVerts.size() == 3)
			{
				oIndices.push_back(0);
				oIndices.push_back(1);
				oIndices.push_back(2);
				return;
			}

			// Create a list of vertices
			std::vector<Vertex> tVerts = iVerts;

			while (true)
			{
				// For every vertex
				for (int i = 0; i < int(tVerts.size()); i++)
				{
					// pPrev = the previous vertex in the list
					Vertex pPrev;
					if (i == 0)
					{
						pPrev = tVerts[tVerts.size() - 1];
					}
					else
					{
						pPrev = tVerts[i - 1];
					}

					// pCur = the current vertex;
					Vertex pCur = tVerts[i];

					// pNext = the next vertex in the list
					Vertex pNext;
					if (i == tVerts.size() - 1)
					{
						pNext = tVerts[0];
					}
					else
					{
						pNext = tVerts[i + 1];
					}

					// Check to see if there are only 3 verts left
					// if so this is the last triangle
					if (tVerts.size() == 3)
					{
						// Create a triangle from pCur, pPrev, pNext
						for (int j = 0; j < int(tVerts.size()); j++)
						{
							if (iVerts[j].Position == pCur.Position)
								oIndices.push_back(j);
							if (iVerts[j].Position == pPrev.Position)
								oIndices.push_back(j);
							if (iVerts[j].Position == pNext.Position)
								oIndices.push_back(j);
						}

						tVerts.clear();
						break;
					}
					if (tVerts.size() == 4)
					{
						// Create a triangle from pCur, pPrev, pNext
						for (int j = 0; j < int(iVerts.size()); j++)
						{
							if (iVerts[j].Position == pCur.Position)
								oIndices.push_back(j);
							if (iVerts[j].Position == pPrev.Position)
								oIndices.push_back(j);
							if (iVerts[j].Position == pNext.Position)
								oIndices.push_back(j);
						}

						Vector3 tempVec;
						for (int j = 0; j < int(tVerts.size()); j++)
						{
							if (tVerts[j].Position != pCur.Position
								&& tVerts[j].Position != pPrev.Position
								&& tVerts[j].Position != pNext.Position)
							{
								tempVec = tVerts[j].Position;
								break;
							}
						}

						// Create a triangle from pCur, pPrev, pNext
						for (int j = 0; j < int(iVerts.size()); j++)
						{
							if (iVerts[j].Position == pPrev.Position)
								oIndices.push_back(j);
							if (iVerts[j].Position == pNext.Position)
								oIndices.push_back(j);
							if (iVerts[j].Position == tempVec)
								oIndices.push_back(j);
						}

						tVerts.clear();
						break;
					}

					// If Vertex is not an interior vertex
					float angle = math::AngleBetweenV3(pPrev.Position - pCur.Position, pNext.Position - pCur.Position) * (180 / 3.14159265359);
					if (angle <= 0 && angle >= 180)
						continue;

					// If any vertices are within this triangle
					bool inTri = false;
					for (int j = 0; j < int(iVerts.size()); j++)
					{
						if (algorithm::inTriangle(iVerts[j].Position, pPrev.Position, pCur.Position, pNext.Position)
							&& iVerts[j].Position != pPrev.Position
							&& iVerts[j].Position != pCur.Position
							&& iVerts[j].Position != pNext.Position)
						{
							inTri = true;
							break;
						}
					}
					if (inTri)
						continue;

					// Create a triangle from pCur, pPrev, pNext
					for (int j = 0; j < int(iVerts.size()); j++)
					{
						if (iVerts[j].Position == pCur.Position)
							oIndices.push_back(j);
						if (iVerts[j].Position == pPrev.Position)
							oIndices.push_back(j);
						if (iVerts[j].Position == pNext.Position)
							oIndices.push_back(j);
					}

					// Delete pCur from the list
					for (int j = 0; j < int(tVerts.size()); j++)
					{
						if (tVerts[j].Position == pCur.Position)
						{
							tVerts.erase(tVerts.begin() + j);
							break;
						}
					}

					// reset i to the start
					// -1 since loop will add 1 to it
					i = -1;
				}

				// if no triangles were created
				if (oIndices.size() == 0)
					break;

				// if no more vertices
				if (tVerts.size() == 0)
					break;
			}
		}

		// Load Materials from .mtl file
		bool LoadMaterials(std::string path)
		{
			// If the file is not a material file return false
			if (path.substr(path.size() - 4, path.size()) != ".mtl")
				return false;

			std::ifstream file(path);

			// If the file is not found return false
			if (!file.is_open())
				return false;

			Material tempMaterial;

			bool listening = false;

			// Go through each line looking for material variables
			std::string curline;
			while (std::getline(file, curline))
			{
				// new material and material name
				if (algorithm::firstToken(curline) == "newmtl")
				{
					if (!listening)
					{
						listening = true;

						if (curline.size() > 7)
						{
							tempMaterial.name = algorithm::tail(curline);
						}
						else
						{
							tempMaterial.name = "none";
						}
					}
					else
					{
						// Generate the material

						// Push Back loaded Material
						LoadedMaterials.push_back(tempMaterial);

						// Clear Loaded Material
						tempMaterial = Material();

						if (curline.size() > 7)
						{
							tempMaterial.name = algorithm::tail(curline);
						}
						else
						{
							tempMaterial.name = "none";
						}
					}
				}
				// Ambient Color
				if (algorithm::firstToken(curline) == "Ka")
				{
					std::vector<std::string> temp;
					algorithm::split(algorithm::tail(curline), temp, " ");

					if (temp.size() != 3)
						continue;

					tempMaterial.Ka.X = std::stof(temp[0]);
					tempMaterial.Ka.Y = std::stof(temp[1]);
					tempMaterial.Ka.Z = std::stof(temp[2]);
				}
				// Diffuse Color
				if (algorithm::firstToken(curline) == "Kd")
				{
					std::vector<std::string> temp;
					algorithm::split(algorithm::tail(curline), temp, " ");

					if (temp.size() != 3)
						continue;

					tempMaterial.Kd.X = std::stof(temp[0]);
					tempMaterial.Kd.Y = std::stof(temp[1]);
					tempMaterial.Kd.Z = std::stof(temp[2]);
				}
				// Specular Color
				if (algorithm::firstToken(curline) == "Ks")
				{
					std::vector<std::string> temp;
					algorithm::split(algorithm::tail(curline), temp, " ");

					if (temp.size() != 3)
						continue;

					tempMaterial.Ks.X = std::stof(temp[0]);
					tempMaterial.Ks.Y = std::stof(temp[1]);
					tempMaterial.Ks.Z = std::stof(temp[2]);
				}
				// Specular Exponent
				if (algorithm::firstToken(curline) == "Ns")
				{
					tempMaterial.Ns = std::stof(algorithm::tail(curline));
				}
				// Optical Density
				if (algorithm::firstToken(curline) == "Ni")
				{
					tempMaterial.Ni = std::stof(algorithm::tail(curline));
				}
				// Dissolve
				if (algorithm::firstToken(curline) == "d")
				{
					tempMaterial.d = std::stof(algorithm::tail(curline));
				}
				// Illumination
				if (algorithm::firstToken(curline) == "illum")
				{
					tempMaterial.illum = std::stoi(algorithm::tail(curline));
				}
				// Ambient Texture Map
				if (algorithm::firstToken(curline) == "map_Ka")
				{
					tempMaterial.map_Ka = algorithm::tail(curline);
				}
				// Diffuse Texture Map
				if (algorithm::firstToken(curline) == "map_Kd")
				{
					tempMaterial.map_Kd = algorithm::tail(curline);
				}
				// Specular Texture Map
				if (algorithm::firstToken(curline) == "map_Ks")
				{
					tempMaterial.map_Ks = algorithm::tail(curline);
				}
				// Specular Hightlight Map
				if (algorithm::firstToken(curline) == "map_Ns")
				{
					tempMaterial.map_Ns = algorithm::tail(curline);
				}
				// Alpha Texture Map
				if (algorithm::firstToken(curline) == "map_d")
				{
					tempMaterial.map_d = algorithm::tail(curline);
				}
				// Bump Map
				if (algorithm::firstToken(curline) == "map_Bump" || algorithm::firstToken(curline) == "map_bump" || algorithm::firstToken(curline) == "bump")
				{
					tempMaterial.map_bump = algorithm::tail(curline);
				}
			}

			// Deal with last material

			// Push Back loaded Material
			LoadedMaterials.push_back(tempMaterial);

			// Test to see if anything was loaded
			// If not return false
			if (LoadedMaterials.empty())
				return false;
			// If so return true
			else
				return true;
		}
	};
}

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

    char getColor(float a) {
		return colors[(int)round(a / 255 * 7)];
	}

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

		Mesh(std::string filename, char color) {
			objl::Loader loader;
			bool loaded = loader.LoadFile(filename);
			if (!loaded) {
				std::cout << "Error loading model : " << filename << std::endl;
			}

			std::vector<objl::Vertex> overts = loader.LoadedVertices;
			std::vector<unsigned int> inds = loader.LoadedIndices;

			unsigned int size = inds.size();

			faceCount = size / 3;

			float maxAmount = 0.0f;

			for (int i = 0; i < size; i++) {
				int index = inds[i];
				float x = overts[index].Position.X;
				float y = overts[index].Position.Y;
				float z = overts[index].Position.Z;
				float amt = sqrt(x * x + y * y + z * z);
				if (amt > maxAmount) maxAmount = amt;
			}

            faces = new Face[faceCount];

            int a = 0;
			for (int i = 0; i < size; i += 3) {
				int index = inds[i];
                Vector3 v1(overts[index].Position.X, overts[index].Position.Y, overts[index].Position.Z);
                Vector3 v2(overts[index + 1].Position.X, overts[index + 1].Position.Y, overts[index + 1].Position.Z);
                Vector3 v3(overts[index + 2].Position.X, overts[index + 2].Position.Y, overts[index + 2].Position.Z);
                Face f(v1, v2, v3, getColor(rand() % 255));
                faces[a] = f;
                a++;
			}

			overts.clear();
			inds.clear();
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

	//const int res = 13;
	const int res = 16;

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
		MoveWindow(console, r.left, r.top, width * res + 50, height * res + res + 50, TRUE);

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

    float cg_min(float a, float b){
        if(a < b) return a;
        return b;
    }

    float cg_max(float a, float b){
        if(a > b) return a;
        return b;
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

	void renderToFile(RenderBuffer buffer, std::string filename) {
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

		std::ofstream file(filename.c_str());
		file << ss.str();
		file.close();
	}
}

#define Vector3 CG::Vector3

namespace BasicShapes {
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
