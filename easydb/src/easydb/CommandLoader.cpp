#include "CommandLoader.h"
#include "CommandFactory.h"

#include "ChangeJsonFile.h"
#include "ChangeTPJsonFile.h"
#include "TrimImage.h"
#include "FormatJsonFile.h"
#include "PackParticle3D.h"
#include "ReverseAnimation.h"
#include "SetNameFromFile.h"
#include "Snapshoot.h"
#include "TransToGif.h"
#include "UniqueImage.h"
#include "VerifyImage.h"
#include "VerifyJson.h"

namespace edb
{

void CommandLoader::Init()
{
	CommandFactory* fc = CommandFactory::Instance();
	fc->Register(ChangeJsonFile::Create());
	fc->Register(ChangeTPJsonFile::Create());
	fc->Register(TrimImage::Create());
	fc->Register(FormatJsonFile::Create());
	fc->Register(PackParticle3D::Create());
	fc->Register(ReverseAnimation::Create());
	fc->Register(SetNameFromFile::Create());
	fc->Register(Snapshoot::Create());
	fc->Register(TransToGif::Create());
	fc->Register(UniqueImage::Create());
	fc->Register(VerifyImage::Create());
	fc->Register(VerifyJson::Create());
}

}