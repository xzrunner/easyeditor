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
#include "ScaleOverall.h"
#include "ScaleJson.h"
#include "AddFrameSprite.h"
#include "FormatTIDString.h"
#include "OutlineImage.h"
#include "OutlineToPolygon.h"
#include "FixSpriteColor.h"
#include "RotateTrimImage.h"
#include "RegularRectCut.h"
#include "RotateImage.h"
#include "PackRegularRect.h"
#include "FixRotateTrimJson.h"
#include "ScaleImage.h"
#include "BinRRP.h"
#include "NumberImages.h"
#include "BinEP.h"
#include "PackCoco.h"
#include "OutlineToTriStrip.h"
#include "TransToPVR.h"
#include "TransToETC1.h"
#include "BinRRR.h"
#include "BinB4R.h"
#include "BlockCompress.h"
#include "AnimToComplexWithColor.h"
#include "RemoveDuplicateSprite.h"
#include "CommandLoader.h"
#include "LRToComplex.h"
#include "TransOldLRFile.h"
#include "LRJsonPacker.h"
#include "TransOldShapeFile.h"
#include "LRSeparateComplex.h"
#include "LRPacker.h"
#include "PackTexture.h"
#include "CalculateImageArea.h"
#include "RectCutWithJson.h"
#include "LRExpansion.h"
#include "ImageVerticalFlip.h"
#include "PackLuaDescFile.h"
#include "PackLuaDescFileNew.h"

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
	fc->Register(ScaleOverall::Create());
	fc->Register(ScaleJson::Create());
	fc->Register(AddFrameSprite::Create());
	fc->Register(FormatTIDString::Create());
	fc->Register(OutlineImage::Create());
	fc->Register(OutlineToPolygon::Create());
	fc->Register(FixSpriteColor::Create());
	fc->Register(RotateTrimImage::Create());
	fc->Register(RegularRectCut::Create());
	fc->Register(RotateImage::Create());
	fc->Register(PackRegularRect::Create());
	fc->Register(FixRotateTrimJson::Create());
	fc->Register(ScaleImage::Create());
	fc->Register(BinRRP::Create());
	fc->Register(NumberImages::Create());
	fc->Register(BinEP::Create());
	fc->Register(PackCoco::Create());
	fc->Register(OutlineToTriStrip::Create());
	fc->Register(TransToPVR::Create());
	fc->Register(TransToETC1::Create());
	fc->Register(BinRRR::Create());
	fc->Register(BinB4R::Create());
	fc->Register(BlockCompress::Create());
	fc->Register(AnimToComplexWithColor::Create());
	fc->Register(RemoveDuplicateSprite::Create());
	fc->Register(LRToComplex::Create());
	fc->Register(TransOldLRFile::Create());
	fc->Register(LRJsonPacker::Create());
	fc->Register(TransOldShapeFile::Create());
	fc->Register(LRSeparateComplex::Create());
	fc->Register(LRPacker::Create());
	fc->Register(PackTexture::Create());
	fc->Register(CalImageArea::Create());
	fc->Register(RectCutWithJson::Create());
	fc->Register(LRExpansion::Create());
	fc->Register(ImageVerticalFlip::Create());
	fc->Register(PackLuaDescFile::Create());
	fc->Register(PackLuaDescFileNew::Create());
}

}