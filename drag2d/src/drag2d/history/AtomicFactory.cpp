#include "AtomicFactory.h"
#include "AtomicType.h"
#include "ArrangeSpriteAtomicOP.h"

namespace d2d
{

AbstractAtomicOP* AtomicFactory::create(const Json::Value& val,
										const std::vector<ISprite*>& sprites)
{
	AtomicType type = AtomicType(val["type"].asInt());

	AbstractAtomicOP* ret = NULL;

	switch (type) 
	{
		case AT_MOVE:
			{
				float dx = val["dx"].asDouble(),
					dy = val["dy"].asDouble();
				ret = new arrange_sprite::MoveSpritesAOP(sprites, Vector(dx, dy));
			}
			break;
		case AT_ROTATE:
			{
				Vector start, end;
				start.x = val["xstart"].asDouble();
				start.y = val["ystart"].asDouble();
				end.x = val["xend"].asDouble();
				end.y = val["yend"].asDouble();

				float angle = val["angle"].asDouble();

				if (start.isValid() && end.isValid()) {
					ret = new arrange_sprite::RotateSpritesAOP(sprites, start, end);
				} else {
					ret = new arrange_sprite::RotateSpritesAOP(sprites, angle);
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
				float xscale = val["xscale"].asDouble();
				float yscale = val["yscale"].asDouble();

				std::vector<Vector> oldScales;
				int i = 0;
				Json::Value pairVal = val["old"][i++];
				while (!pairVal.isNull()) {
					float x = pairVal["x"].asDouble();
					float y = pairVal["y"].asDouble();
					oldScales.push_back(Vector(x, y));
					pairVal = val["old"][i++];
				}

				ret = new arrange_sprite::ScaleSpritesAOP(sprites, Vector(xscale, yscale), oldScales);
			}
			break;
		case AT_SHEAR:
			{
				float xshear = val["xshear"].asDouble();
				float yshear = val["yshear"].asDouble();

				std::vector<Vector> oldShears;
				int i = 0;
				Json::Value pairVal = val["old"][i++];
				while (!pairVal.isNull()) {
					float x = pairVal["x"].asDouble();
					float y = pairVal["y"].asDouble();
					oldShears.push_back(Vector(x, y));
					pairVal = val["old"][i++];
				}

				ret = new arrange_sprite::ShearSpritesAOP(sprites, Vector(xshear, yshear), oldShears);
			}
			break;
		case AT_MIRROR:
			{
				float xscale = val["xmirror"].asDouble();
				float yscale = val["ymirror"].asDouble();

				std::vector<std::pair<bool, bool> > oldMirrors;
				int i = 0;
				Json::Value pairVal = val["old"][i++];
				while (!pairVal.isNull()) {
					bool x = pairVal["x"].asBool();
					bool y = pairVal["y"].asBool();
					oldMirrors.push_back(std::make_pair(x, y));
					pairVal = val["old"][i++];
				}

				ret = new arrange_sprite::MirrorSpritesAOP(sprites, xscale, yscale, oldMirrors);
			}
			break;
	}

	return ret;
}

}