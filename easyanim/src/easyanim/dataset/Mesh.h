#pragma once

#include <drag2d.h>

namespace eanim
{
	typedef d2d::DelaunayTriangulation DT;

	struct MeshNode : public d2d::Object, public d2d::ISerializable
	{
		d2d::Vector texCoords, projCoords;

		MeshNode();
		MeshNode(const MeshNode& node);
		MeshNode(const d2d::Vector& pos, float hWidth, float hHeight);
		MeshNode& operator = (const MeshNode& node);

		//
		// d2d::ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		void initCoords(const d2d::Vector& pos, float hWidth, float hHeight);
	};

	class MeshNodeCmp
	{
	public:
		bool operator () (const MeshNode* lhs, const MeshNode* rhs) const {
			return lhs->projCoords.x < rhs->projCoords.x 
				|| lhs->projCoords.x == rhs->projCoords.x && lhs->projCoords.y < rhs->projCoords.y;
		}
	}; // MeshNodeCmp

	class MeshTri : public d2d::ICloneable
	{
	public:
		MeshTri();
		MeshTri(const MeshTri& tri);
		~MeshTri();

		//
		// d2d::ICloneable interface
		//
		virtual MeshTri* clone() const;

		void getNodes(std::vector<MeshNode*>& nodes);

	private:
		MeshNode* m_nodes[3];

		friend class Mesh;
		friend class MorphingSprite;

	}; // MeshTri

	class Mesh : public d2d::ICloneable, public d2d::ISerializable
	{
	public:
		Mesh(float width, float height);
		Mesh(const Mesh& mesh);
		~Mesh();

		//
		// d2d::ICloneable interface
		//
		virtual Mesh* clone() const;

		//
		// d2d::ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		void insertNode(const d2d::Vector& pos);
		void deleteNode(const d2d::Vector& pos);
		const std::vector<MeshTri*>& getAllTris() const;

		bool isMerged() const;
		void mergeNodes();
		void separateNodes();

		void tween(const Mesh& start, const Mesh& end, float process);

		void clear();

	private:
		void loadTrisFromDT();

	private:
		std::vector<MeshTri*> m_tris;

		DT* m_dt;

		float m_halfWidth, m_halfHeight;

		bool m_isMerged;

	}; // Mesh
}

