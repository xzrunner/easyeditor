#include "AtomicFactory.h"
#include "AtomicType.h"
#include "TranslateSpriteAOP.h"
#include "RotateSpriteAOP.h"

#include <float.h>

namespace ee
{

AtomicOP* AtomicFactory::Create(const Json::Value& val,
								const std::vector<Sprite*>& sprites)
{
	AtomicType type = AtomicType(val["type"].asInt());

	AtomicOP* ret = NULL;

	switch (type) 
	{
		case AT_MOVE:
			{
				float dx = static_cast<float>(val["dx"].asDouble()),
					  dy = static_cast<float>(val["dy"].asDouble());
				ret = new TranslateSpriteAOP(sprites, sm::vec2(dx, dy));
			}
			break;
		case AT_ROTATE:
			{
				sm::vec2 start, end;
				start.x = static_cast<float>(val["xstart"].asDouble());
				start.y = static_cast<float>(val["ystart"].asDouble());
				end.x = static_cast<float>(val["xend"].asDouble());
				end.y = static_cast<float>(val["yend"].asDouble());
				bool valid = start.x != FLT_MAX && start.y != FLT_MAX && end.x != FLT_MAX && end.y != FLT_MAX;

				float angle = static_cast<float>(val["angle"].asDouble());

				if (valid) {
					ret = new RotateSpriteAOP(sprites, start, end);
				} else {
					ret = new RotateSpriteAOP(sprites, angle);
				}
			}
			break;
		case AT_DELETE:
			{
				// todo:
			}
			break;
		case AT_SCALE:
			{
				float xscale = static_cast<float>(val["xscale"].asDouble());
				float yscale = static_cast<float>(val["yscale"].asDouble());

				std::vector<sm::vec2> oldScales;
				int i = 0;
				Json::Value pair_val = val["old"][i++];
				while (!pair_val.isNull()) {
					float x = static_cast<float>(pair_val["x"].asDouble());
					float y = static_cast<float>(pair_val["y"].asDouble());
					oldScales.push_back(sm::vec2(x, y));
					pair_val = val["old"][i++];
				}

//				ret = new arrange_sprite::ScaleSpritesAOP(sprites, sm::vec2(xscale, yscale), oldScales);
			}
			break;
		case AT_SHEAR:
			{
				float xshear = static_cast<float>(val["xshear"].asDouble());
				float yshear = static_cast<float>(val["yshear"].asDouble());

				std::vector<sm::vec2> oldShears;
				int i = 0;
				Json::Value pair_val = val["old"][i++];
				while (!pair_val.isNull()) {
					float x = static_cast<float>(pair_val["x"].asDouble());
					float y = static_cast<float>(pair_val["y"].asDouble());
					oldShears.push_back(sm::vec2(x, y));
					pair_val = val["old"][i++];
				}

//				ret = new arrange_sprite::ShearSpritesAOP(sprites, sm::vec2(xshear, yshear), oldShears);
			}
			break;
		case AT_MIRROR:
			{
				float xscale = static_cast<float>(val["xmirror"].asDouble());
				float yscale = static_cast<float>(val["ymirror"].asDouble());

				std::vector<std::pair<bool, bool> > oldMirrors;
				int i = 0;
				Json::Value pair_val = val["old"][i++];
				while (!pair_val.isNull()) {
					bool x = pair_val["x"].asBool();
					bool y = pair_val["y"].asBool();
					oldMirrors.push_back(std::make_pair(x, y));
					pair_val = val["old"][i++];
				}

//				ret = new arrange_sprite::MirrorSpritesAOP(sprites, xscale, yscale, oldMirrors);
			}
			break;
	}

	return ret;
}

}