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
//	MultiColorMaterial(const std::vector<sm::vec2>& vertices,
//		const ee::Colorf& color);
//
//	virtual Json::Value Store(const std::string& dirpath) const;
//
//	virtual void Draw(const sm::mat4& mt) const;
//
//	virtual void ReloadTexture() {}
//
//protected:
//	virtual void Build(const std::vector<sm::vec2>& vertices);
//	virtual void Build(const std::vector<sm::vec2>& vertices,
//		const std::vector<sm::vec2>& segments);
//
//private:
//	std::vector<ee::Colorf> m_colors;
//
//}; // MultiColorMaterial
//
//}
//
//#endif // _EASYSHAPE_MULTI_COLOR_MATERIAL_H_