#pragma once

#include "Mesh.h"

namespace eanim
{
	class PartSkeleton : public ee::ICloneable, public ee::ISerializable
	{
	public:
		struct Node;
		class Joint;

	public:
		PartSkeleton(float width, float height);
		PartSkeleton(const PartSkeleton& skeleton);
		~PartSkeleton();

		//
		// ee::ICloneable interface
		//
		virtual PartSkeleton* clone() const;

		//
		// ee::ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		void pushJoint(const ee::Vector& pos);
		void popJoint();
		ee::Vector topJoint() const;

		Joint* queryJointByPos(const ee::Vector& pos);
		Node* queryNodeByPos(const ee::Vector& pos);

		void translateJoint(Joint* joint, const ee::Vector& newPos);
		void translateNode(Node* node, const ee::Vector& newPos);
		void rotateJoint(Joint* joint, const ee::Vector& newPos);

		void onDraw() const;
		void drawMeshes() const;

		void tween(const PartSkeleton& start, const PartSkeleton& end, float process);

	private:
		void clear();

		void drawTriangle(const ee::Vector& p0, const ee::Vector& p1, const ee::Vector& p2) const;
		void drawTriangle(const ee::Vector& p0, const ee::Vector& p1, const ee::Vector& p2,
			const ee::Vector& c0, const ee::Vector& c1, const ee::Vector& c2) const;

	public:
		struct Node : public MeshNode
		{
			ee::Vector local;	// at the coordinate system which positive x is from start joint to end joint

			Node() {}
			Node(const Node& node);

			//
			// ee::ISerializable interface
			//
			virtual void loadFromTextFile(std::ifstream& fin);
			virtual void storeToTextFile(std::ofstream& fout) const;

		}; // Node

		class Bone;
		class Joint : public MeshNode
		{
		public:
			Joint();
			Joint(const ee::Vector& p, float hWidth, float hHeight);
			Joint(const Joint& joint);

			//
			// ee::ISerializable interface
			//
			virtual void loadFromTextFile(std::ifstream& fin);
			virtual void storeToTextFile(std::ofstream& fout) const;

			void initNodesLocalCoords(float length);
			void initConnectedCoords(const ee::Vector& pos, float hWidth, float hHeight);

			void getNodesPos(ee::Vector& left, ee::Vector& right) const;

		private:
			Node m_left, m_right;
			Bone *m_from, *m_to;

			friend class PartSkeleton;

		}; // Joint

		class Bone : public ee::ISerializable
		{
		public:
			Bone();
			Bone(Joint* start, Joint* end, float length, float hWidth, float hHeight);
			Bone(const Bone& bone);

			//
			// ee::ISerializable interface
			//
			virtual void loadFromTextFile(std::ifstream& fin);
			virtual void storeToTextFile(std::ofstream& fout) const;

			float getLength() const;
			ee::Vector getCenter() const;
			ee::Vector getVector() const;

			void getNodesPos(ee::Vector& left, ee::Vector& right) const;

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

