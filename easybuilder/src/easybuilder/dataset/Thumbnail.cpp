
#include "Thumbnail.h"

#include "dataset/Game.h"
#include "dataset/Actor.h"

using namespace ebuilder;

const float Thumbnail::SCALE = 0.15f;

void Thumbnail::draw(const std::vector<Actor*>& actors, wxMemoryDC& memDC)
{
	for (size_t i = 0, n = actors.size(); i < n; ++i)
	{
		Actor* actor = actors[i];
		const wxBitmap* bitmap = actor->getSymbol().getBitmap()->getBitmap();

		const float width = std::max(1.0f, actor->getSymbol().getWidth() * SCALE),
			height = std::max(1.0f, actor->getSymbol().getHeight() * SCALE);
		const float hWidth = width * 0.5f,
			hHeight = height * 0.5f;

		float xMin, yMin;
		xMin = yMin = FLT_MAX;

		ee::Vector p = ee::Math::rotateVector(ee::Vector(-hWidth, -hHeight), actor->getAngle());
		if (p.x < xMin) xMin = p.x;
		if (p.y < yMin) yMin = p.y;

		p = ee::Math::rotateVector(ee::Vector(-hWidth, hHeight), actor->getAngle());
		if (p.x < xMin) xMin = p.x;
		if (p.y < yMin) yMin = p.y;

		p = ee::Math::rotateVector(ee::Vector(hWidth, -hHeight), actor->getAngle());
		if (p.x < xMin) xMin = p.x;
		if (p.y < yMin) yMin = p.y;

		p = ee::Math::rotateVector(ee::Vector(hWidth, hHeight), actor->getAngle());
		if (p.x < xMin) xMin = p.x;
		if (p.y < yMin) yMin = p.y;

		wxPoint pos;
		pos.x = Game::WIDTH * 0.5f * SCALE + actor->getPosition().x * SCALE + xMin;
		pos.y = Game::HEIGHT * 0.5f * SCALE - actor->getPosition().y * SCALE + yMin;

		memDC.DrawBitmap(
			bitmap->ConvertToImage().Scale(width, height).Rotate(actor->getAngle(), wxPoint(0, 0)),
			pos,
			true);
	}
}