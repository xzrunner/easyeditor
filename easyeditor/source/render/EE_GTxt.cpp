#include "EE_GTxt.h"
#include "Config.h"

#include <gum/GTxt.h>

#include <string>
#include <vector>

namespace ee
{

void GTxt::Init()
{
	auto& fonts = Config::Instance()->GetFonts();
	auto& user_fonts = Config::Instance()->GetUserFonts();
	gum::GTxt::Instance()->Init(fonts, user_fonts);
}

}