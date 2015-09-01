#ifndef _LIBCOMPLEX_FILE_LOADER_H_
#define _LIBCOMPLEX_FILE_LOADER_H_

#include <drag2d.h>
#include <easycoco.h>

namespace ecomplex
{

class Symbol;

class FileLoader
{
public:
	static void Load(const std::string& filepath, Symbol* complex);

private:
	static void LoadJson(const Json::Value& value, const std::string& dir, Symbol* complex);
	static void LoadLua(const Json::Value& value, const std::string& dir, Symbol* complex);

	static void LoadImages(const std::string& name, std::vector<d2d::Image*>& images);

	static void LoadFromNode(const libcoco::IPackNode* node, Symbol* symbol);
	static d2d::ISprite* Node2Sprite(const libcoco::IPackNode* node);
	static d2d::ISprite* Quad2Sprite(const libcoco::PackPicture::Quad* quad);
	static d2d::ISprite* Pic2Sprite(const libcoco::PackPicture* pic);
	static d2d::ISprite* Label2Sprite(const libcoco::PackLabel* label);
	static d2d::ISprite* Anim2Sprite(const libcoco::PackAnimation* anim);

}; // FileLoader

}

#endif // _LIBCOMPLEX_FILE_LOADER_H_