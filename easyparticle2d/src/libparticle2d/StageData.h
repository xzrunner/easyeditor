//#ifndef _EASYPARTICLE2D_STAGE_DATA_H_
//#define _EASYPARTICLE2D_STAGE_DATA_H_
//
//#include <drag2d.h>
//
//namespace eparticle2d
//{
//
//class ParticleSystem;
//class Symbol;
//class ToolbarPanel;
//
//class StageData : public d2d::UICallback
//{
//public:
//	StageData();
//	~StageData();
//
//	//
//	// UICallback interface
//	//
//	virtual void SetValue(int key, const Data& data);
//	virtual void GetValue(int key, Data& data);
//
//	ParticleSystem* GetPS() const { return m_ps; }
//	ParticleSystem* GetCurrPS() const;
//
//	void SetSelected(Symbol* selected);
//
//	void SetToolbar(ToolbarPanel* toolbar) { m_toolbar = toolbar; }
//
//	void ChangePSSymbol(d2d::ISymbol* symbol);
//
//private:
//	ParticleSystem* m_ps;
//
//	Symbol* m_selected;
//
//	// only for reset toolbar after select sprite 
//	ToolbarPanel* m_toolbar;
//
//}; // StageData
//
//}
//
//#endif // _EASYPARTICLE2D_STAGE_DATA_H_