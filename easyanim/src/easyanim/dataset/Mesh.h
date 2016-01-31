#pragma once



namespace eanim
{
	typedef ee::DelaunayTriangulation DT;

	struct MeshNode : public ee::Object, public ee::ISerializable
	{
		ee::Vector texCoords, projCoords;

		MeshNode();
		MeshNode(const MeshNode& node);
		MeshNode(const ee::Vector& pos, float hWidth, float hHeight);
		MeshNode& operator = (const MeshNode& node);

		//
		// ee::ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		void initCoords(const ee::Vector& pos, float hWidth, float hHeight);
	};

	class MeshNodeCmp
	{
	public:
		bool operator () (const MeshNode* lhs, const MeshNode* rhs) const {
			return lhs->projCoords.x < rhs->projCoords.x 
				|| lhs->projCoords.x == rhs->projCoords.x && lhs->projCoords.y < rhs->projCoords.y;
		}
	}; // MeshNodeCmp

	class MeshTri : public ee::ICloneable
	{
	public:
		MeshTri();
		MeshTri(const MeshTri& tri);
		~MeshTri();

		//
		// ee::ICloneable interface
		//
		virtual MeshTri* clone() const;

		void getNodes(std::vector<MeshNode*>& nodes);

	private:
		MeshNode* m_nodes[3];

		friend class Mesh;
		friend class MorphingSprite;

	}; // MeshTri

	class Mesh : public ee::ICloneable, public ee::ISerializable
	{
	public:
		Mesh(float width, float height);
		Mesh(const Mesh& mesh);
		~Mesh();

		//
		// ee::ICloneable interface
		//
		virtual Mesh* clone() const;

		//
		// ee::ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		void insertNode(const ee::Vector& pos);
		void deleteNode(const ee::Vector& pos);
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

