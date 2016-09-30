#ifndef _EASYSKELETON_FILE_STORE_H_
#define _EASYSKELETON_FILE_STORE_H_

#include <json/json.h>

#include <string>
#include <vector>

namespace ee { class Sprite; }

namespace eskeleton
{

class Joint;

class FileStorer
{
public:
	static void Store(const std::string& filepath, const std::vector<ee::Sprite*>& sprs);

private:
	static Json::Value StoreSprite(const std::vector<ee::Sprite*>& sprs, const std::string& dir);

	static Json::Value StoreSkeleton(const std::vector<ee::Sprite*>& sprs);
	static Json::Value StoreJoint(const Joint* joint, const std::map<const Joint*, int>& map_joint_id);
	
}; // FileStorer

}

#endif // _EASYSKELETON_FILE_STORE_H_