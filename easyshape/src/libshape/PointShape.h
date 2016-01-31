#ifndef _LIBSHAPE_POINT_SHAPE_H_
#define _LIBSHAPE_POINT_SHAPE_H_



namespace libshape
{

class PointShape : public ee::Shape
{
public:
	PointShape();
	PointShape(const ee::Vector& pos);
	PointShape(const PointShape& point);

	//
	// IObject interface
	//
	virtual PointShape* Clone() const;

	//
	// Shape interface
	//
	virtual const char* GetShapeDesc() const { return "point"; }
	virtual bool IsContain(const ee::Vector& pos) const;
	virtual bool IsIntersect(const ee::Rect& rect) const;
	virtual void Translate(const ee::Vector& offset);
	virtual const ee::Rect& GetRect() const { return m_rect; }
	virtual void Draw(const ee::Matrix& mt,
		const ee::ColorTrans& color = ee::ColorTrans()) const;
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;

	const ee::Vector& GetPos() const { return m_pos; }
	void SetPos(const ee::Vector& pos) { m_pos = pos; }

private:
	static const int RADIUS = 5;

private:
	ee::Vector m_pos;

	ee::Rect m_rect;

}; // PointShape

}

#endif // _LIBSHAPE_POINT_SHAPE_H_