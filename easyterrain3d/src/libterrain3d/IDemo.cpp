#include "IDemo.h"

#include <ee/StringHelper.h>

#include <easy3d.h>

namespace eterrain3d
{

void IDemo::Load() 
{ 
	m_timer.Init(); 
}

void IDemo::Update()
{
	m_timer.Update();

	m_verts_per_frame = m_tris_per_frame = 0;
}

void IDemo::Draw() const 
{ 
	e3d::LogViewer* log = e3d::LogViewer::Instance();

	log->Add(ee::StringHelper::Format("FPS:  %7.0f", m_timer.GetFps()));
	log->Add(ee::StringHelper::Format("Vertices: %d", m_verts_per_frame));
	log->Add(ee::StringHelper::Format("Tris/S:   %d", m_tris_per_frame));
	log->Add(ee::StringHelper::Format("MTris/S:  %.3f", m_tris_per_frame * m_timer.GetFps() / 1000000.0f));

	log->Draw();
}

}