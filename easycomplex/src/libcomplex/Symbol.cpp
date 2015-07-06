#include "Symbol.h"
#include "Sprite.h"
#include "config.h"

#include <queue>

namespace ecomplex
{

const float Symbol::SCALE = 0.15f;

Symbol::Symbol()
	: m_use_render_cache(false)
	, m_render_version(0)
	, m_render_cache_open(true)
{
	static int id = 0;
	m_name = FILE_TAG + wxVariant(id++);

	m_clipbox.xMin = m_clipbox.xMax = m_clipbox.yMin = m_clipbox.yMax = 0;
	m_style.color.set(0, 0.8f, 0);
}

Symbol::~Symbol()
{
	clear();
}

void Symbol::ReloadTexture() const
{
	std::set<const ISymbol*> symbols;
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		symbols.insert(&m_sprites[i]->getSymbol());
	std::set<const ISymbol*>::iterator itr = symbols.begin();
	for ( ; itr != symbols.end(); ++itr)
		(*itr)->ReloadTexture();
}

void Symbol::Draw(const d2d::Matrix& mt,
				  const d2d::Colorf& mul, 
				  const d2d::Colorf& add,
				  const d2d::Colorf& r_trans,
				  const d2d::Colorf& g_trans,
				  const d2d::Colorf& b_trans,
				  const d2d::ISprite* sprite/* = NULL*/) const
{
 	const d2d::TPNode* n = NULL;
	if (d2d::Config::Instance()->IsUseDTex() && 
		m_render_cache_open) 
	{
		d2d::DynamicTexAndFont* dtex = d2d::DynamicTexAndFont::Instance();
		n = dtex->Query(m_filepath);
	}
 	if (n) 
 	{
		d2d::DynamicTexAndFont* dtex = d2d::DynamicTexAndFont::Instance();

		d2d::ShaderMgr* shader = d2d::ShaderMgr::Instance();
		if (shader->GetVersion() != m_render_version)
		{
			m_render_cache_open = false;
			dtex->RefreshSymbol(*this, *n);
			m_render_cache_open = true;

			m_render_version = shader->GetVersion();
		}

		d2d::Vector vertices[4];
		float hw = m_rect.xLength() * 0.5f,
			hh = m_rect.yLength() * 0.5f;
		vertices[0] = d2d::Math::transVector(d2d::Vector(m_rect.xMin, m_rect.yMin), mt);
		vertices[1] = d2d::Math::transVector(d2d::Vector(m_rect.xMax, m_rect.yMin), mt);
		vertices[2] = d2d::Math::transVector(d2d::Vector(m_rect.xMax, m_rect.yMax), mt);
		vertices[3] = d2d::Math::transVector(d2d::Vector(m_rect.xMin, m_rect.yMax), mt);
		if (n->IsRotated())
		{
			d2d::Vector tmp = vertices[3];
			vertices[3] = vertices[2];
			vertices[2] = vertices[1];
			vertices[1] = vertices[0];
			vertices[0] = tmp;
		}

		d2d::Vector texcoords[4];
		float txmin, txmax, tymin, tymax;
		float extend = dtex->GetExtend();
		int width = dtex->GetWidth();
		int height = dtex->GetHeight();
		int texid = dtex->GetTextureID();
		txmin = (n->GetMinX()+extend) / width;
		txmax = (n->GetMaxX()-extend) / width;
		tymin = (n->GetMinY()+extend) / height;
		tymax = (n->GetMaxY()-extend) / height;

		if (texid != 1) {
			wxLogDebug(_T("img dt's tex = %d"), texid);
		}
		texcoords[0].set(txmin, tymin);
		texcoords[1].set(txmax, tymin);
		texcoords[2].set(txmax, tymax);
		texcoords[3].set(txmin, tymax);

		shader->sprite();
		shader->Draw(vertices, texcoords, texid);
 	}
 	else
	{
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
			d2d::SpriteRenderer::Instance()->Draw(m_sprites[i], mt, mul, add, r_trans, g_trans, b_trans);
		}
		if (m_clipbox.xLength() > 0 && m_clipbox.yLength() > 0) {
			d2d::PrimitiveDraw::rect(mt, m_clipbox, m_style);
		}
	}
}

d2d::Rect Symbol::GetSize(const d2d::ISprite* sprite/* = NULL*/) const
{
	return m_rect;
}

bool Symbol::isOneLayer() const
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		if (dynamic_cast<Sprite*>(m_sprites[i]))
			return false;
	return true;
}

void Symbol::InitBounding()
{
	m_rect.makeInfinite();
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		std::vector<d2d::Vector> vertices;
		m_sprites[i]->getBounding()->getBoundPos(vertices);
		for (size_t j = 0, m = vertices.size(); j < m; ++j)
			m_rect.combine(vertices[j]);
	}

	// 为兼容老数据，临时去掉
	//// to center
	//float x = m_rect.xCenter(),
	//	y = m_rect.yCenter();
	//for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	//	m_sprites[i]->translate(d2d::Vector(-x, -y));
	//m_rect.translate(d2d::Vector(-x, -y));
}

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

void Symbol::LoadResources()
{
	bool use_dtex = d2d::Config::Instance()->IsUseDTex();
	d2d::DynamicTexAndFont* dtex = NULL;
	if (use_dtex) {
		dtex = d2d::DynamicTexAndFont::Instance();
		dtex->BeginImage();
	}

	clear();

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	name = value["name"].asString();

	m_clipbox.xMin = value["xmin"].asInt();
	m_clipbox.xMax = value["xmax"].asInt();
	m_clipbox.yMin = value["ymin"].asInt();
	m_clipbox.yMax = value["ymax"].asInt();

	m_use_render_cache = value["use_render_cache"].asBool();

	wxString dir = d2d::FilenameTools::getFileDir(m_filepath);
	int i = 0;
	Json::Value spriteValue = value["sprite"][i++];
	while (!spriteValue.isNull()) {
		std::string filepath = d2d::SymbolSearcher::GetSymbolPath(dir, spriteValue);
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
		if (!symbol) {
			std::string filepath = spriteValue["filepath"].asString();
			throw d2d::Exception("Symbol doesn't exist, [dir]:%s, [file]:%s !", dir.ToStdString().c_str(), filepath.c_str());
		}
		d2d::SymbolSearcher::SetSymbolFilepaths(dir, symbol, spriteValue);

		//		symbol->refresh();
		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
		sprite->load(spriteValue);

		symbol->Release();

		m_sprites.push_back(sprite);
		spriteValue = value["sprite"][i++];
	}	

	InitBounding();

	if (use_dtex) {
		dtex->EndImage();
		if (m_use_render_cache) {
			dtex->InsertSymbol(*this);
		}
	}
}

void Symbol::clear()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		m_sprites[i]->Release();
	m_sprites.clear();
}

}