#include "FileIO.h"
#include "StagePanel.h"
#include "ToolBarPanel.h"
#include "Context.h"
#include "config.h"

using namespace epacker;

void FileIO::load(const char* filename)
{
	Context* context = Context::Instance();

	d2d::SymbolMgr::Instance()->Clear();
	d2d::BitmapMgr::Instance()->Clear();

	wxString ext = wxT("_") + wxString(FILE_TAG) + wxT(".json");
	if (wxString(filename).Contains(ext)) {
		loadFromEasypackerFile(filename);
	} else {
		loadFromTexPackerFile(filename);
	}

	context->library->LoadFromSymbolMgr(*d2d::SymbolMgr::Instance());
}

void FileIO::store(const char* filename)
{
	wxString ext = wxT("_") + wxString(FILE_TAG) + wxT(".json");
	if (wxString(filename).Contains(ext)) {
		storeToEasypackerFile(filename);
	} else {
		storeToTexPackerFile(filename);
	}
}

void FileIO::loadFromEasypackerFile(const char* filename)
{
	Context* context = Context::Instance();

	d2d::TexPackerAdapter adapter;
	adapter.load(filename);
	context->toolbar->setSize(adapter.width, adapter.height);

	for (size_t i = 0, n = adapter.textures.size(); i < n; ++i)
	{
		d2d::TexPackerAdapter::Texture tex = adapter.textures[i];
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(tex.filepath);
		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
		symbol->Release();

		d2d::Vector pos;
		pos.x = tex.region.left + tex.region.width * 0.5f;
		pos.y = tex.region.low + tex.region.height * 0.5f;
		if (tex.bRotate)
			sprite->setTransform(pos, d2d::PI * 0.5f);
		else
			sprite->setTransform(pos, 0);

		context->stage->insertSpriteNoArrange(sprite);
	}
}

void FileIO::loadFromTexPackerFile(const char* filename)
{
	Context* context = Context::Instance();

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	d2d::SettingData& settings = d2d::Config::Instance()->GetSettings();
	bool old_open_image_edge_clip = settings.open_image_edge_clip;
	settings.open_image_edge_clip = false;

	context->width = value["meta"]["size"]["w"].asInt();
	context->height = value["meta"]["size"]["h"].asInt();

	std::string dir = d2d::FilenameTools::getFileDir(filename);

	int i = 0;
	Json::Value frame_val = value["frames"][i++];
	while (!frame_val.isNull()) {
		std::string filepath = frame_val["filename"].asString();
		if (!d2d::FilenameTools::isExist(filepath))
			filepath = d2d::FilenameTools::getAbsolutePath(dir, filepath);

		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
		symbol->Release();

		int width = frame_val["sourceSize"]["w"].asInt();
		int height = frame_val["sourceSize"]["h"].asInt();
		d2d::Vector pos;
		float angle = 0;
		if (frame_val["rotated"].asBool())
		{
			int left = frame_val["frame"]["x"].asInt() - (height - frame_val["spriteSourceSize"]["y"].asInt() - frame_val["spriteSourceSize"]["h"].asInt());
			int top = frame_val["frame"]["y"].asInt() - frame_val["spriteSourceSize"]["x"].asInt();
 			pos.x = left + height * 0.5f;
 			pos.y = context->height - (top + width * 0.5f);
			angle = -d2d::PI*0.5f;
		}
		else
		{
			int left = frame_val["frame"]["x"].asInt() - frame_val["spriteSourceSize"]["x"].asInt();
			int top = frame_val["frame"]["y"].asInt() - frame_val["spriteSourceSize"]["y"].asInt();
			pos.x = left + width * 0.5f;
			pos.y = context->height - (top + height * 0.5f);
		}
		sprite->setTransform(pos, angle);

		sprite->editable = false;

		Json::Value* val = new Json::Value;
		*val = frame_val;
		sprite->SetUserData(val);

		context->stage->insertSpriteNoArrange(sprite);

		frame_val = value["frames"][i++];
	}

	Context::Instance()->tp_meta = value["meta"];

	settings.open_image_edge_clip = old_open_image_edge_clip;
}

void FileIO::storeToEasypackerFile(const char* filename)
{
	storeImage(filename);
	storeEasypackerPosition(filename);
}

void FileIO::storeToTexPackerFile(const char* filename)
{
	storeImage(filename);
	storeTexpackerPosition(filename);
}

