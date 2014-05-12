
#ifndef EBUILDER_SETTINGS_H
#define EBUILDER_SETTINGS_H

namespace ebuilder
{
	class Settings
	{
	public:

		enum CodeType
		{
			e_love2d = 0,
			e_libgdx,
			e_cocos2dx,
//			e_unity,
			e_easy2d
		};

		static CodeType code;

	}; // Settings
}

#endif // EBUILDER_SETTINGS_H
