#include "PackPVR.h"
#include "Lzma.h"
#include "typedef.h"

#include <ee/Symbol.h>
#include <ee/SymbolMgr.h>
#include <ee/SettingData.h>
#include <ee/Config.h>
#include <ee/ImageVerticalFlip.h>

#include <easyimage.h>

#include <sprite2/DrawRT.h>

#include <string>

#include <assert.h>

namespace erespacker
{

#define PVR_TEXTURE_FLAG_TYPE_MASK  0xff

#define COMPRESSED_RGBA_PVRTC_4BPPV1_IMG 4
#define COMPRESSED_RGBA_PVRTC_2BPPV1_IMG 2
static char prv_texture_identifier[5] = "PVR!";

#define PVRTEX3_HEADERSIZE 52

typedef unsigned int    PVRTuint32;
// V3 Header Identifiers.
const PVRTuint32 PVRTEX3_IDENT      = 0x03525650; // 'P''V''R'3
const PVRTuint32 PVRTEX3_IDENT_REV  = 0x50565203;   

enum
{
	kPVRTextureFlagTypePVRTC_2 = 24,
	kPVRTextureFlagTypePVRTC_4
};

struct PVRTexHeader
{
	uint32_t headerLength;
	uint32_t height;
	uint32_t width;
	uint32_t numMipmaps;
	uint32_t flags;
	uint32_t dataLength;
	uint32_t bpp;
	uint32_t bitmaskRed;
	uint32_t bitmaskGreen;
	uint32_t bitmaskBlue;
	uint32_t bitmaskAlpha;
	uint32_t pvrTag;
	uint32_t numSurfs;
};

struct PVRTexHeaderV3 
{
	uint32_t  u32Version;     ///< Version of the file header, used to identify it.
	uint32_t  u32Flags;     ///< Various format flags.
	uint64_t  u64PixelFormat;   ///< The pixel format, 8cc value storing the 4 channel identifiers and their respective sizes.
	uint32_t  u32ColourSpace;   ///< The Colour Space of the texture, currently either linear RGB or sRGB.
	uint32_t  u32ChannelType;   ///< Variable type that the channel is stored in. Supports signed/unsigned int/short/byte or float for now.
	uint32_t  u32Height;      ///< Height of the texture.
	uint32_t  u32Width;     ///< Width of the texture.
	uint32_t  u32Depth;     ///< Depth of the texture. (Z-slices)
	uint32_t  u32NumSurfaces;   ///< Number of members in a Texture Array.
	uint32_t  u32NumFaces;    ///< Number of faces in a Cube Map. Maybe be a value other than 6.
	uint32_t  u32MIPMapCount;   ///< Number of MIP Maps in the texture - NB: Includes top level.
	uint32_t  u32MetaDataSize;  ///< Size of the accompanying meta data.  
};

//Compressed pixel formats
enum EPVRTPixelFormat
{
	ePVRTPF_PVRTCI_2bpp_RGB,
	ePVRTPF_PVRTCI_2bpp_RGBA,
	ePVRTPF_PVRTCI_4bpp_RGB,
	ePVRTPF_PVRTCI_4bpp_RGBA,
	ePVRTPF_PVRTCII_2bpp,
	ePVRTPF_PVRTCII_4bpp,
	ePVRTPF_ETC1,
	ePVRTPF_DXT1,
	ePVRTPF_DXT2,
	ePVRTPF_DXT3,
	ePVRTPF_DXT4,
	ePVRTPF_DXT5,

	//These formats are identical to some DXT formats.
	ePVRTPF_BC1 = ePVRTPF_DXT1,
	ePVRTPF_BC2 = ePVRTPF_DXT3,
	ePVRTPF_BC3 = ePVRTPF_DXT5,

	//These are currently unsupported:
	ePVRTPF_BC4,
	ePVRTPF_BC5,
	ePVRTPF_BC6,
	ePVRTPF_BC7,

