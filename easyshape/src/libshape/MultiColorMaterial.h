//#ifndef _EASYSHAPE_MULTI_COLOR_MATERIAL_H_
//#define _EASYSHAPE_MULTI_COLOR_MATERIAL_H_
//
//#include "Material.h"
//
//namespace eshape
//{
//
//class MultiColorMaterial : public Material
//{
//public:
//	MultiColorMaterial(const CU_VEC<sm::vec2>& vertices,
//		const s2::Color& color);
//
//	virtual Json::Value Store(const std::string& dirpath) const;
//
//	virtual void Draw(const S2_MAT& mt) const;
//
//	virtual void ReloadTexture() {}
//
//protected:
//	virtual void Build(const CU_VEC<sm::vec2>& vertices);
//	virtual void Build(const CU_VEC<sm::vec2>& vertices,
//		const CU_VEC<sm::vec2>& segments);
//
//private:
//	CU_VEC<s2::Color> m_colors;
//
//}; // MultiColorMaterial
//
//}
//
//#endif // _EASYSHAPE_MULTI_COLOR_MATERIAL_H_