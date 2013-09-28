#pragma once

#include "Mesh.h"

namespace eanim
{
	class PartSkeleton : public d2d::ICloneable, public d2d::ISerializable
	{
	public:
		struct Node;
		class Joint;

	public:
		PartSkeleton(float width, float height);
		PartSkeleton(const PartSkeleton& skeleton);
		~PartSkeleton();

		//
		// d2d::ICloneable interface
		//
		virtual PartSkeleton* clone();

		//
		// d2d::ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		void pushJoint(const d2d::Vector& pos);
		void popJoint();
		d2d::Vector topJoint() const;

		Joint* queryJointByPos(const d2d::Vector& pos);
		Node* queryNodeByPos(const d2d::Vector& pos);

		void translateJoint(Joint* joint, const d2d::Vector& newPos);
		void translateNode(Node* node, const d2d::Vector& newPos);
		void rotateJoint(Joint* joint, const d2d::Vector& newPos);

		void onDraw() const;
		void drawMeshes() const;

		void tween(const PartSkeleton& start, const PartSkeleton& end, float process);

	private:
		void clear();

		void drawTriangle(const d2d::Vector& p0, const d2d::Vector& p1, const d2d::Vector& p2) const;
		void drawTriangle(const d2d::Vector& p0, const d2d::Vector& p1, const d2d::Vector& p2,
			const d2d::Vector& c0, const d2d::Vector& c1, const d2d::Vector& c2) const;

	public:
		struct Node : public MeshNode
		{
			d2d::Vector local;	// at the coordinate system which positive x is from start joint to end joint

			Node() {}
			Node(const Node& node);

			//
			// d2d::ISerializable interface
			//
			virtual void loadFromTextFile(std::ifstream& fin);
			virtual void storeToTextFile(std::ofstream& fout) const;

		}; // Node

		class Bone;
		class Joint : public MeshNode
		{
		public:
			Joint();
			Joint(const d2d::Vector& p, float hWidth, float hHeight);
			Joint(const Joint& joint);

			//
			// d2d::ISerializable interface
			//
			virtual void loadFromTextFile(std::ifstream& fin);
			virtual void storeToTextFile(std::ofstream& fout) const;

			void initNodesLocalCoords(float length);
			void initConnectedCoords(const d2d::Vector& pos, float hWidth, float hHeight);

			void getNodesPos(d2d::Vector& left, d2d::Vector& right) const;

		private:
			Node m_left, m_right;
			Bone *m_from, *m_to;

			friend class PartSkeleton;

		}; // Joint

		class Bone : public d2d::ISerializable
		{
		public:
			Bone();
			Bone(Joint* start, Joint* end, float length, float hWidth, float hHeight);
			Bone(const Bone& bone);

			//
			// d2d::ISerializable interface
			//
			virtual void loadFromTextFile(std::ifstream& fin);
			virtual void storeToTextFile(std::ofstream& fout) const;

			float getLength() const;
			d2d::Vector getCenter() const;
			d2d::Vector getVector() const;

			void getNodesPos(d2d::Vector& left, d2d::Vector& right) const;

			void initNodesLocalCoords(float length);
			void initCoords(float hWidth, float hHeight, Joint* except);

		private:
			Joint *m_start, *m_end;
			Node m_left, m_right;

			friend class PartSkeleton;

		}; // Bone

	private:
		static const float JOINT_RADIUS;
		static const float NODE_RADIUS;
		static const float WIDTH_LENGTH_RATIO;

	private:
		float m_halfWidth, m_halfHeight;

		std::vector<Joint*> m_joints;

	}; // PartSkeleton
}

