#include "Love2dCode.h"

#include "Context.h"

#include "dataset/Layer.h"
#include "dataset/KeyFrame.h"

namespace eanim
{
namespace lua = ebuilder::lua;

Love2dCode::Love2dCode(ebuilder::CodeGenerator& gen)
	: m_gen(gen)
{
	m_packerAdapter.load(Context::Instance()->packer);
}

void Love2dCode::resolve()
{
	lua::TableAssign ta(m_gen, "frames");

	if (Context::Instance()->layers.size() > 0)
	{
		Layer* layer = Context::Instance()->layers.getLayer(0);
		const std::map<int, KeyFrame*>& frames = layer->getAllFrames();
		std::map<int, KeyFrame*>::const_iterator itr;
		for (itr = frames.begin(); itr != frames.end(); ++itr)
		{
			KeyFrame* frame = itr->second;			
			const std::vector<d2d::ISprite*>& sprites = frame->getAllSprites();
			if (!sprites.empty())
			{
				d2d::ISprite* sprite = sprites[0];
				int index = query(sprite->getSymbol().getFilepath().ToStdString());
				if (index != -1)
				{
					std::string time = wxString::FromDouble(frame->getTime(), 0);

					std::string x = wxString::FromDouble(sprite->getPosition().x, 1),
						y = wxString::FromDouble(sprite->getPosition().y, 1);

					std::string left = wxString::FromDouble(m_packerAdapter.textures[index].region.left, 1),
						low = wxString::FromDouble(m_packerAdapter.textures[index].region.low, 1),
						width = wxString::FromDouble(m_packerAdapter.textures[index].region.width, 1),
						height = wxString::FromDouble(m_packerAdapter.textures[index].region.height, 1);

					lua::call(m_gen, "", "Frame", 7, time.c_str(), x.c_str(), y.c_str(), left.c_str(), low.c_str(), width.c_str(), height.c_str());
				}
			}
		}
	}
}

int Love2dCode::query(const std::string& filepath) const
{
	for (size_t i = 0, n = m_packerAdapter.textures.size(); i < n; ++i)
	{
		if (m_packerAdapter.textures[i].filepath == filepath)
			return i;
	}
	for (size_t i = 0, n = m_packerAdapter.textures.size(); i < n; ++i)
	{
		std::string filename = wxFileName(filepath).GetName();
		if (filepath.find(filename) != std::string::npos)
			return i;
	}
	return -1;
}
} // eanim