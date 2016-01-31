#ifndef _LIBSHAPE_INTERFACE_MATERIAL_H_
#define _LIBSHAPE_INTERFACE_MATERIAL_H_



namespace libshape
{

class Material : public ee::Object
{
public:
	Material() {}
	virtual ~Material() {}

	virtual Json::Value Store(const std::string& dirpath) const = 0;

	virtual void Translate(const ee::Vector& offset);
	virtual void Draw(const ee::Matrix& mt, const ee::ColorTrans& color) const = 0;

	virtual void ReloadTexture() = 0;

	void BuildBegin(const std::vector<ee::Vector>& outline);
	void BuildSetSegments(const std::vector<ee::Vector>& segs);
	void BuildSetHoles(const std::vector<std::vector<ee::Vector> >& holes);
	virtual void BuildEnd() = 0;

	const std::vector<ee::Vector>& GetTriangles() const { return m_tris; }

	void Refresh(const std::vector<ee::Vector>& vertices);

	void DebugDrawTris(const ee::Matrix& mt) const;

protected:
	std::vector<ee::Vector> m_tris;

	// shape
	std::vector<ee::Vector> m_outline;
	std::vector<ee::Vector> m_segments;
	std::vector<std::vector<ee::Vector> > m_holes;

}; // Material

}

#endif // _LIBSHAPE_INTERFACE_MATERIAL_H_