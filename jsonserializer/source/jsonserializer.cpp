#include "jsonserializer.h"

#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/writer.h>

namespace jsons
{

void read_from_file(const char* filepath, rapidjson::Document& doc)
{
	FILE* read_fp = fopen(filepath, "rb");
	char read_buffer[65536];
	rapidjson::FileReadStream read(read_fp, read_buffer, sizeof(read_buffer));

	doc.ParseStream(read);

	fclose(read_fp);
}

void write_to_file(const char* filepath, const rapidjson::Document& doc)
{
	char write_buffer[65536];
	FILE* fp = fopen(filepath, "wb");
	rapidjson::FileWriteStream fout(fp, write_buffer, sizeof(write_buffer));

	rapidjson::Writer<rapidjson::FileWriteStream> writer(fout);
	doc.Accept(writer);

	fclose(fp);
}

}