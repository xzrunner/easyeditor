#include "PVRLoader.h"
#include "Lzma.h"
#include "image_type.h"

#include <assert.h>

namespace epbin
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

PVRLoader::PVRLoader()
{
	m_type = TYPE_PVRTC;

	m_tex.width = m_tex.height = 0;
	m_tex.internal_format = COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
	m_tex.has_alpha = 0;
	ClearImageData();
}

PVRLoader::~PVRLoader()
{
	
}

void PVRLoader::Load(const std::string& filepath)
{
	std::ifstream fin(filepath.c_str(), std::ios::binary);
	
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

	uint32_t width = 0, height = 0, bpp = 4;
	uint32_t blockSize = 0, widthBlocks = 0, heightBlocks = 0;
	if (formatFlags == kPVRTextureFlagTypePVRTC_4 || formatFlags == kPVRTextureFlagTypePVRTC_2)
	{
		ClearImageData();
		if (formatFlags == kPVRTextureFlagTypePVRTC_4) {
			m_tex.internal_format = COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
		} else if (formatFlags == kPVRTextureFlagTypePVRTC_2) {
			m_tex.internal_format = COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
		}
		m_tex.width = width = header.width;
		m_tex.height = height = header.height;
		if (header.bitmaskAlpha) {
			m_tex.has_alpha = 1;
		} else {
			m_tex.has_alpha = 0;
		}

		// no mipmap
		// Calculate the data size for each texture level and respect the minimum number of blocks
		// while (dataOffset < dataLength){
		if (formatFlags == kPVRTextureFlagTypePVRTC_4) {
			blockSize = 4 * 4;
			widthBlocks = width / 4;
			heightBlocks = height / 4;
			bpp = 4;
		} else {
			blockSize = 8 * 4;
			widthBlocks = width / 8;
			heightBlocks = height / 4;
			bpp = 2;						
		}

		// Clamp to minimum number of blocks
		if (widthBlocks < 2) {
			widthBlocks = 2;
		}
		if (heightBlocks < 2) {
			heightBlocks = 2;
		}

		uint32_t dataSize = widthBlocks * heightBlocks * ((blockSize * bpp) / 8);
		assert(m_tex.image_data_count < 10);
		assert(dataSize < sizeof(m_tex.image_data[0].data));
		m_tex.image_data[m_tex.image_data_count].size = dataSize;
		fin.read(reinterpret_cast<char*>(&m_tex.image_data[m_tex.image_data_count].data), dataSize);		
		++m_tex.image_data_count;

		width = std::max((int)(width >> 1), 1);
		height = std::max((int)(height >> 1), 1);

		// }
	}

	fin.close();
}

void PVRLoader::Store(std::ofstream& fout) const
{
	if (m_compress)
	{
		size_t sz = sizeof(int8_t) + sizeof(int8_t) + sizeof(int16_t) * 2;
		for (int i = 0; i < m_tex.image_data_count; ++i) {
			const Slice& s = m_tex.image_data[i];
			sz += sizeof(uint32_t);
			sz += s.size;
		}
		uint8_t* buf = new uint8_t[sz];
		uint8_t* ptr = buf;

		memcpy(ptr, &m_type, sizeof(int8_t));
		ptr += sizeof(int8_t);
		memcpy(ptr, &m_tex.internal_format, sizeof(int8_t));
		ptr += sizeof(int8_t);
		memcpy(ptr, &m_tex.width, sizeof(int16_t));
		ptr += sizeof(int16_t);
		memcpy(ptr, &m_tex.height, sizeof(int16_t));
		ptr += sizeof(int16_t);
		for (int i = 0; i < m_tex.image_data_count; ++i) {
			const Slice& s = m_tex.image_data[i];
			memcpy(ptr, &s.size, sizeof(uint32_t));
			ptr += sizeof(uint32_t);
			memcpy(ptr, &s.data[0], s.size);
			ptr += s.size;
		}

		uint8_t* dst = NULL;
		size_t dst_sz;
		Lzma::Compress(&dst, &dst_sz, buf, sz);
		delete[] buf;

		fout.write(reinterpret_cast<const char*>(&dst_sz), sizeof(uint32_t));
		fout.write(reinterpret_cast<const char*>(dst), dst_sz);
		delete[] dst;
	}
	else
	{
		int sz = 0;
		sz += sizeof(int8_t);	// type
		sz += sizeof(int8_t);	// internal_format
		sz += sizeof(int16_t);	// width
		sz += sizeof(int16_t);	// height
		for (int i = 0; i < m_tex.image_data_count; ++i) {
			const Slice& s = m_tex.image_data[i];
			sz += sizeof(uint32_t) + s.size;
		}
		sz = -sz;
		fout.write(reinterpret_cast<const char*>(&sz), sizeof(int32_t));

		fout.write(reinterpret_cast<const char*>(&m_type), sizeof(int8_t));
		fout.write(reinterpret_cast<const char*>(&m_tex.internal_format), sizeof(int8_t));
		fout.write(reinterpret_cast<const char*>(&m_tex.width), sizeof(int16_t));
		fout.write(reinterpret_cast<const char*>(&m_tex.height), sizeof(int16_t));
		for (int i = 0; i < m_tex.image_data_count; ++i) {
			const Slice& s = m_tex.image_data[i];
			fout.write(reinterpret_cast<const char*>(&s.size), sizeof(uint32_t));
			fout.write(reinterpret_cast<const char*>(&s.data[0]), s.size);
		}		
	}
}

void PVRLoader::ClearImageData()
{
	m_tex.image_data_count = 0;
}

}