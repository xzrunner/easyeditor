//#include "StageData.h"
//#include "ParticleSystem.h"
//#include "Symbol.h"
//#include "ToolbarPanel.h"
//
//namespace eparticle2d
//{
//
//StageData::StageData()
//	: m_ps(NULL)
//	, m_selected(NULL)
//{
//}
//
//StageData::~StageData()
//{
//	if (m_ps) {
//		m_ps->Release();
//	}
//	if (m_selected) {
//		m_selected->Release();
//	}
//}
//
//void StageData::SetValue(int key, const Data& data)
//{
//	d2d::UICallback* cb = GetCurrPS();
//	if (cb) {
//		cb->SetValue(key, data);
//	}
//}
//
//void StageData::GetValue(int key, Data& data)
//{
//	d2d::UICallback* cb = GetCurrPS();
//	if (cb) {
//		cb->GetValue(key, data);
//	}	
//}
//
//ParticleSystem* StageData::GetCurrPS() const
//{
//	if (m_selected) {
//		return m_selected->GetPS();
//	} else {
//		return m_ps;
//	}
//}
//
//void StageData::SetSelected(Symbol* selected)
//{
//	if (m_selected != selected) {
//		if (m_selected) {
//			m_selected->Release();
//		}
//		m_selected = selected;
//		if (m_selected) {
//			m_selected->Retain();
//		}
//		m_toolbar->ResetUIFromPS();
//	}
//}
//
//void StageData::ChangePSSymbol(d2d::Symbol* symbol)
//{
//	if (!m_ps) {
//		m_ps = new ParticleSystem(1000);
//	}
//
//	m_ps->SetSymbol(symbol);
//	m_toolbar->InitPSFromUI();
//	m_ps->Start();
//	m_ps->Reset();
//}
//
//}