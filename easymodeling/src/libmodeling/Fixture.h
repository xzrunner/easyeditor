
#ifndef LIBMODELING_FIXTURE_H
#define LIBMODELING_FIXTURE_H



namespace libmodeling
{
	class Body;

	class Fixture
	{
	public:
		Fixture();
		~Fixture();

		bool isContain(const ee::Vector& pos) const;
		bool isIntersect(const ee::Rect& rect) const;

		void draw(const ee::Matrix& mt, const ee::Colorf& cFace, 
			const ee::Colorf& cEdge) const;

	private:
		void transLocalToWorld(const std::vector<ee::Vector>& local, std::vector<ee::Vector>& world) const;

	public:
		wxString name;

		Body* body;

		ee::Shape* shape;

		float density;

		float friction;
		float restitution;

		bool isSensor;

		unsigned short categoryBits;
		unsigned short maskBits;
		short groupIndex;

	}; // Fixture
}

#endif // LIBMODELING_FIXTURE_H
