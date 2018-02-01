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
//		const pt2::Color& color);
//
//	virtual Json::Value Store(const std::string& dirpath) const override;
//
//	virtual void Draw(const S2_MAT& mt) const override;
//
//	virtual void ReloadTexture() override {}
//
//protected:
//	virtual void Build(const CU_VEC<sm::vec2>& vertices) override;
//	virtual void Build(const CU_VEC<sm::vec2>& vertices,
//		const CU_VEC<sm::vec2>& segments) override;
//
//private:
//	CU_VEC<pt2::Color> m_colors;
//
//}; // MultiColorMaterial
//
//}
//
//#endif // _EASYSHAPE_MULTI_COLOR_MATERIAL_H_