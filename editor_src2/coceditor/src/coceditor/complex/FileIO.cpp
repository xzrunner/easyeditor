#include "FileIO.h"

#include "Context.h"

namespace coceditor
{
namespace complex
{
	void FileIO::store(const char* filepath, const complex::Sprite* complexSymbol)
	{
		Json::Value value;

		for (size_t i = 0, n = complexSymbol->m_sprites.size(); i < n; ++i)
			value["sprite"][i] = store(complexSymbol->m_sprites[i]);

		Json::StyledStreamWriter writer;
		std::ofstream fout(filepath);
		writer.write(fout, value);
		fout.close();
	}

	Json::Value FileIO::store(d2d::ISprite* sprite)
	{
		Json::Value value;

//		value["id"] = *static_cast<int*>(sprite->getUserData());
		void* data = sprite->getUserData();

		value["filepath"] = d2d::FilenameTools::getRelativePath(
			d2d::Context::Instance()->getDir(),
			sprite->getSymbol().getFilepath()).ToStdString();

		value["position"]["x"] = sprite->getPosition().x;
		value["position"]["y"] = sprite->getPosition().y;

		value["angle"] = sprite->getAngle();

		value["scale"] = sprite->getScaleX();

		bool xMirror, yMirror;
		sprite->getMirror(xMirror, yMirror);
		value["x mirror"] = xMirror;
		value["y mirror"] = yMirror;

		return value;
	}
}
}