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
//		m_ps->RemoveReference();
//	}
//	if (m_selected) {
//		m_selected->RemoveReference();
//	}
//}
//
//void StageData::SetValue(int key, const Data& data)
//{
//	ee::UICallback* cb = GetCurrPS();
//	if (cb) {
//		cb->SetValue(key, data);
//	}
//}
//
//void StageData::GetValue(int key, Data& data)
//{
//	ee::UICallback* cb = GetCurrPS();
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
//			m_selected->RemoveReference();
//		}
//		m_selected = selected;
//		if (m_selected) {
//			m_selected->AddReference();
//		}
//		m_toolbar->ResetUIFromPS();
//	}
//}
//
//void StageData::ChangePSSymbol(ee::Symbol* sym)
//{
//	if (!m_ps) {
//		m_ps = new ParticleSystem(1000);
//	}
//
//	m_ps->SetSymbol(sym);
//	m_toolbar->InitPSFromUI();
//	m_ps->Start();
//	m_ps->Reset();
//}
//
//}