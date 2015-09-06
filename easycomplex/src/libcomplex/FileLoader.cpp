#include "FileLoader.h"
#include "Symbol.h"
#include "Sprite.h"

#include <easyicon.h>
#include <easyanim.h>

namespace ecomplex
{

//void Symbol::loadResources()
//{
//	bool use_dtex = d2d::Config::Instance()->IsUseDTex();
//	d2d::DynamicTexAndFont* dtex = NULL;
//	if (use_dtex) {
//		dtex = d2d::DynamicTexAndFont::Instance();
//		dtex->BeginImage();
//	}
//
//	clear();
//
//	Json::Value value;
//	Json::Reader reader;
//	std::locale::global(std::locale(""));
//	std::ifstream fin(m_filepath.fn_str());
//	std::locale::global(std::locale("C"));
//	reader.parse(fin, value);
//	fin.close();
//
//	name = value["name"].asString();
//
//	m_clipbox.xMin = value["xmin"].asInt();
//	m_clipbox.xMax = value["xmax"].asInt();
//	m_clipbox.yMin = value["ymin"].asInt();
//	m_clipbox.yMax = value["ymax"].asInt();
//
//	m_use_render_cache = value["use_render_cache"].asBool();
//
// 	wxString dir = d2d::FilenameTools::getFileDir(m_filepath);
//	int i = 0;
//	Json::Value spriteValue = value["sprite"][i++];
//	while (!spriteValue.isNull()) {
//		wxString path = d2d::FilenameTools::getAbsolutePath(dir, spriteValue["filepath"].asString());
//		ISymbol* symbol = NULL;
//		std::string real_filepath = path;
//		try {
//			symbol = d2d::SymbolMgr::Instance()->fetchSymbol(path);
//		} catch (d2d::Exception& e) {
// 			symbol = NULL;
// 			Json::Value filepaths_val = spriteValue["filepaths"];
// 			if (!filepaths_val.isNull()) 
// 			{
// 				int j = 0;
// 				Json::Value filepath_val = filepaths_val[j++];
// 				while (!filepath_val.isNull() && !symbol) {
//					real_filepath = filepath_val.asString();
// 					wxString filepath = d2d::FilenameTools::getAbsolutePath(dir, real_filepath);
//					filepath_val = filepaths_val[j++];
// 					try {
// 						symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
// 					} catch (d2d::Exception& e) {
// 						symbol = NULL;
// 					}
// 				}
// 			}
//
//			if (!symbol) {
//				throw e;
//			}
//		}
//
// 		// load symbol filepaths
// 		Json::Value filepaths_val = spriteValue["filepaths"];
// 		if (!filepaths_val.isNull()) {
// 			std::vector<std::string> filepaths;
// 			int i = 0;
// 			Json::Value filepath_val = filepaths_val[i++];
// 			while (!filepath_val.isNull()) {
//				std::string p = filepath_val.asString();
//				if (p != real_filepath) {
//					filepaths.push_back(p);
//				}
// 				filepath_val = filepaths_val[i++];
// 			}
//			filepaths.push_back(spriteValue["filepath"].asString());
// 			symbol->SetFilepaths(filepaths);
// 		}
//
////		symbol->refresh();
//		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
//		sprite->load(spriteValue);
//
//		symbol->Release();
//
//		m_sprites.push_back(sprite);
//		spriteValue = value["sprite"][i++];
//	}	
//
//	initBounding();
//
//	if (use_dtex) {
//		dtex->EndImage();
//		if (m_use_render_cache) {
//			dtex->InsertSymbol(*this);
//		}
//	}
//}

void FileLoader::Load(const std::string& filepath, Symbol* complex)
{
	bool use_dtex = d2d::Config::Instance()->IsUseDTex();
	d2d::DynamicTexAndFont* dtex = NULL;
	if (use_dtex) {
		dtex = d2d::DynamicTexAndFont::Instance();
		dtex->BeginImage();
	}

	complex->Clear();

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = d2d::FilenameTools::getFileDir(filepath);
	if (value["lua desc"].isNull()) {
		LoadJson(value, dir, complex);
	} else {
		LoadLua(value, dir, complex);
	}

	complex->InitBounding();

	if (use_dtex) {
		dtex->EndImage();
		if (complex->m_use_render_cache) {
			dtex->InsertSymbol(*complex);
		}
	}
}

void FileLoader::LoadJson(const Json::Value& value, const std::string& dir, Symbol* complex)
{
	complex->name = value["name"].asString();

	complex->m_clipbox.xMin = value["xmin"].asInt();
	complex->m_clipbox.xMax = value["xmax"].asInt();
	complex->m_clipbox.yMin = value["ymin"].asInt();
	complex->m_clipbox.yMax = value["ymax"].asInt();

	complex->m_use_render_cache = value["use_render_cache"].asBool();

	int i = 0;
	Json::Value spriteValue = value["sprite"][i++];
	while (!spriteValue.isNull()) {
		std::string filepath = d2d::SymbolSearcher::GetSymbolPath(dir, spriteValue);
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
		if (!symbol) {
			std::string filepath = spriteValue["filepath"].asString();
			throw d2d::Exception("Symbol doesn't exist, [dir]:%s, [file]:%s !", dir.c_str(), filepath.c_str());
		}
		d2d::SymbolSearcher::SetSymbolFilepaths(dir, symbol, spriteValue);

		//		symbol->refresh();
		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
		sprite->Load(spriteValue);

		symbol->Release();

		static_cast<Symbol*>(complex)->m_sprites.push_back(sprite);
		spriteValue = value["sprite"][i++];
	}	
}

void FileLoader::LoadLua(const Json::Value& value, const std::string& dir, Symbol* symbol)
{
	std::vector<d2d::Image*> images;
	std::string img_name = d2d::FilenameTools::getAbsolutePath(dir, value["image name"].asString());
	LoadImages(img_name, images);

	std::string lua_file = d2d::FilenameTools::getAbsolutePath(dir, value["lua desc"].asString());
	libcoco::CocoUnpacker unpacker(lua_file, images);

	std::string export_name = value["export name"].asString();
	
	libcoco::IPackNode* node = libcoco::UnpackNodeFactory::Instance()->Query(export_name);
	LoadFromNode(node, symbol);	
}

void FileLoader::LoadImages(const std::string& name, std::vector<d2d::Image*>& images)
{
	d2d::SettingData& data = d2d::Config::Instance()->GetSettings();
	bool old_open_image_edge_clip = data.open_image_edge_clip;
	data.open_image_edge_clip = false;
	bool old_pre_multi_alpha = data.pre_multi_alpha;
	data.pre_multi_alpha = false;

	int idx = 1;
	while (true)
	{
		std::string filepath = name + d2d::StringTools::ToString(idx++) + ".png";
		if (d2d::FilenameTools::IsFileExist(filepath)) {
			d2d::Image* img = d2d::ImageMgr::Instance()->GetItem(filepath);
			images.push_back(img);
		} else {	
			break;
		}
	}

	data.open_image_edge_clip = old_open_image_edge_clip;
	data.pre_multi_alpha = old_pre_multi_alpha;
}

void FileLoader::LoadFromNode(const libcoco::IPackNode* node, Symbol* symbol)
{
	symbol->m_sprites.push_back(Node2Sprite(node));
}

d2d::ISprite* FileLoader::Node2Sprite(const libcoco::IPackNode* node)
{
	if (const libcoco::PackPicture* pic = dynamic_cast<const libcoco::PackPicture*>(node)) {
		return Pic2Sprite(pic);
	} else if (const libcoco::PackLabel* label = dynamic_cast<const libcoco::PackLabel*>(node)) {
		return Label2Sprite(label);
	} else if (const libcoco::PackAnimation* anim = dynamic_cast<const libcoco::PackAnimation*>(node)) {
		return Anim2Sprite(anim);
	} else {
		throw d2d::Exception("FileLoader::Node2Sprite unknown type.");
	}
}

d2d::ISprite* FileLoader::Quad2Sprite(const libcoco::PackPicture::Quad* quad)
{
	float w = quad->img->GetOriginWidth(),
		h = quad->img->GetOriginHeight();

	d2d::Vector src[4], screen[4];
	for (int i = 0; i < 4; ++i) {
		src[i].x = quad->texture_coord[i].x / w;
		src[i].y = 1 - quad->texture_coord[i].y / h;
		screen[i] = quad->screen_coord[i];
	}

	eicon::QuadIcon* icon = new eicon::QuadIcon(
		const_cast<d2d::Image*>(quad->img), src, screen);

	eicon::Symbol* symbol = new eicon::Symbol;
	symbol->SetIcon(icon);
	d2d::ISprite* ret = new eicon::Sprite(symbol);
	ret->BuildBounding();
	return ret;
}

d2d::ISprite* FileLoader::Pic2Sprite(const libcoco::PackPicture* pic)
{
	if (pic->quads.size() == 1) {
		return Quad2Sprite(&pic->quads[0]);
	} else if (pic->quads.size() > 1) {
		ecomplex::Symbol* complex = new ecomplex::Symbol;
		for (int i = 0, n = pic->quads.size(); i < n; ++i) {
			complex->m_sprites.push_back(Quad2Sprite(&pic->quads[i]));
		}
		complex->InitBounding();
		return new Sprite(complex);
	} else {
		return NULL;
	}
}

d2d::ISprite* FileLoader::Label2Sprite(const libcoco::PackLabel* label)
{
	d2d::FontBlankSymbol* symbol = new d2d::FontBlankSymbol();

	d2d::FontSprite* spr = new d2d::FontSprite(symbol);
	spr->font = label->font;
	spr->has_edge = label->has_edge;
	spr->color = label->color;
	spr->align_hori = label->align_hori;
	spr->align_vert = label->align_vert;
	spr->size = label->size;
	spr->width = label->width;
	spr->height = label->height;
	return spr;
}

d2d::ISprite* FileLoader::Anim2Sprite(const libcoco::PackAnimation* anim)
{
	assert(!anim->actions.empty() && anim->actions[0].size >= 1);
	if (anim->actions[0].size == 1) {
		return Anim2ComplexSprite(anim);
	} else {
		return Anim2AnimSprite(anim);
	}
}

d2d::ISprite* FileLoader::Anim2ComplexSprite(const libcoco::PackAnimation* anim)
{
	assert(!anim->actions.empty() && anim->actions[0].size == 1);

	ecomplex::Symbol* complex = new ecomplex::Symbol;
	const libcoco::PackAnimation::Frame& src = anim->frames[0];
	ecomplex::Symbol* dst = new ecomplex::Symbol;
	for (int i = 0; i < src.parts.size(); ++i) {
		const libcoco::PackAnimation::Part& part = src.parts[i];
		d2d::ISprite* spr = Node2Sprite(anim->components[part.comp_idx].node);
		TransSprite(spr, part.t);
		dst->m_sprites.push_back(spr);
	}
	dst->InitBounding();
	complex->m_sprites.push_back(new Sprite(dst));
	complex->InitBounding();
	return new Sprite(complex);
}

d2d::ISprite* FileLoader::Anim2AnimSprite(const libcoco::PackAnimation* anim)
{
	assert(!anim->actions.empty() && anim->actions[0].size >= 1);

	libanim::Symbol* anim_symbol = new libanim::Symbol;
	libanim::Symbol::Layer* layer = new libanim::Symbol::Layer;
	for (int i = 0; i < anim->actions[0].size; ++i) {
		const libcoco::PackAnimation::Frame& src = anim->frames[i];
		libanim::Symbol::Frame* frame = new libanim::Symbol::Frame;
		frame->index = i;
		frame->bClassicTween = false;
		for (int j = 0; j < src.parts.size(); ++j) {
			const libcoco::PackAnimation::Part& part = src.parts[j];
			d2d::ISprite* spr = Node2Sprite(anim->components[part.comp_idx].node);
			TransSprite(spr, part.t);
			frame->sprites.push_back(spr);
		}
		layer->frames.push_back(frame);
	}
	anim_symbol->setFPS(30);
	anim_symbol->m_layers.push_back(layer);
	anim_symbol->InitBounding();
	return new libanim::Sprite(anim_symbol);
}

void FileLoader::TransSprite(d2d::ISprite* spr, const libcoco::PackAnimation::SpriteTrans& t)
{
	if (!libcoco::PackAnimation::IsMatrixIdentity(t.mat)) {
		TransSpriteMat(spr, t);
	}
	TransSpriteCol(spr, t);
}

void FileLoader::TransSpriteMat(d2d::ISprite* spr, const libcoco::PackAnimation::SpriteTrans& t)
{
	float dx = t.mat[4] / 16.0f,
		dy = -t.mat[5] / 16.0f;

	// no shear
// 	mat[0] = sx*c;
// 	mat[1] = sx*s;
// 	mat[2] = -sy*s;
// 	mat[3] = sy*c;	

	float angle = atan2((float)t.mat[1], (float)t.mat[0]);
// #ifdef _DEBUG
// 	if (t.mat[2] != 0) {
// 		assert(atan2(-(float)t.mat[2], (float)t.mat[3]) == angle);
// 	}
// #endif
	float c = cos(angle), s = sin(angle);
	float sx, sy;
	if (c != 0) {
		sx = t.mat[0] / c / 1024.0f;
		sy = t.mat[3] / c / 1024.0f;
	} else {
		sx = t.mat[1] / s / 1024.0f;
		sy = -t.mat[2] / s / 1024.0f;
	}

	if (fabs(t.mat[0] - sx * c * 1024) >= 1 ||
		fabs(t.mat[1] - sx * s * 1024) >= 1 ||
		fabs(t.mat[2] - -sy * s * 1024) >= 1 ||
		fabs(t.mat[3] - sy * c * 1024) >= 1) {
		// todo use mat for sprite

		sx = sy = 1;
		angle = 0;
	}
	
	// no scale
// 	mat[0] = c - ky*s;
// 	mat[1] = s + ky*c;
// 	mat[2] = kx*c - s;
// 	mat[3] = kx*s + c;

	bool xmirror = false, ymirror = false;
	if (sx < 0) {
		xmirror = true;
		sx = -sx;
	}
	if (sy < 0) {
		ymirror = true;
		sy = -sy;
	}
	spr->SetMirror(xmirror, ymirror);

	spr->SetScale(sx, sy);
	spr->SetTransform(d2d::Vector(dx, dy), angle);
}

void FileLoader::TransSpriteCol(d2d::ISprite* spr, const libcoco::PackAnimation::SpriteTrans& t)
{
	spr->multiCol = d2d::transColor(t.color, d2d::PT_ARGB);
	spr->addCol = d2d::transColor(t.additive, d2d::PT_ARGB);

	spr->r_trans = d2d::transColor(t.rmap, d2d::PT_ARGB);
	spr->g_trans = d2d::transColor(t.gmap, d2d::PT_ARGB);
	spr->b_trans = d2d::transColor(t.bmap, d2d::PT_ARGB);
}

}