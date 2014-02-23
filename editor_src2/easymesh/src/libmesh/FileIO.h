#ifndef _EASYMESH_FILE_IO_H_
#define _EASYMESH_FILE_IO_H_

namespace Json { class Value; }

namespace emesh
{

class Mesh;
class Symbol;
class FileIO
{
public:
	static void store(const char* filepath, const Symbol* symbol);
	static void load(const char* filepath, Symbol* symbol);

private:
	static void storeTransform(const Mesh* mesh, Json::Value& value);
	static void loadTransform(const Json::Value& value, Mesh* mesh);

}; // FileIO

}

#endif // _EASYMESH_FILE_IO_H_