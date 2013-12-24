///*
//* Copyright (c) 2013 Guang Zhu http://runnersoft.net
//*
//* This software is provided 'as-is', without any express or implied
//* warranty.  In no event will the authors be held liable for any damages
//* arising from the use of this software.
//* Permission is granted to anyone to use this software for any purpose,
//* including commercial applications, and to alter it and redistribute it
//* freely, subject to the following restrictions:
//* 1. The origin of this software must not be misrepresented; you must not
//* claim that you wrote the original software. If you use this software
//* in a product, an acknowledgment in the product documentation would be
//* appreciated but is not required.
//* 2. Altered source versions must be plainly marked as such, and must not be
//* misrepresented as being the original software.
//* 3. This notice may not be removed or altered from any source distribution.
//*/
//
//#ifndef EPARTICLE_TOOLBAR_PANEL_H
//#define EPARTICLE_TOOLBAR_PANEL_H
//
//#include <drag2d.h>
//
//#include "Color.h"
//
//namespace eparticle
//{
//	class StagePanel;
//
//	class ToolbarPanel : public d2d::ToolbarPanel
//	{
//	public:
//		ToolbarPanel(wxWindow* parent);
//
//		void load(std::ifstream& fin);
//		void store(std::ofstream& fout) const;
//
//	protected:
//		virtual wxSizer* initLayout();
//
//	private:
//		void enable(bool enable);
//
//		void initParticle();
//
//		void onOpenImage(wxCommandEvent& event);
//
//		void onSetEmissionRate(wxSpinEvent& event);
//
//		void onSetSpeed(wxSpinEvent& event);
//
//		void onSetGravity(wxSpinEvent& event);
//
//		void onSetSize(wxSpinEvent& event);
//
//		void onSetColorStart(wxCommandEvent& event);
//		void onSetColorEnd(wxCommandEvent& event);
//		void onSetColorFromSpin(wxSpinEvent& event);
//		void resetParticleColor();
//
//		void onSetLifeTime(wxSpinEvent& event);
//		void onSetParticleLife(wxSpinEvent& event);
//
//		void onSetDirection(wxSpinEvent& event);
//		void onSetSpread(wxSpinEvent& event);
//
//		void onSetRadialAcceleration(wxSpinEvent& event);
//		void onSetTangentialAcceleration(wxSpinEvent& event);
//
//	private:
//		static const float EMISSION_RATE;
//		static const float MIN_SPEED;
//		static const float MAX_SPEED;
//		static const float GRAVITY;
//		static const float SIZE1;
//		static const float SIZE2;
//		static const float LIFE_TIME;
//		static const float PARTICLE_TIME;
//		static const float DIRECTION;
//		static const float SPREAD;
//		static const float RADIAL_ACC;
//		static const float TANGENTIAL_ACC;
//
//	private:
//		struct Color
//		{
//			love::graphics::Color val;
//			wxButton* btn;
//
//			wxSpinCtrl *sr, *sg, *sb, *sa;
//
//			void set(int r, int g, int b, int a);
//
//			void enable(bool enable); 
//
//			void load(std::ifstream& fin);
//			void store(std::ofstream& fout) const;
//		};
//
//	private:
//		d2d::Image* m_image;
//
//		wxSpinCtrl* m_spinEmissionRate;
//		wxSpinCtrl *m_spinMinSpeed, *m_spinMaxSpeed;
//		wxSpinCtrl* m_spinGravity;
//		wxSpinCtrl *m_spinSize1, *m_spinSize2; 
//		Color m_startColor, m_endColor;
//		wxSpinCtrl* m_spinLifeTime;
//		wxSpinCtrl* m_spinParticleTime;
//		wxSpinCtrl* m_spinDirection;
//		wxSpinCtrl* m_spinSpread;
//		wxSpinCtrl* m_spinRadialAcc;
//		wxSpinCtrl* m_spinTangentialAcc;
//
//	}; // ToolbarPanel
//}
//
//#endif // EPARTICLE_TOOLBAR_PANEL_H
