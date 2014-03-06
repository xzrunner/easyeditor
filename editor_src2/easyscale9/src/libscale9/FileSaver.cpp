#include "FileSaver.h"

namespace escale9
{

void FileSaver::store(const char* filepath, const d2d::Scale9Symbol& symbol)
{
	Json::Value value;
	Json::Reader reader;
	std::ifstream fin(filepath);
	reader.parse(fin, value);
	fin.close();

	value["width"] = symbol.getSize().xLength();
	value["height"] = symbol.getSize().yLength();

	Json::StyledStreamWriter writer;
	std::ofstream fout(filepath);
	writer.write(fout, value);
	fout.close();
}

//void FileSaver::store(const std::string& filepath, const d2d::Scale9Symbol& symbol)
//{
//	Json::Value value;
//
//	value["width"] = symbol.getWidth();
//	value["height"] = symbol.getHeight();
//
//	std::string dlg = d2d::FilenameTools::getFileDir(filepath);
//	for (size_t i = 0; i < 3; ++i)
//		for (size_t j = 0; j < 3; ++j)
//			value["sprite"][i*3+j] = store(symbol.m_sprites[i][j], dlg);
//
//	Json::StyledStreamWriter writer;
//	std::ofstream fout(filepath.c_str());
//	writer.write(fout, value);
//	fout.close();
//}
//
//Json::Value FileSaver::store(d2d::ISprite* sprite, const std::string& dlg)
//{
//	Json::Value value;
//
//	value["filepath"] = d2d::FilenameTools::getRelativePath(dlg,
//		sprite->getSymbol().getFilepath()).ToStdString();
//
//	value["position"]["x"] = sprite->getPosition().x;
//	value["position"]["y"] = sprite->getPosition().y;
//
//	value["angle"] = sprite->getAngle();
//
//	value["scale"] = sprite->getScale().x;
//
//	bool xMirror, yMirror;
//	sprite->getMirror(xMirror, yMirror);
//	value["x mirror"] = xMirror;
//	value["y mirror"] = yMirror;
//
//	return value;
//}
} // escale9