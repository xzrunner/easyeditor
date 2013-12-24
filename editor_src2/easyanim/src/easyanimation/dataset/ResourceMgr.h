#pragma once

namespace eanim
{
	class ResourceMgr
	{
	public:
		struct Item
		{
			std::string path;
			std::string name;
		};

		std::vector<Item> items;
		int choice;

	public:
		ResourceMgr() : choice(0) {}

		void clear() {
			items.clear();
			choice = 0;
		}

		bool empty() const { return items.empty(); }

		const std::string& dir() const { return items[choice].path; }

	}; // ResourceMgr
}