#include "IDemo.h"

#include <easy3d.h>

namespace libterrain3d
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

	wxString str;

	str.Printf("FPS:  %7.0f", m_timer.GetFps());
	log->Add(str.ToStdString());

	str.Printf("Vertices: %d", m_verts_per_frame);
	log->Add(str.ToStdString());

	str.Printf("Tris/S:   %d", m_tris_per_frame);
	log->Add(str.ToStdString());

	str.Printf("MTris/S:  %.3f", m_tris_per_frame * m_timer.GetFps() / 1000000.0f);
	log->Add(str.ToStdString());

	log->Draw();
}

}