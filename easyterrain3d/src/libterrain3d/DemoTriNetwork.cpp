#include "DemoTriNetwork.h"

#include "randtab.h"

#include <SM_Vector.h>
#include <SM_Calc.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Model3Shader.h>
#include <unirender/RenderContext.h>
#include <gum/RenderContext.h>

#include <painting3/PrimitiveDraw.h>
#include <painting3/Camera.h>

namespace eterrain3d
{

static const float EDGE = 10;

static const int MAX_LEVEL = 10;

DemoTriNetwork::DemoTriNetwork(pt3::Camera& cam)
	: m_cam(cam)
	, m_tex(0)
{
}

void DemoTriNetwork::Load()
{
	IDemo::Load();

	m_cam.Pitch(-45);
	m_cam.MoveToward(5);

	GenTex();

	m_level2size.reserve(MAX_LEVEL + 1);
	for(int i = 0; i <= MAX_LEVEL; ++i) {
		m_level2size.push_back(3.0f / ((float)sqrt((float)(1 << i))));
	}
}

void DemoTriNetwork::Draw() const
{
	IDemo::Draw();

	const float HALF_EDGE = EDGE * 0.5f;
	sm::vec3 v0(-HALF_EDGE,  HALF_EDGE, 0), 
		 v1( HALF_EDGE,  HALF_EDGE, 0),
		 v2( HALF_EDGE, -HALF_EDGE, 0),
		 v3(-HALF_EDGE, -HALF_EDGE, 0);
	DrawTriByLevel(v1, v0, v2, 0);
	DrawTriByLevel(v3, v0, v2, 0);
}

void DemoTriNetwork::DrawTriByLevel(const sm::vec3& v0, const sm::vec3& v1, const sm::vec3& v2, int level) const
{
	sm::vec3 c = (v0 + v1 + v2) / 3;
	float dis = sm::dis_pos3_to_pos3(c, m_cam.GetPos());
	float len = sm::dis_square_pos3_to_pos3(v1, v2);
//	ee3::LogViewer::Instance()->Add(("dis:"+wxString::FromDouble(dis)).ToStdString());

	if (level < MAX_LEVEL && len > dis * 0.005f)
	{
		sm::vec3 new_node = (v1 + v2) * 0.5f;
		new_node.z = GetRandHeight(v1, v2, level);
		DrawTriByLevel(new_node, v0, v1, level + 1);
		DrawTriByLevel(new_node, v0, v2, level + 1);
		return;
	}

	n3::PrimitiveDraw::Line(v0, v1);
	n3::PrimitiveDraw::Line(v1, v2);
	n3::PrimitiveDraw::Line(v2, v0);

//	ee3::DrawTriLine(v0, v1, v2);

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::MODEL3);
	sl::Model3Shader* shader = static_cast<sl::Model3Shader*>(mgr->GetShader());	

	sl::Model3Shader::Material material;
	material.ambient.Assign(0.04f, 0.04f, 0.04f);
	material.diffuse.Assign(1, 1, 1);
	material.specular.Assign(1, 1, 1);
	material.shininess = 50;
	material.tex_id = m_tex;
	shader->SetMaterial(material);

	std::vector<float> vertices = {
		v0.x, v0.y, v0.z, 1, 0,
		v1.x, v1.y, v1.z, 0, 0,
		v2.x, v2.y, v2.z, 1, 1
	};

	std::vector<uint16_t> indices = { 0, 1, 2 };

	shader->Draw(&vertices[0], vertices.size(), &indices[0], indices.size(), false, true);

	m_verts_per_frame += 3;
	m_tris_per_frame += 1;
}

void DemoTriNetwork::GenTex()
{
	//generate pixels
	const int SIZE = 128;
	uint8_t* pixels = new uint8_t[SIZE * SIZE * 4];
	for(int y = 0; y < SIZE; ++y)
	{
		for(int x = 0; x < SIZE; ++x)
		{
			//create bump-shaped function f that is zero on each edge
			float a0 = (float)y / 127.0f;
			float a1 = (float)(127 - x) / 127.0f;
			float a2 = (float)(x - y) / 127.0f;
			float f = (float)sqrt(a0*a1*a2);

			//quantize the function value and make into color
			//store in rgb components of texture entry
			int k = (int)floor(1400.0f * f) ;
			if(k < 0) {
				k = 0;
			}
			if (k > 255) {
				k = 255;
			}

			pixels[4*(y*SIZE+x)+0] = 30 + (sqrt((float)k)) / 290.0f;
			pixels[4*(y*SIZE+x)+1] = k;
			pixels[4*(y*SIZE+x)+2] = (k < 128 ? 0 : (k-128)*2);
			pixels[4*(y*SIZE+x)+3] = 0;
		}
	}

	//set up the gridview texture
	m_tex = gum::RenderContext::Instance()->GetImpl()->CreateTexture(pixels, SIZE, SIZE, ur::TEXTURE_A8);
}

float DemoTriNetwork::GetRandHeight(const sm::vec3& v1, const sm::vec3& v2, int level) const
{
	float rand_height;

	float src[2] = {
		(v1.x + v2.x) * 0.5f,
		0.0f
	};

	//determine random perturbation of center z using hash of x,y
	//random number lookup per byte of (x, z) data, all added
	unsigned char* pC = (unsigned char*)src;
	unsigned int uiS = 0;
	for(int i = 0; i < 8; ++i) {
		uiS += randtab[(i << 8) | pC[i]];
	}

	//stuff random hash value bits from uiS into float (float viewed
	//as an int, IEEE float tricks here...)
	int* pInt = (int*)(&rand_height);

	*pInt	    = 0x40000000 + (uiS & 0x007fffff);
	rand_height-= 3.0f;
	rand_height	= rand_height * m_level2size[level];

	return (v1.z + v2.z) * 0.5f + rand_height;
}

}