#include "bsn/NodeFactory.h"
#include "bsn/NodeType.h"

#include "bsn/ComplexSpr.h"

#include <guard/check.h>
#include <bs/ImportStream.h>
#include <json/value.h>

#include <algorithm>

namespace bsn
{

NodeSpr* NodeFactory::CreateNodeSpr(mm::LinearAllocator& alloc, bs::ImportStream& is)
{
	NodeSpr* spr = nullptr;

	uint8_t type = is.UInt8();
	switch (type)
	{
	case NODE_COMPLEX:
		spr = ComplexSpr::Create(alloc, is);
		break;
	default:
		GD_REPORT_ASSERT("unknown node type.");
	}
	
	return spr;
}

NodeSpr* NodeFactory::CreateNodeSpr(mm::LinearAllocator& alloc, const Json::Value& val)
{
	NodeSpr* spr = nullptr;

	std::string filepath = val["filepath"].asString();
	NodeType type = GetNodeType(filepath);
	GD_ASSERT(type != NODE_INVALID, "err node type.");
	switch (type)
	{
	case NODE_COMPLEX:
		spr = ComplexSpr::Create(alloc, val);
		break;
	default:
		GD_REPORT_ASSERT("unknown node type.");
	}

	return spr;
}

NodeType NodeFactory::GetNodeType(const std::string& filepath)
{
	NodeType type = NODE_INVALID;
	std::string ext = filepath.substr(filepath.rfind('.') + 1);
	std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
	if (ext == "png" || ext == "jpg" || ext == "bmp" || ext == "ppm" || ext == "pvr" || ext == "pkm") 
	{
		type = NODE_IMAGE;
	} 
	else if (ext == "json") 
	{
		std::string tag = filepath.substr(filepath.rfind('_') + 1);
		std::transform(tag.begin(), tag.end(), tag.begin(), tolower);
		if (tag == "scale9") {
			type = NODE_SCALE9;
		} else if (tag == "icon") {
			type = NODE_ICON;
		} else if (tag == "texture") {
			type = NODE_TEXTURE;
		} else if ((tag == "text")) {
			type = NODE_TEXTBOX;
		} else if (tag == "complex") {
			type = NODE_COMPLEX;
		} else if (tag == "anim") {
			type = NODE_ANIMATION;
		} else if (tag == "particle") {
			type = NODE_PARTICLE3D;
		} else if (tag == "particle2d") {
			type = NODE_PARTICLE2D;
		} else if (tag == "shape") {
			type = NODE_SHAPE;
		} else if (tag == "mesh") {
			type = NODE_MESH;
		} else if (tag == "mask") {
			type = NODE_MASK;
		} else if (tag == "trail") {
			type = NODE_TRAIL;
		} else if (tag == "skeleton") {
			type = NODE_SKELETON;
		}
	}
	else if (ext == "mp3") 
	{
		type = NODE_AUDIO;
	}

	return type;
}

}