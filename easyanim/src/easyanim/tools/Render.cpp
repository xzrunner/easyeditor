#include "Render.h"
#include "dataset/Mesh.h"
#include "dataset/PartSkeleton.h"
#include "dataset/WholeSkeleton.h"
#include "dataset/Sprite.h"
#include "dataset/Symbol.h"

#include <windows.h>
#include <GL/gl.h>

namespace eanim
{

void Render::drawPos(const ee::Vector& pos, float radius)
{
	glBegin(GL_QUADS);
		glVertex2f(pos.x - radius, pos.y - radius);
		glVertex2f(pos.x + radius, pos.y - radius);
		glVertex2f(pos.x + radius, pos.y + radius);
		glVertex2f(pos.x - radius, pos.y + radius);
	glEnd();
}

void Render::drawSprites(const std::vector<Sprite*>& sprites)
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		Sprite* sprite = sprites[i];
		const ee::Vector& pos = sprite->getPosition();

		glPushMatrix();
		glTranslatef(pos.x, pos.y, 0.0f);
		glRotatef(sprite->getAngle() * ee::TRANS_RAD_TO_DEG, 0, 0, 1);
			drawSpriteCenter(sprite);
		glPopMatrix();
	}
}

void Render::drawSpriteCenter(Sprite* sprite)
{
	if (sprite->getMesh())
		drawSpriteCenterMesh(sprite);
	else if (sprite->getPartSkeleton())
		drawSpriteCenterPartSkeleton(sprite);
	else if (sprite->getWholeSkeleton())
		drawSpriteCenterWholeSkeleton(sprite);
	else
		ee::SpriteDraw::drawSprite(sprite);
}

void Render::drawSpriteCenterMesh(Sprite* sprite)
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	Mesh* mesh = sprite->getMesh();
	const std::vector<MeshTri*>& tris = mesh->getAllTris();
	glBindTexture(GL_TEXTURE_2D, sprite->getSymbol()->getTextureID());
	for (size_t i = 0, n = tris.size(); i < n; ++i)
	{
		MeshTri* tri = tris[i];
		std::vector<MeshNode*> nodes;
		tri->getNodes(nodes);
		glBegin(GL_TRIANGLES);
		for (size_t j = 0; j < 3; ++j)
		{
			glTexCoord2f(nodes[j]->texCoords.x, nodes[j]->texCoords.y); 
			glVertex2f(nodes[j]->projCoords.x, nodes[j]->projCoords.y);
		}
		glEnd();
	}
	glBindTexture(GL_TEXTURE_2D, NULL);
}

void Render::drawSpriteCenterPartSkeleton(Sprite* sprite)
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glBindTexture(GL_TEXTURE_2D, sprite->getSymbol()->getTextureID());
	sprite->getPartSkeleton()->drawMeshes();
	glBindTexture(GL_TEXTURE_2D, NULL);
}

void Render::drawSpriteCenterWholeSkeleton(Sprite* sprite)
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	sprite->getWholeSkeleton()->onDraw();
}

void Render::drawRawPixels(const ee::RawPixels& pixels)
{
	int width, height, channels;
	pixels.getSize(width, height);
	channels = pixels.getChannels();
	assert(channels == 3 || channels == 4);

	glPushMatrix();
	glTranslatef(- width * 0.5f, - height * 0.5f, 0.0f);

	const unsigned char* data = pixels.getPixelData();
	int index = 0;
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			unsigned char color[4];
			color[3] = 255;
			for (int k = 0; k < channels; ++k)
				color[k] = data[index++];

			glColor4ub(color[0], color[1], color[2], color[3]);
			glBegin(GL_QUADS);
				glVertex2f(j - 0.5f, height - 1 - i - 0.5f);
				glVertex2f(j + 0.5f, height - 1 - i - 0.5f);
				glVertex2f(j + 0.5f, height - 1 - i + 0.5f);
				glVertex2f(j - 0.5f, height - 1 - i + 0.5f);
			glEnd();
		}
	}

	glPopMatrix();
}

void Render::drawRawPixelsBound(const ee::RawPixels& pixels)
{
	int width, height, channels;
	pixels.getSize(width, height);
	channels = pixels.getChannels();
	if (channels == 3) return;
	assert(channels == 4);

	glPushMatrix();
	glTranslatef(- width * 0.5f, - height * 0.5f, 0.0f);

	const unsigned char* data = pixels.getPixelData();
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			if (data[4 * (width * i + j) + 3] != 0)
			{
				glColor3f(1.0f, 0.0f, 0.0f);

				glBegin(GL_LINE_LOOP);
					glVertex2f(j - 0.5f, height - 1 - i - 0.5f);
					glVertex2f(j + 0.5f, height - 1 - i - 0.5f);
					glVertex2f(j + 0.5f, height - 1 - i + 0.5f);
					glVertex2f(j - 0.5f, height - 1 - i + 0.5f);
				glEnd();

// 				if (j == 0 || j > 0 && data[4 * (width * i + j - 1) + 3] == 0)
// 				{
// 					glBegin(GL_LINES);
// 						glVertex2f(j - 0.5f, height - i - 0.5f);
// 						glVertex2f(j - 0.5f, height - i + 0.5f);
// 					glEnd();
// 				}
// 				if (j < width)
			}
		}
	}

	glPopMatrix();
}

void Render::drawRawPixelsSelected(const ee::RawPixels& original, const ee::RawPixels::PixelBuf& selected)
{
	int width, height, channels;
	original.getSize(width, height);
	channels = original.getChannels();
	assert(channels == 3 || channels == 4);

	glPushMatrix();
	glTranslatef(- width * 0.5f, - height * 0.5f, 0.0f);

	const unsigned char* data = original.getPixelData();
	ee::RawPixels::PixelBuf::const_iterator itr = selected.begin();
	for ( ; itr != selected.end(); ++itr)
	{
		float x = (*itr)->x, y = (*itr)->y;

		int index = ((height - 1 - y) * width + x) * channels;
		unsigned char color[4];
		color[3] = 255;
		for (int k = 0; k < channels; ++k)
			color[k] = data[index++];

		glColor4ub(color[0], color[1], color[2], color[3]);
		glBegin(GL_QUADS);
			glVertex2f(x - 0.5f, y - 0.5f);
			glVertex2f(x + 0.5f, y - 0.5f);
			glVertex2f(x + 0.5f, y + 0.5f);
			glVertex2f(x - 0.5f, y + 0.5f);
		glEnd();
	}

	glPopMatrix();
}

void Render::drawRawPixelsSelectedFlag(const ee::RawPixels& original, const ee::RawPixels::PixelBuf& selected)
{
	int width, height, channels;
	original.getSize(width, height);
	channels = original.getChannels();
	assert(channels == 3 || channels == 4);

	glPushMatrix();
	glTranslatef(- width * 0.5f, - height * 0.5f, 0.0f);

	ee::RawPixels::PixelBuf::const_iterator itr = selected.begin();
	glColor4f(1.0f, 0.2f, 0.2f, 0.5f);
	for ( ; itr != selected.end(); ++itr)
	{
		float x = (*itr)->x, y = (*itr)->y;
		glBegin(GL_QUADS);
			glVertex2f(x - 0.5f, y - 0.5f);
			glVertex2f(x + 0.5f, y - 0.5f);
			glVertex2f(x + 0.5f, y + 0.5f);
			glVertex2f(x - 0.5f, y + 0.5f);
		glEnd();
	}

	glPopMatrix();
}

} // eanim