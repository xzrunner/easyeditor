#include "FileIO.h"
#include "StagePanel.h"
#include "ToolBarPanel.h"
#include "Context.h"

#include <SOIL/SOIL.h>
// #include <SOIL/stb_image_write.h>

using namespace epacker;

void FileIO::load(const char* filename)
{
	Context* context = Context::Instance();

	d2d::SymbolMgr::Instance()->clear();
	d2d::BitmapMgr::Instance()->clear();

	d2d::TexPackerAdapter adapter;
	adapter.load(filename);
	context->toolbar->setSize(adapter.width, adapter.height);

	for (size_t i = 0, n = adapter.textures.size(); i < n; ++i)
	{
		d2d::TexPackerAdapter::Texture tex = adapter.textures[i];
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(tex.filepath);
		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
		symbol->release();

		d2d::Vector pos;
		pos.x = tex.region.left + tex.region.width * 0.5f;
		pos.y = tex.region.low + tex.region.height * 0.5f;
		if (tex.bRotate)
			sprite->setTransform(pos, d2d::PI * 0.5f);
		else
			sprite->setTransform(pos, 0);

		context->stage->insertSpriteNoArrange(sprite);
	}

	context->library->loadFromSymbolMgr(*d2d::SymbolMgr::Instance());
}

void FileIO::store(const char* filename)
{
	storeImage(filename);
	storePosition(filename);
}

void FileIO::storeImage(const char* filename)
{
	StagePanel* stage = Context::Instance()->stage;

	const int width = Context::Instance()->width,
		height = Context::Instance()->height;
	int channel = 4;
	ToolbarPanel::IMG_TYPE type = Context::Instance()->toolbar->getImgType();
	if (type == ToolbarPanel::e_bmp || type == ToolbarPanel::e_jpg) 
		channel = 3;

	unsigned char* dst_data = (unsigned char*) malloc(channel * width * height);
	memset(dst_data, 0, channel * width * height);

	std::vector<d2d::ISprite*> sprites;
	stage->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = sprites[i];
		const d2d::Vector& center = sprite->getPosition();

		float sw, sh;
		if (sprite->getAngle() == 0)
		{
			sw = sprite->getSymbol().getSize().xLength();
			sh = sprite->getSymbol().getSize().yLength();
		}
		else
		{
			sw = sprite->getSymbol().getSize().yLength();
			sh = sprite->getSymbol().getSize().xLength();
		}

		if (sprite->getPosition().x - sw * 0.5f < 0 || sprite->getPosition().x + sw * 0.5f > width ||
			sprite->getPosition().y - sh * 0.5f < 0 || sprite->getPosition().y + sh * 0.5f > width)
			continue;

		int w, h, c;
		unsigned char* src_data = SOIL_load_image(sprite->getSymbol().getFilepath().c_str(), &w, &h, &c, 0);

		if (sprite->getAngle() != 0)
		{
			for (size_t iRow = 0; iRow < w; ++iRow) {
				for (size_t iCol = 0; iCol < h; ++iCol) {
					const int baseFrom = (iCol * w + (w - 1 - iRow)) * c,
						baseTo = ((height - center.y - w * 0.5f + iRow) * width + center.x - h * 0.5f + iCol) * channel;
					for (size_t iCanel = 0; iCanel < channel; ++iCanel)
						dst_data[baseTo + iCanel] = src_data[baseFrom + iCanel];
				}
			}
		}
		else
		{
			for (size_t iRow = 0; iRow < h; ++iRow) {
				for (size_t iCol = 0; iCol < w; ++iCol) {
					const int baseFrom = (iRow * w + iCol) * c,
						baseTo = ((height - center.y - h * 0.5f + iRow) * width + center.x - w * 0.5f + iCol) * channel;
					for (size_t iCanel = 0; iCanel < channel; ++iCanel)
						dst_data[baseTo + iCanel] = src_data[baseFrom + iCanel];
				}
			}
		}

		delete[] src_data;
	}

	wxString imgFile(filename);
	imgFile = d2d::FilenameTools::getFilePathExceptExtension(imgFile);

	switch (type)
	{
	case ToolbarPanel::e_bmp:
//		stbi_write_bmp((imgFile + ".bmp").c_str(), width, height, channel, dst_data);
		break;
	case ToolbarPanel::e_jpg:
		{
			d2d::LibJpeg::ImageData data;
			data.width = width;
			data.height = height;
			data.pixels = dst_data;

			d2d::LibJpeg::write_JPEG_file((imgFile + ".jpg").c_str(), 80, data);
		}
		break;
	case ToolbarPanel::e_png:
//		stbi_write_png((imgFile + ".png").c_str(), width, height, channel, dst_data, 0);
		break;
	}

	free((void*)dst_data);
}

void FileIO::storePosition(const char* filename)
{
	Json::Value value;

	value["width"] = Context::Instance()->width;
	value["height"] = Context::Instance()->height;

	std::vector<d2d::ISprite*> sprites;
	Context::Instance()->stage->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		value["image"][i] = store(sprites[i]);

	Json::StyledStreamWriter writer;
	std::ofstream fout(filename);
	writer.write(fout, value);
	fout.close();
}

Json::Value FileIO::store(const d2d::ISprite* sprite)
{
	Json::Value value;

	const d2d::ISymbol& symbol = sprite->getSymbol();
	const float w = symbol.getSize().xLength(),
		h = symbol.getSize().yLength();
	const d2d::Vector& pos = sprite->getPosition();

	bool bRotate = sprite->getAngle() != 0;
	float left, low, width, height;
	if (bRotate)
	{
		left = pos.x - h * 0.5f;
		low = pos.y - w * 0.5f;
		width = h;
		height = w;
	}
	else
	{
		left = pos.x - w * 0.5f;
		low = pos.y - h * 0.5f;
		width = w;
		height = h;
	}

	value["filepath"] = symbol.getFilepath().ToStdString();
	value["left"] = left;
	value["low"] = low;
	value["width"] = width;
	value["height"] = height;
	value["rotate"] = bRotate;

	return value;
}