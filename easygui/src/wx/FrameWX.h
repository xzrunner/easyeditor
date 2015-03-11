#ifndef _EASYGUI_FRAME_WX_H_
#define _EASYGUI_FRAME_WX_H_

#include "interface/Frame.h"

#include <wx/frame.h>
#include <string>

namespace egui
{

class FrameWX : public Frame
{
public:
	FrameWX(const std::string& title);
	virtual ~FrameWX();

	//
	// interface Window
	//
	virtual void* GetRawPtr();

	//
	// interface Frame
	//
	virtual void Show();

private:
	class Impl : public wxFrame
	{
	public:
		Impl(const std::string& title);

	private:
		void OnClose(wxCloseEvent& event);

	private:
		DECLARE_EVENT_TABLE()

	}; // Impl

private:
	Impl* m_impl;

}; // FrameWX

}

#endif // _EASYGUI_FRAME_WX_H_