	//These are supported
	ePVRTPF_UYVY,
	ePVRTPF_YUY2,
	ePVRTPF_BW1bpp,
	ePVRTPF_SharedExponentR9G9B9E5,
	ePVRTPF_RGBG8888,
	ePVRTPF_GRGB8888,
	ePVRTPF_ETC2_RGB,
	ePVRTPF_ETC2_RGBA,
	ePVRTPF_ETC2_RGB_A1,
	ePVRTPF_EAC_R11,
	ePVRTPF_EAC_RG11,

	//Invalid value
	ePVRTPF_NumCompressedPFs
};

PackPVR::PackPVR(bool fast)
	: m_fast(fast)
	, m_internal_format(COMPRESSED_RGBA_PVRTC_4BPPV1_IMG)
	, m_has_alpha(0)
	, m_width(0)
	, m_height(0)
	, m_data_sz(0)
	, m_buffer(NULL)
{
	m_type = TT_PVR;
}

PackPVR::~PackPVR()
{
	Clear();
}

void PackPVR::Load(const std::string& filepath)
{
	Clear();

	m_base_path = filepath.substr(0, filepath.find_last_of('.')) + ".png";

	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));
	
	PVRTexHeader header;
	uint8_t* bytes = NULL;
	uint32_t formatFlags;
	uint32_t type;
	fin.read(reinterpret_cast<char*>(&type), sizeof(uint32_t));
	fin.seekg(0, fin.beg);
	if (type != PVRTEX3_IDENT)
	{
		// read head
		fin.read(reinterpret_cast<char*>(&header.headerLength), sizeof(uint32_t));
		fin.read(reinterpret_cast<char*>(&header.height), sizeof(uint32_t));
		fin.read(reinterpret_cast<char*>(&header.width), sizeof(uint32_t));
		fin.read(reinterpret_cast<char*>(&header.numMipmaps), sizeof(uint32_t));
		fin.read(reinterpret_cast<char*>(&header.flags), sizeof(uint32_t));
		fin.read(reinterpret_cast<char*>(&header.dataLength), sizeof(uint32_t));
		fin.read(reinterpret_cast<char*>(&header.bpp), sizeof(uint32_t));
		fin.read(reinterpret_cast<char*>(&header.bitmaskRed), sizeof(uint32_t));
		fin.read(reinterpret_cast<char*>(&header.bitmaskGreen), sizeof(uint32_t));
		fin.read(reinterpret_cast<char*>(&header.bitmaskBlue), sizeof(uint32_t));
		fin.read(reinterpret_cast<char*>(&header.bitmaskAlpha), sizeof(uint32_t));
		fin.read(reinterpret_cast<char*>(&header.pvrTag), sizeof(uint32_t));
		fin.read(reinterpret_cast<char*>(&header.numSurfs), sizeof(uint32_t));

		uint32_t pvr_tag = header.pvrTag;
		if (prv_texture_identifier[0] != ((pvr_tag >>  0) & 0xff) ||
			prv_texture_identifier[1] != ((pvr_tag >>  8) & 0xff) ||
			prv_texture_identifier[2] != ((pvr_tag >> 16) & 0xff) ||
			prv_texture_identifier[3] != ((pvr_tag >> 24) & 0xff))
		{
			return;
		}
//		bytes = data + sizeof(header);

		uint32_t flags = header.flags;
		formatFlags = flags & PVR_TEXTURE_FLAG_TYPE_MASK;
	}
	else
	{
		// read head
		PVRTexHeaderV3 headerv3;
		fin.read(reinterpret_cast<char*>(&headerv3.u32Version), sizeof(uint32_t));		
		fin.read(reinterpret_cast<char*>(&headerv3.u32Flags), sizeof(uint32_t));		
		fin.read(reinterpret_cast<char*>(&headerv3.u64PixelFormat), sizeof(uint64_t));		
		fin.read(reinterpret_cast<char*>(&headerv3.u32ColourSpace), sizeof(uint32_t));		
		fin.read(reinterpret_cast<char*>(&headerv3.u32ChannelType), sizeof(uint32_t));		
		fin.read(reinterpret_cast<char*>(&headerv3.u32Height), sizeof(uint32_t));		
		fin.read(reinterpret_cast<char*>(&headerv3.u32Width), sizeof(uint32_t));		
		fin.read(reinterpret_cast<char*>(&headerv3.u32Depth), sizeof(uint32_t));		
		fin.read(reinterpret_cast<char*>(&headerv3.u32NumSurfaces), sizeof(uint32_t));		
		fin.read(reinterpret_cast<char*>(&headerv3.u32NumFaces), sizeof(uint32_t));		
		fin.read(reinterpret_cast<char*>(&headerv3.u32MIPMapCount), sizeof(uint32_t));		
		fin.read(reinterpret_cast<char*>(&headerv3.u32MetaDataSize), sizeof(uint32_t));		

		header.flags = headerv3.u32Flags;
		header.width = headerv3.u32Width;
		header.height = headerv3.u32Height;
		header.bitmaskAlpha = 1;

		fin.seekg(headerv3.u32MetaDataSize, fin.cur);

		if (headerv3.u64PixelFormat == ePVRTPF_PVRTCI_4bpp_RGB || headerv3.u64PixelFormat == ePVRTPF_PVRTCI_4bpp_RGBA) {
			formatFlags = kPVRTextureFlagTypePVRTC_4;
		} else if (headerv3.u64PixelFormat == ePVRTPF_PVRTCI_2bpp_RGB || headerv3.u64PixelFormat == ePVRTPF_PVRTCI_2bpp_RGBA) {
			formatFlags = kPVRTextureFlagTypePVRTC_2;
		}
	}

	if (formatFlags == kPVRTextureFlagTypePVRTC_4 || formatFlags == kPVRTextureFlagTypePVRTC_2)
	{
		if (formatFlags == kPVRTextureFlagTypePVRTC_4) {
			m_internal_format = COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
		} else if (formatFlags == kPVRTextureFlagTypePVRTC_2) {
			m_internal_format = COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
		}
		m_width = header.width;
		m_height = header.height;
		if (header.bitmaskAlpha) {
			m_has_alpha = 1;
		} else {
			m_has_alpha = 0;
		}

		m_data_sz = CalTexSize(m_internal_format, m_width, m_height);
		m_buffer = new uint8_t[m_data_sz];
		if (!m_buffer) {
			fin.close();
			return;
		}
		fin.read(reinterpret_cast<char*>(m_buffer), m_data_sz);		
	}

	fin.close();
}

