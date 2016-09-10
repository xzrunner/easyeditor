#include "EE_GTxt.h"
#include "Config.h"

#include <gum/GUM_GTxt.h>

#include <string>
#include <vector>

namespace ee
{

void GTxt::Init()
{
	const std::vector<std::pair<std::string, std::string> >& 
		fonts = Config::Instance()->GetFonts();
	const std::vector<std::pair<std::string, std::string> >& 
		user_fonts = Config::Instance()->GetUserFonts();
	gum::GTxt::Instance()->Init(fonts, user_fonts);
}

}