void FileIO::storeImage(const char* filename)
{
	StagePanel* stage = Context::Instance()->stage;

	int extrude = Context::Instance()->extrude;

	const int width = Context::Instance()->width,
		height = Context::Instance()->height;
	int channel = 4;
	IMG_TYPE type = Context::Instance()->toolbar->getImgType();
	if (type == e_bmp || type == e_jpg) 
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
			sw = sprite->getSymbol().GetSize().xLength();
			sh = sprite->getSymbol().GetSize().yLength();
		}
		else
		{
			sw = sprite->getSymbol().GetSize().yLength();
			sh = sprite->getSymbol().GetSize().xLength();
		}

		//if (sprite->getPosition().x - sw * 0.5f < 0 || sprite->getPosition().x + sw * 0.5f > width ||
		//	sprite->getPosition().y - sh * 0.5f < 0 || sprite->getPosition().y + sh * 0.5f > width)
		//	continue;

		bool use_premultiplied_alpha = Context::Instance()->premultiplied_alpha && channel == 4;

		int w, h, c, f;
		uint8_t* src_data = d2d::ImageLoader::loadData(sprite->getSymbol().GetFilepath().ToStdString(), w, h, c, f);

		if (sprite->getAngle() != 0)
		{
			for (size_t iRow = 0; iRow < w; ++iRow) {
				for (size_t iCol = 0; iCol < h; ++iCol) {
					int dst_row = height - center.y - w * 0.5f + iRow;
					int dst_col = center.x - h * 0.5f + iCol;
					if (dst_row < 0 || dst_row >= height ||
						dst_col < 0 || dst_col >= width ) {
						continue;
					}
//					const int baseFrom = (iCol * w + (w - 1 - iRow)) * c,
//					const int baseFrom = ((h - 1 - iCol) * w + iRow) * c,
					const int baseFrom = (iCol * w + iRow) * c,
						baseTo = (dst_row * width + dst_col) * channel;
					if (channel == 4 && src_data[baseFrom + 3] == 0) {
						continue;
					}

					for (size_t iCanel = 0; iCanel < channel; ++iCanel)
						dst_data[baseTo + iCanel] = src_data[baseFrom + iCanel];

					if (use_premultiplied_alpha)
					{
						float alpha = src_data[baseFrom + 3] / 255.0f;
						for (size_t iCanel = 0; iCanel < channel-1; ++iCanel)
							dst_data[baseTo + iCanel] = dst_data[baseTo + iCanel] * alpha;
					}
				}
			}
		}
		else
		{
			for (size_t iRow = 0; iRow < h; ++iRow) {
				for (size_t iCol = 0; iCol < w; ++iCol) {
					int dst_row = height - center.y - h * 0.5f + iRow;
					int dst_col = center.x - w * 0.5f + iCol;
					if (dst_row < 0 || dst_row >= height ||
						dst_col < 0 || dst_col >= width ) {
							continue;
					}
//					const int baseFrom = (iRow * w + iCol) * c,
					const int baseFrom = ((h - 1 - iRow) * w + iCol) * c,
						baseTo = (dst_row * width + dst_col) * channel;
					if (channel == 4 && src_data[baseFrom + 3] == 0) {
						continue;
					}
					for (size_t iCanel = 0; iCanel < channel; ++iCanel)
						dst_data[baseTo + iCanel] = src_data[baseFrom + iCanel];

					if (use_premultiplied_alpha)
					{
						float alpha = src_data[baseFrom + 3] / 255.0f;
						for (size_t iCanel = 0; iCanel < channel-1; ++iCanel)
							dst_data[baseTo + iCanel] = dst_data[baseTo + iCanel] * alpha;
					}
				}
			}
		}

		delete[] src_data;
	}

	wxString imgFile(filename);
	imgFile = d2d::FilenameTools::getFilePathExceptExtension(imgFile);

	switch (type)
	{
	case e_bmp:
//		stbi_write_bmp((imgFile + ".bmp").c_str(), width, height, channel, dst_data);
		break;
	case e_jpg:
		{
			d2d::LibJpeg::ImageData data;
			data.width = width;
			data.height = height;
			data.pixels = dst_data;

			d2d::LibJpeg::write_JPEG_file((imgFile + ".jpg").c_str(), 80, data);
		}
		break;
	case e_png:
//		stbi_write_png((imgFile + ".png").c_str(), width, height, channel, dst_data, 0);
		d2d::ImageSaver::storeToFile(dst_data, width, height, 4, imgFile.ToStdString(), 
			d2d::ImageSaver::e_png);
		break;
	}

	free((void*)dst_data);
}

void FileIO::storeEasypackerPosition(const char* filename)
{
	Json::Value value;

	value["width"] = Context::Instance()->width;
	value["height"] = Context::Instance()->height;

	std::vector<d2d::ISprite*> sprites;
	Context::Instance()->stage->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		value["image"][i] = store(sprites[i]);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void FileIO::storeTexpackerPosition(const char* filename)
{
	Json::Value value;

	value["meta"] = Context::Instance()->tp_meta;

	std::vector<d2d::ISprite*> sprites;
	Context::Instance()->stage->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i) {
		d2d::ISprite* sprite = sprites[i];
		if (sprite->GetUserData()) 
		{
			Json::Value* val = static_cast<Json::Value*>(sprite->GetUserData());
			value["frames"][i] = *val;
		} 
		else 
		{
			d2d::Image* img = static_cast<const d2d::ImageSymbol&>(sprite->getSymbol()).getImage();
			Json::Value val;
			val["filename"] = d2d::FilenameTools::getFilenameWithExtension(img->GetFilepath()).ToStdString();
			val["rotated"] = sprite->getAngle() == 0 ? false : true;
			val["trimmed"] = true;
			val["sourceSize"]["w"] = img->GetOriginWidth();
			val["sourceSize"]["h"] = img->GetOriginHeight();

			d2d::Rect r = img->getRegion();
			val["frame"]["w"] = val["spriteSourceSize"]["w"] = r.xLength();
			val["frame"]["h"] = val["spriteSourceSize"]["h"] = r.yLength();
			val["spriteSourceSize"]["x"] = r.xMin + 0.5f * img->GetOriginWidth();
			val["spriteSourceSize"]["y"] = img->GetOriginHeight() - (r.yMax + 0.5f * img->GetOriginHeight());
			
			const d2d::Vector& pos = sprite->getPosition();
			val["frame"]["x"] = pos.x + r.xMin;
			val["frame"]["y"] = Context::Instance()->height - (pos.y + r.yMax);

			value["frames"][i] = val;
		}
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

Json::Value FileIO::store(const d2d::ISprite* sprite)
{
	Json::Value value;

	const d2d::ISymbol& symbol = sprite->getSymbol();
	const float w = symbol.GetSize().xLength(),
		h = symbol.GetSize().yLength();
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

	value["filepath"] = symbol.GetFilepath().ToStdString();
	value["left"] = left;
	value["low"] = low;
	value["width"] = width;
	value["height"] = height;
	value["rotate"] = bRotate;

	return value;
}