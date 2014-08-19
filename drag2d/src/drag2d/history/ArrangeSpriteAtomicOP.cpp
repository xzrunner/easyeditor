//#include "ArrangeSpriteAtomicOP.h"
//#include "AtomicType.h"
//
//#include "common/Math.h"
//#include "common/FileNameTools.h"
//#include "dataset/ISymbol.h"
//#include "view/MultiSpritesImpl.h"
//
//namespace d2d
//{
//namespace arrange_sprite
//{
////////////////////////////////////////////////////////////////////////////
//// class SpritesAOP
////////////////////////////////////////////////////////////////////////////
//
//SpritesAOP::SpritesAOP(const SpriteSelection& selection)
//{
//	selection.traverse(FetchAllVisitor<ISprite>(m_sprites));
//	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
//		m_sprites[i]->retain();
//}
//
//SpritesAOP::SpritesAOP(const std::vector<ISprite*>& sprites)
//{
//	for (size_t i = 0, n = sprites.size(); i < n; ++i)
//	{
//		sprites[i]->retain();
//		m_sprites.push_back(sprites[i]);
//	}
//}
//
//SpritesAOP::~SpritesAOP()
//{
//	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
//		m_sprites[i]->release();
//}
//
//Json::Value SpritesAOP::store(const std::vector<ISprite*>& sprites)
//{
//	Json::Value ret;
//	int count = 0;
//	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
//	{
//		int ptr = -1;
//		for (size_t j = 0, m = sprites.size(); j < m; ++j) {
//			if (sprites[j] == m_sprites[i]) {
//				ptr = j;
//				break;
//			}
//		}
//		if (ptr != -1) {
//			ret["sprites"][count++] = ptr;
//		}
//	}
//
//	if (count != 0)
//		ret["val"] = storeValues();
//
//	return ret;
//}
//
////////////////////////////////////////////////////////////////////////////
//// class MoveSpritesAOP
////////////////////////////////////////////////////////////////////////////
//
//MoveSpritesAOP::MoveSpritesAOP(const SpriteSelection& selection, const Vector& offset)
//	: SpritesAOP(selection)
//{
//	m_offset = offset;
//}
//
//MoveSpritesAOP::MoveSpritesAOP(const std::vector<ISprite*>& sprites, const Vector& offset)
//	: SpritesAOP(sprites)
//{
//	m_offset = offset;
//}
//
//void MoveSpritesAOP::undo()
//{
//	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
//	{
//		ISprite* sprite = m_sprites[i];
//		sprite->translate(-m_offset);
//	}
//}
//
//void MoveSpritesAOP::redo()
//{
//	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
//	{
//		ISprite* sprite = m_sprites[i];
//		sprite->translate(m_offset);
//	}
//}
//
//Json::Value MoveSpritesAOP::storeValues()
//{
//	Json::Value val;
//	val["type"] = AT_MOVE;
//	val["dx"] = m_offset.x;
//	val["dy"] = m_offset.y;
//	return val;
//}
//
////////////////////////////////////////////////////////////////////////////
//// class RotateSpritesAOP
////////////////////////////////////////////////////////////////////////////
//
//RotateSpritesAOP::RotateSpritesAOP(const SpriteSelection& selection, const Vector& start, const Vector& end)
//	: SpritesAOP(selection)
//	, m_start(start)
//	, m_end(end)
//	, m_angle(0)
//{
//}
//
//RotateSpritesAOP::RotateSpritesAOP(const std::vector<ISprite*>& sprites, const Vector& start, const Vector& end)
//	: SpritesAOP(sprites)
//	, m_start(start)
//	, m_end(end)
//	, m_angle(0)
//{
//}
//
//RotateSpritesAOP::RotateSpritesAOP(const std::vector<ISprite*>& sprites, float angle)
//	: SpritesAOP(sprites)
//	, m_angle(angle)
//{
//	m_start.setInvalid();
//	m_end.setInvalid();
//}
//
//void RotateSpritesAOP::undo()
//{
//	if (m_start.isValid() && m_end.isValid()) {
//		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
//		{
//			ISprite* sprite = m_sprites[i];
//			float angle = Math::getAngleInDirection(sprite->getPosition(), m_start, m_end);
//			sprite->rotate(-angle);
//		}
//	} else if (m_angle != 0) {
//		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
//		{
//			ISprite* sprite = m_sprites[i];
//			sprite->rotate(-m_angle);
//		}
//	}
//}
//
//void RotateSpritesAOP::redo()
//{
//	if (m_start.isValid() && m_end.isValid()) {
//		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
//		{
//			ISprite* sprite = m_sprites[i];
//			float angle = Math::getAngleInDirection(sprite->getPosition(), m_start, m_end);
//			sprite->rotate(angle);
//		}
//	} else if (m_angle != 0) {
//		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
//		{
//			ISprite* sprite = m_sprites[i];
//			sprite->rotate(m_angle);
//		}
//	}
//}
//
//Json::Value RotateSpritesAOP::storeValues()
//{
//	Json::Value val;
//	val["type"] = AT_ROTATE;
//	val["xstart"] = m_start.x;
//	val["ystart"] = m_start.y;
//	val["xend"] = m_end.x;
//	val["yend"] = m_end.y;
//	val["angle"] = m_angle;
//	return val;
//}
//
////////////////////////////////////////////////////////////////////////////
//// class DeleteSpritesAOP
////////////////////////////////////////////////////////////////////////////
//
//DeleteSpritesAOP::DeleteSpritesAOP(const SpriteSelection& selection, MultiSpritesImpl* spritesImpl)
//	: SpritesAOP(selection)
//{
//	m_spritesImpl = spritesImpl;
//}
//
//DeleteSpritesAOP::DeleteSpritesAOP(const std::vector<ISprite*>& sprites, MultiSpritesImpl* spritesImpl)
//	: SpritesAOP(sprites)
//{
//	m_spritesImpl = spritesImpl;
//}
//
//void DeleteSpritesAOP::undo()
//{
//	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
//		m_spritesImpl->insertSprite(m_sprites[i]);
//}
//
//void DeleteSpritesAOP::redo()
//{
//	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
//		m_spritesImpl->removeSprite(m_sprites[i]);
//}
//
//Json::Value DeleteSpritesAOP::storeValues()
//{
//	Json::Value val;
//	val["type"] = AT_DELETE;
//	return val;
//}
//
////////////////////////////////////////////////////////////////////////////
//// class ScaleSpritesAOP
////////////////////////////////////////////////////////////////////////////
//
//ScaleSpritesAOP::ScaleSpritesAOP(const std::vector<ISprite*>& sprites, 
//								 const Vector& new_scale, 
//								 const Vector& old_scale)
//	: SpritesAOP(sprites)
//	, m_scale(new_scale)
//{
//	m_oldScales.reserve(sprites.size());
//	for (size_t i = 0, n = sprites.size(); i < n; ++i) {
//		ISprite* sprite = sprites[i];
//		m_oldScales.push_back(old_scale);
//	}
//}
//
//ScaleSpritesAOP::ScaleSpritesAOP(const std::vector<ISprite*>& sprites, const Vector& new_scale,
//				const std::vector<Vector>& oldScales)
//	: SpritesAOP(sprites)
//	, m_scale(new_scale)
//	, m_oldScales(oldScales)
//{
//}
//
//void ScaleSpritesAOP::undo()
//{
//	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
//		ISprite* sprite = m_sprites[i];
//		sprite->setScale(m_oldScales[i].x, m_oldScales[i].y);
//	} 
//}
//
//void ScaleSpritesAOP::redo()
//{
//	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
//		ISprite* sprite = m_sprites[i];
//		sprite->setScale(m_scale.x, m_scale.y);
//	} 
//}
//
//Json::Value ScaleSpritesAOP::storeValues()
//{
//	Json::Value val;
//	val["type"] = AT_SCALE;
//	val["xscale"] = m_scale.x;
//	val["yscale"] = m_scale.y;
//	for (size_t i = 0, n = m_oldScales.size(); i < n; ++i) {
//		val["old"][i]["x"] = m_oldScales[i].x;
//		val["old"][i]["y"] = m_oldScales[i].y;
//	}
//	return val;
//}
//
////////////////////////////////////////////////////////////////////////////
//// class ShearSpritesAOP
////////////////////////////////////////////////////////////////////////////
//
//ShearSpritesAOP::ShearSpritesAOP(const std::vector<ISprite*>& sprites, const Vector& new_shear, 
//								 const Vector& old_shear)
//	: SpritesAOP(sprites)
//	, m_shear(new_shear)
//{
//	m_oldShears.reserve(sprites.size());
//	for (size_t i = 0, n = sprites.size(); i < n; ++i) {
//		ISprite* sprite = sprites[i];
//		m_oldShears.push_back(old_shear);
//	}
//}
//
//ShearSpritesAOP::ShearSpritesAOP(const std::vector<ISprite*>& sprites, const Vector& new_shear,
//								 const std::vector<Vector>& oldShears)
//	: SpritesAOP(sprites)
//	, m_shear(new_shear)
//	, m_oldShears(oldShears)
//{
//}
//
//void ShearSpritesAOP::undo()
//{
//	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
//		ISprite* sprite = m_sprites[i];
//		sprite->setShear(m_oldShears[i].x, m_oldShears[i].y);
//	} 
//}
//
//void ShearSpritesAOP::redo()
//{
//	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
//		ISprite* sprite = m_sprites[i];
//		sprite->setShear(m_shear.x, m_shear.y);
//	} 
//}
//
//Json::Value ShearSpritesAOP::storeValues()
//{
//	Json::Value val;
//	val["type"] = AT_SHEAR;
//	val["xshear"] = m_shear.x;
//	val["yshear"] = m_shear.y;
//	for (size_t i = 0, n = m_oldShears.size(); i < n; ++i) {
//		val["old"][i]["x"] = m_oldShears[i].x;
//		val["old"][i]["y"] = m_oldShears[i].y;
//	}
//	return val;
//}
//
////////////////////////////////////////////////////////////////////////////
//// class MirrorSpritesAOP
////////////////////////////////////////////////////////////////////////////
//
//MirrorSpritesAOP::MirrorSpritesAOP(const std::vector<ISprite*>& sprites, bool xMirror, bool yMirror)
//	: SpritesAOP(sprites)
//	, m_xMirror(xMirror)
//	, m_yMirror(yMirror)
//{
//	m_oldMirrors.reserve(sprites.size());
//	for (size_t i = 0, n = sprites.size(); i < n; ++i) {
//		ISprite* sprite = sprites[i];
//		bool mx, my;
//		sprite->getMirror(mx, my);
//		m_oldMirrors.push_back(std::make_pair(mx, my));
//	}
//}
//
//MirrorSpritesAOP::MirrorSpritesAOP(const std::vector<ISprite*>& sprites, bool xMirror, bool yMirror,
//								   const std::vector<std::pair<bool, bool> >& oldMirrors)
//	: SpritesAOP(sprites)
//	, m_xMirror(xMirror)
//	, m_yMirror(yMirror)
//	, m_oldMirrors(oldMirrors)
//{
//}
//
//void MirrorSpritesAOP::undo()
//{
//	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
//		ISprite* sprite = m_sprites[i];
//		sprite->setMirror(m_oldMirrors[i].first, m_oldMirrors[i].second);
//	} 
//}
//
//void MirrorSpritesAOP::redo()
//{
//	for (size_t i = 0, n = m_sprites.size(); i < n; ++i) {
//		ISprite* sprite = m_sprites[i];
//		sprite->setMirror(m_xMirror, m_yMirror);
//	} 
//}
//
//Json::Value MirrorSpritesAOP::storeValues()
//{
//	Json::Value val;
//	val["type"] = AT_SHEAR;
//	val["xmirror"] = m_xMirror;
//	val["ymirror"] = m_yMirror;
//	for (size_t i = 0, n = m_oldMirrors.size(); i < n; ++i) {
//		val["old"][i]["x"] = m_oldMirrors[i].first;
//		val["old"][i]["y"] = m_oldMirrors[i].second;
//	}
//	return val;
//}
//
//} // arrange_sprite
//} // d2d