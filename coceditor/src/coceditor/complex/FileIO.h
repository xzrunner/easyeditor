#pragma once



namespace coceditor
{
namespace complex
{
	class FileIO
	{
	public:
		static void store(const char* filepath, const complex::Symbol* complexSymbol);

	private:
		static Json::Value store(ee::ISprite* sprite);

	}; // FileIO
}
}

