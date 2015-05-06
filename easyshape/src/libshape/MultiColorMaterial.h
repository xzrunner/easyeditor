//#ifndef _LIBSHAPE_MULTI_COLOR_MATERIAL_H_
//#define _LIBSHAPE_MULTI_COLOR_MATERIAL_H_
//
//#include "Material.h"
//
//namespace libshape
//{
//
//class MultiColorMaterial : public Material
//{
//public:
//	MultiColorMaterial(const std::vector<d2d::Vector>& vertices,
//		const d2d::Colorf& color);
//
//	virtual Json::Value Store(const std::string& dirpath) const;
//
//	virtual void Draw(const d2d::Matrix& mt) const;
//
//	virtual void ReloadTexture() {}
//
//protected:
//	virtual void Build(const std::vector<d2d::Vector>& vertices);
//	virtual void Build(const std::vector<d2d::Vector>& vertices,
//		const std::vector<d2d::Vector>& segments);
//
//private:
//	std::vector<d2d::Colorf> m_colors;
//
//}; // MultiColorMaterial
//
//}
//
//#endif // _LIBSHAPE_MULTI_COLOR_MATERIAL_H_