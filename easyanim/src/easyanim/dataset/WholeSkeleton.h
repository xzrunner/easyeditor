#pragma once



namespace eanim
{
	namespace edit_whole_skeleton { class RotateBoneAOP; }

	class Symbol;

	class WholeSkeleton : public ee::ListItem
	{
	public:
		struct Node;
		class Body;
		class Sprite;

	public:
		WholeSkeleton();
		WholeSkeleton(const WholeSkeleton& skeleton);
		virtual ~WholeSkeleton();

		//
		// ee::ICloneable interface
		//
		virtual WholeSkeleton* clone() const;

		void loadFromTextFile(std::ifstream& fin, const std::vector<Node*>& nodes,
			const std::vector<Body*>& bodies);
		void storeToTextFile(std::ofstream& fout, const std::vector<Node*>& nodes,
			const std::vector<Body*>& bodies) const;

		void traverse(ee::IVisitor& visitor);

		void onDraw();

		void getBounding(ee::AbstractBV& bounding);

		Sprite* queryByPos(const ee::Vector& pos);

		const wxString& getName() const;

		void tween(const WholeSkeleton& start, const WholeSkeleton& end, float process);

	private:
		void clear();

	public:
		struct Node
		{
			ee::Vector pos;

			Node();
			Node(const ee::Vector& p);

		}; // Node

		class Body : public ee::ISerializable
		{
		public:
			Body(Symbol* symbol);
			~Body();

			//
			// ee::ISerializable interface
			//
			virtual void loadFromTextFile(std::ifstream& fin);
			virtual void storeToTextFile(std::ofstream& fout) const;

			void onDraw() const;

			Node* queryNodeByPos(const ee::Vector& pos);
			void insertNode(const ee::Vector& pos);
			void eraseNode(Node* node);

			Symbol* getSymbol() { return m_symbol; }

			const std::vector<Node*>& getNodes() const { return m_nodes; }

		private:
			void clear();

		public:
			static const float NODE_RADIUS;

		private:
			Symbol* m_symbol;

			std::vector<Node*> m_nodes;

			friend class WholeSkeleton;
			friend class ComposeWholeSkeletonOP;

		}; // Body

		class Sprite : public ee::ImageSprite
		{
		public:
			Sprite();
			Sprite(Body* body, const ee::Vector& pos);
			Sprite(const Sprite& sprite, Sprite* parent = NULL);
			~Sprite();
			
			//
			// ee::ICloneable interface
			//
			virtual Sprite* clone() const;

			void loadFromTextFile(std::ifstream& fin, const std::vector<Node*>& nodes,
				const std::vector<Body*>& bodies, const std::vector<Sprite*>& sprites);
			void storeToTextFile(std::ofstream& fout, const std::vector<Node*>& nodes,
				const std::vector<Body*>& bodies, const std::vector<Sprite*>& sprites) const;

			void loadFromTextFile(std::ifstream& fin);
			void storeToTextFile(std::ofstream& fout) const;

			void traverse(ee::IVisitor& visitor);

			void remove();

			void onDraw() const;
			void drawPosterity();

			int getLayer() const;
			void setLayer(int layer);

			Symbol* getSymbol();

			ee::Vector getNodeWorldCoords(Node* node) const;

		private:
			void clear();

			void initBounding();

			void drawBone(const Sprite* bone, const Sprite* curr, const Sprite* currChild, int& times) const;

			void computePosterityAbsoluteCoords();

		private:
			struct RelativeCoords
			{
				float delta;
				Node *from, *to;

				RelativeCoords() : delta(0.0f), from(NULL), to(NULL) {}
				RelativeCoords(const RelativeCoords& rc) {
					delta = rc.delta;
					from = rc.from;
					to = rc.to;
				}

				void loadFromTextFile(std::ifstream& fin, const std::vector<Node*>& nodes);
				void storeToTextFile(std::ofstream& fout, const std::vector<Node*>& nodes) const;

			}; // RelativeCoords

			class PosterityAbsoluteCoordsVisitor : public ee::IVisitor
			{
			public:
				virtual void visit(ee::ICloneable* object, bool& bFetchNext);
			}; // PosterityAbsoluteCoordsVisitor 
			
		private:
			static const int MIN_LAYER = 1;
			static const int MAX_LAYER = 10;

		private:
			Body* m_body;

			Sprite* m_parent;
			std::vector<Sprite*> m_children;

			RelativeCoords m_relativeCoords;

			int m_layer;	// [MIN_LAYER, MAX_LAYER]

			friend class WholeSkeleton;
			friend class ComposeWholeSkeletonOP;
			friend class EditWholeSkeletonOP;
			friend class edit_whole_skeleton::RotateBoneAOP;

		}; // Sprite

		class BoneCmp
		{
		public:
			bool operator() (const Sprite* s0, const Sprite* s1) const;
		}; // BoneCmp

		class FetchAllBonesVisitor : public ee::IVisitor
		{
		public:
			FetchAllBonesVisitor(std::vector<Sprite*>& bones);
			virtual void visit(ee::ICloneable* object, bool& bFetchNext);

		private:
			std::vector<Sprite*>& m_bones;

		}; // FetchAllBonesVisitor

		class DrawVisitor : public ee::IVisitor
		{
		public:
			DrawVisitor();
			virtual void visit(ee::ICloneable* object, bool& bFetchNext);

		private:
			void drawBone(Sprite* bone, Sprite* curr, int times);

		}; // DrawVisitor

		class ComputeBoundingVisitor : public ee::IVisitor
		{
		public:
			ComputeBoundingVisitor(ee::AbstractBV& bounding);
			virtual void visit(ee::ICloneable* object, bool& bFetchNext);

		private:
			ee::AbstractBV& m_bounding;

		}; // ComputeBoundingVisitor

		class PointQueryVisitor : public ee::IVisitor
		{
		public:
			PointQueryVisitor(const ee::Vector& pos, Sprite** pSelected);
			virtual void visit(ee::ICloneable* object, bool& bFetchNext);

		private:
			const ee::Vector& m_pos;
			Sprite** m_pSelected;

		}; // PointQueryVisitor

	private:
		Sprite* m_root;

		friend class ComposeWholeSkeletonOP;

	}; // WholeSkeleton
}

