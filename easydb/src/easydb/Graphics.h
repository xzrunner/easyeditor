#pragma once



namespace edb
{
	struct Node
	{
		std::vector<ee::Sprite*> out, in;
	};

	class Graphics
	{
	public:
		void connect(ee::Sprite* from, ee::Sprite* to);

		void move(ee::Sprite* sprite, const ee::Vector& offset);

		Node* query(ee::Sprite* sprite) const;

		void clear();

	public:
		std::map<ee::Sprite*, Node*> connection;
	};
}