void PackPVR::Store(const std::string& filepath, float scale) const
{
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));
	if (scale == 1) {
		Store(fout, m_buffer, m_data_sz, m_width, m_height);
	} else {
		StoreScaled(fout, scale);
	}
	fout.close();
}

int PackPVR::CalTexSize(int type, int width, int height)
{
	uint32_t bpp = 4;
	uint32_t block_sz = 0, w_blocks = 0, h_blocks = 0;
	// no mipmap
	// Calculate the data size for each texture level and respect the minimum number of blocks
	// while (dataOffset < dataLength){
	if (type == COMPRESSED_RGBA_PVRTC_4BPPV1_IMG) {
		block_sz = 4 * 4;
		w_blocks = width / 4;
		h_blocks = height / 4;
		bpp = 4;
	} else {
		block_sz = 8 * 4;
		w_blocks = width / 8;
		h_blocks = height / 4;
		bpp = 2;						
	}

	// Clamp to minimum number of blocks
	if (w_blocks < 2) {
		w_blocks = 2;
	}
	if (h_blocks < 2) {
		h_blocks = 2;
	}

	return w_blocks * h_blocks * ((block_sz * bpp) / 8);
}

void PackPVR::Clear()
{
	m_internal_format = COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
	m_has_alpha = 0;
	m_width = m_height = 0;
	m_data_sz = 0;
	delete[] m_buffer, m_buffer = NULL;
}

