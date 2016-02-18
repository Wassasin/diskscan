#pragma once

#include <string>
#include <fstream>
#include <memory>

namespace diskscan
{

class filescan
{
public:
	struct result_t
	{
		size_t size;

		size_t sectors;
		size_t empty_sectors;

		result_t()
			: size(0)
			, sectors(0)
			, empty_sectors(0)
		{}
	};

private:
	size_t sector_size;
	std::unique_ptr<char[]> buf;

	public:
	filescan(size_t _sector_size = 4096)
		: sector_size(_sector_size)
		, buf(new char[sector_size])
	{}

	result_t exec(std::string const& path)
	{
		result_t result;

		std::ifstream is(path, std::ifstream::binary | std::ifstream::ate);
		result.size = is.tellg();
		result.sectors = (result.size - 1) / sector_size + 1;
		is.seekg(0);

		for(size_t s = 0; s < result.sectors; ++s)
		{
			size_t current_sector_size = sector_size;
			if(s == result.sectors-1)
				current_sector_size = result.size % sector_size;

			is.read(buf.get(), current_sector_size);

			bool empty = true;
			for(size_t i = 0; i < current_sector_size; ++i)
			{
				if(buf[i] != 0)
				{
					empty = false;
					break;
				}
			}

			if(empty)
				result.empty_sectors++;
		}

		return result;
	}
};

}
