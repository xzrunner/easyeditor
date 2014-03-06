
#include "StageCanvas.h"
#include "StagePanel.h"

namespace eparticle
{
namespace coco3d
{

	BEGIN_EVENT_TABLE(StageCanvas, d2d::PerspectCanvas)
		EVT_TIMER(TIMER_ID, StageCanvas::onTimer)
	END_EVENT_TABLE()

	StageCanvas::StageCanvas(StagePanel* editPanel)
		: d2d::PerspectCanvas(editPanel)
		, m_stage(editPanel)
		, m_timer(this, TIMER_ID)
	{
		m_bgColor.set(0, 0, 0, 1);

		m_timer.Start(1000 / FRAME_RATE);
	}

	StageCanvas::~StageCanvas()
	{
	}

	void StageCanvas::onDraw()
	{
 		if (m_stage->m_particle)
 		{
 			glEnable(GL_BLEND);
 			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 			m_stage->m_particle->draw();
 			glDisable(GL_BLEND);
 		}

		//////////////////////////////////////////////////////////////////////////

//    glPushMatrix();
//
////	glTranslatef(0.0f, 0.0f, -200.0f);
//
//     glRotatef(m_stage->xRot, 1.0f, 0.0f, 0.0f);
//     glRotatef(m_stage->yRot, 0.0f, 1.0f, 0.0f);
//
//    // Draw six quads
//    glBegin(GL_QUADS);
//        // Front Face
//        // White
//        glColor3ub((GLubyte) 255, (GLubyte)255, (GLubyte)255);
//        glVertex3f(50.0f,50.0f,50.0f);
//
//        // Yellow
//        glColor3ub((GLubyte) 255, (GLubyte)255, (GLubyte)0);
//        glVertex3f(50.0f,-50.0f,50.0f);
//
//        // Red
//        glColor3ub((GLubyte) 255, (GLubyte)0, (GLubyte)0);
//        glVertex3f(-50.0f,-50.0f,50.0f);
//
//        // Magenta
//        glColor3ub((GLubyte) 255, (GLubyte)0, (GLubyte)255);
//        glVertex3f(-50.0f,50.0f,50.0f);
//
//	
//	// Back Face
//        // Cyan
//        glColor3f(0.0f, 1.0f, 1.0f);
//        glVertex3f(50.0f,50.0f,-50.0f);
//
//        // Green
//        glColor3f(0.0f, 1.0f, 0.0f);
//        glVertex3f(50.0f,-50.0f,-50.0f);
//		
//        // Black
//        glColor3f(0.0f, 0.0f, 0.0f);
//        glVertex3f(-50.0f,-50.0f,-50.0f);
//
//        // Blue
//        glColor3f(0.0f, 0.0f, 1.0f);
//        glVertex3f(-50.0f,50.0f,-50.0f);
//	
//	// Top Face
//        // Cyan
//        glColor3f(0.0f, 1.0f, 1.0f);
//        glVertex3f(50.0f,50.0f,-50.0f);
//
//        // White
//        glColor3f(1.0f, 1.0f, 1.0f);
//        glVertex3f(50.0f,50.0f,50.0f);
//
//        // Magenta
//        glColor3f(1.0f, 0.0f, 1.0f);
//        glVertex3f(-50.0f,50.0f,50.0f);
//
//        // Blue
//        glColor3f(0.0f, 0.0f, 1.0f);
//        glVertex3f(-50.0f,50.0f,-50.0f);
//	
//	// Bottom Face
//        // Green
//        glColor3f(0.0f, 1.0f, 0.0f);
//        glVertex3f(50.0f,-50.0f,-50.0f);
//
//        // Yellow
//        glColor3f(1.0f, 1.0f, 0.0f);
//        glVertex3f(50.0f,-50.0f,50.0f);
//
//        // Red
//        glColor3f(1.0f, 0.0f, 0.0f);
//        glVertex3f(-50.0f,-50.0f,50.0f);
//
//        // Black
//        glColor3f(0.0f, 0.0f, 0.0f);
//        glVertex3f(-50.0f,-50.0f,-50.0f);
//	
//	// Left face
//        // White
//        glColor3f(1.0f, 1.0f, 1.0f);
//        glVertex3f(50.0f,50.0f,50.0f);
//
//        // Cyan
//        glColor3f(0.0f, 1.0f, 1.0f);
//        glVertex3f(50.0f,50.0f,-50.0f);
//
//        // Green
//        glColor3f(0.0f, 1.0f, 0.0f);
//        glVertex3f(50.0f,-50.0f,-50.0f);
//
//        // Yellow
//        glColor3f(1.0f, 1.0f, 0.0f);
//        glVertex3f(50.0f,-50.0f,50.0f);
//	
//	// Right face
//        // Magenta
//        glColor3f(1.0f, 0.0f, 1.0f);
//        glVertex3f(-50.0f,50.0f,50.0f);
//
//        // Blue
//        glColor3f(0.0f, 0.0f, 1.0f);
//        glVertex3f(-50.0f,50.0f,-50.0f);
//
//        // Black
//        glColor3f(0.0f, 0.0f, 0.0f);
//        glVertex3f(-50.0f,-50.0f,-50.0f);
//
//        // Red
//        glColor3f(1.0f, 0.0f, 0.0f);
//        glVertex3f(-50.0f,-50.0f,50.0f);
//    glEnd();
//
//    glPopMatrix();

	}

	void StageCanvas::onTimer(wxTimerEvent& event)
	{
		if (m_stage->m_particle)
			m_stage->m_particle->update(1.0f / FRAME_RATE);

		Refresh();
	}

}
}