void PackPVR::Store(std::ofstream& fout, uint8_t* buffer, int buf_sz, int width, int height) const
{
	if (m_compress)
	{
		size_t sz = sizeof(int8_t) + sizeof(int8_t) + sizeof(int16_t) * 2;
		sz += sizeof(uint32_t);
		sz += buf_sz;

		uint8_t* buf = new uint8_t[sz];
		uint8_t* ptr = buf;

		memcpy(ptr, &m_type, sizeof(int8_t));
		ptr += sizeof(int8_t);
		memcpy(ptr, &m_internal_format, sizeof(int8_t));
		ptr += sizeof(int8_t);
		memcpy(ptr, &width, sizeof(int16_t));
		ptr += sizeof(int16_t);
		memcpy(ptr, &height, sizeof(int16_t));
		ptr += sizeof(int16_t);

		memcpy(ptr, &buf_sz, sizeof(uint32_t));
		ptr += sizeof(uint32_t);
		memcpy(ptr, buffer, buf_sz);
		ptr += buf_sz;

		uint8_t* dst = NULL;
		size_t dst_sz;
		Lzma::Compress(&dst, &dst_sz, buf, sz);
		delete[] buf;

		if (dst_sz > 0) {
			fout.write(reinterpret_cast<const char*>(&dst_sz), sizeof(uint32_t));
			fout.write(reinterpret_cast<const char*>(dst), dst_sz);
			delete[] dst;
		}
	}
	else
	{
		int sz = 0;
		sz += sizeof(int8_t);	// type
		sz += sizeof(int8_t);	// internal_format
		sz += sizeof(int16_t);	// width
		sz += sizeof(int16_t);	// height
		sz += sizeof(uint32_t) + buf_sz;

		sz = -sz;
		fout.write(reinterpret_cast<const char*>(&sz), sizeof(int32_t));

		fout.write(reinterpret_cast<const char*>(&m_type), sizeof(int8_t));
		fout.write(reinterpret_cast<const char*>(&m_internal_format), sizeof(int8_t));
		fout.write(reinterpret_cast<const char*>(&width), sizeof(int16_t));
		fout.write(reinterpret_cast<const char*>(&height), sizeof(int16_t));
		fout.write(reinterpret_cast<const char*>(&buf_sz), sizeof(uint32_t));
		fout.write(reinterpret_cast<const char*>(buffer), buf_sz);
	}
}

void PackPVR::StoreScaled(std::ofstream& fout, float scale) const
{
	ee::SettingData& sd = ee::Config::Instance()->GetSettings();
	bool old_clip = sd.open_image_edge_clip;
	bool old_premul = sd.pre_multi_alpha;
	sd.open_image_edge_clip = false;
	sd.pre_multi_alpha = false;

	ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(m_base_path);

	int w = static_cast<int>(m_width * scale),
		h = static_cast<int>(m_height * scale);
	s2::DrawRT rt;
	rt.Draw(sym, false, scale);
	uint8_t* png_buf = rt.StoreToMemory(w, h);
	sym->RemoveReference();

	ee::ImageVerticalFlip revert(png_buf, w, h);
	uint8_t* buf_revert = revert.Revert();		
	delete[] png_buf;

	eimage::TransToPVR trans(buf_revert, w, h, 4, false, m_fast);
	delete[] buf_revert;

	uint8_t* pvr_buf = trans.GetPixelsData(w, h);
	int sz = CalTexSize(m_internal_format, w, h);
	Store(fout, pvr_buf, sz, w, h);
	delete[] pvr_buf;

	sd.open_image_edge_clip = old_clip;
	sd.pre_multi_alpha = old_premul;
}

}