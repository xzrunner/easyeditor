#ifndef _LIBSHAPE_POLYGON_SHAPE_H_
#define _LIBSHAPE_POLYGON_SHAPE_H_

#include "ChainShape.h"

namespace libshape
{

class PolygonShape : public ChainShape
{
public:
	PolygonShape(const PolygonShape& polygon);
	PolygonShape(const std::vector<d2d::Vector>& vertices);
	virtual ~PolygonShape();
	
	//
	// IObject interface
	//
	virtual PolygonShape* clone() const;

	//
	// IShape interface
	//
	virtual bool isContain(const d2d::Vector& pos) const;
	// todo
	//virtual bool isIntersect(const d2d::Rect& rect) const;
	virtual void Translate(const d2d::Vector& offset);
	virtual void draw(const d2d::Colorf& color = d2d::Colorf(0, 0, 0)) const;
	virtual d2d::IPropertySetting* createPropertySetting(d2d::EditPanel* editPanel);

	//
	// ChainShape interface
	//
	virtual void refresh();

	void SetMaterialColor(const d2d::Colorf& color);
	void SetMaterialTexture(d2d::ImageSymbol* image);

private:
	class Material : public ICloneable
	{
	public:
		Material() {}
		Material(const Material& material);
		virtual ~Material() {}

		virtual Material* clone() const = 0;

		virtual void Translate(const d2d::Vector& offset);
		virtual void Draw() const = 0;

		void Refresh(const std::vector<d2d::Vector>& vertices);

		// for debug
		void DrawTrisEdge() const;

	protected:
		virtual void Build(const std::vector<d2d::Vector>& vertices) = 0;
		virtual void Build(const std::vector<d2d::Vector>& vertices,
			const std::vector<d2d::Vector>& segments) = 0;

	protected:
		std::vector<d2d::Vector> m_vertices;

	}; // Material

	class Color : public Material
	{
	public:
		Color(const std::vector<d2d::Vector>& vertices,
			const d2d::Colorf& color);
		Color(const Color& color);

		virtual Color* clone() const;

		virtual void Draw() const;

	protected:
		virtual void Build(const std::vector<d2d::Vector>& vertices);
		virtual void Build(const std::vector<d2d::Vector>& vertices,
			const std::vector<d2d::Vector>& segments);

	private:
		d2d::Colorf m_color;

	}; // Color

	class Texture : public Material
	{
	public:
		Texture(const std::vector<d2d::Vector>& vertices,
			d2d::ImageSymbol* image);
		Texture(const Texture& texture);
		virtual ~Texture();

		virtual Texture* clone() const;

		virtual void Translate(const d2d::Vector& offset);
		virtual void Draw() const;

	protected:
		virtual void Build(const std::vector<d2d::Vector>& vertices);
		virtual void Build(const std::vector<d2d::Vector>& vertices,
			const std::vector<d2d::Vector>& segments);

	private:
		d2d::Rect GetBoundingRegion(const std::vector<d2d::Vector>& bounding) const;

		void GetTexBoundarySegments(const d2d::Rect& rect, std::vector<d2d::Vector>& segments);

		void CalTexcoords(const d2d::Rect& rect);

	private:
		d2d::ImageSymbol* m_image;

		std::vector<d2d::Vector> m_texcoords;

	}; // Texture

private:
	Material* m_material;

}; // PolygonShape

}

#endif // _LIBSHAPE_POLYGON_SHAPE_H_