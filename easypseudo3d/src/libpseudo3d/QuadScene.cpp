#include "QuadScene.h"

#include <easy3d.h>

namespace epseudo3d
{

QuadScene::QuadScene()
	: m_img(NULL)
{
}

QuadScene::~QuadScene()
{
	delete m_img;
}

void QuadScene::Load()
{
	m_img = d2d::ImageMgr::Instance()->getItem("box/crate.jpg");
}

void QuadScene::Draw() const
{
	e3d::DrawCube(vec3(-5, -5, -5), vec3(5, 5, 5), m_img->textureID());
}

}