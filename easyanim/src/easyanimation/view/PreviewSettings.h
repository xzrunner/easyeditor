#ifndef _EASYANIM_PREVIEW_SETTINGS_H_
#define _EASYANIM_PREVIEW_SETTINGS_H_

namespace eanim
{

struct PlaySettings
{
	bool isCirculate;
	bool isStop;

	PlaySettings()
	{
		isCirculate = true;
		isStop = false;
	}

}; // PlaySettings

}

#endif // _EASYANIM_PREVIEW_SETTINGS_H_