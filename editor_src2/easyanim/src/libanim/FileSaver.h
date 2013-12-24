#pragma once

#include <drag2d.h>

namespace libanim
{
	class FileSaver
	{
	public:
		static void store(const std::string& filepath, const d2d::AnimSymbol& symbol);

	private:
		static Json::Value store(d2d::AnimSymbol::Layer* layer, const wxString& dlg);
		static Json::Value store(d2d::AnimSymbol::Frame* frame, const wxString& dlg);
		static Json::Value store(d2d::ISprite* sprite, const wxString& dlg);

	}; // FileSaver
}

