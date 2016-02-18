#pragma once

#include <diskscan/filescan.hpp>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include <string>
#include <iostream>

namespace diskscan
{

class cli
{
private:
	struct cli_options
	{
		std::string path;
		size_t sector_size;
		bool silent = false;
	};

	static int read_options(cli_options& opt, int argc, char** argv)
	{
		boost::program_options::options_description o_general("Options");
		o_general.add_options()
				("help,h", "display this message")
				("silent,s", "do not print progress")
				("sectorsize,S", boost::program_options::value(&opt.sector_size), "size of sectors to scan for (default: 4096)")
				("path,p", boost::program_options::value(&opt.path), "path which to process (default: .)");

		boost::program_options::variables_map vm;
		boost::program_options::positional_options_description pos;

		boost::program_options::options_description options("Allowed options");
		options.add(o_general);

		try
		{
			boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(options).positional(pos).run(), vm);
		} catch(boost::program_options::unknown_option &e)
		{
			std::cerr << "Unknown option --" << e.get_option_name() << ", see --help." << std::endl;
			return EXIT_FAILURE;
		}

		try
		{
			boost::program_options::notify(vm);
		} catch(const boost::program_options::required_option &e)
		{
			std::cerr << "You forgot this: " << e.what() << std::endl;
			return EXIT_FAILURE;
		}

		if(vm.count("help"))
		{
			std::cout
				<< "Disk scan utility. [https://github.com/Wassasin/diskscan]" << std::endl
				<< "Usage: ./diskscan [options]" << std::endl
				<< std::endl
				<< o_general;

			return EXIT_FAILURE;
		}

		if(vm.count("silent"))
			opt.silent = true;

		if(!vm.count("path"))
			opt.path = ".";

		if(!vm.count("sectorsize"))
			opt.sector_size = 4096;

		return EXIT_SUCCESS;
	}

public:
	cli() = delete;

	static int exec(int argc, char** argv)
	{
		cli_options opt;

		int result = read_options(opt, argc, argv);
		if(result != EXIT_SUCCESS)
			return result;

		boost::filesystem::recursive_directory_iterator di(opt.path), di_end;
		for(; di != di_end; ++di)
		{
			auto p = di->path();
			if(boost::filesystem::is_regular_file(p))
			{
				std::string p_str = p.string();
				filescan::result_t r = filescan::exec(p_str, opt.sector_size);

				if(r.empty_sectors > 0)
					std::cout << p_str << std::endl;

				if(!opt.silent)
				{
					float ratio = std::round(((float)r.empty_sectors / (float)r.sectors) * 1000.0f) / 1000.0f;
					std::cerr << r.empty_sectors << '\t' << r.sectors << '\t' << ratio << '\t' << p_str << std::endl;
				}
			}
		}

		return EXIT_SUCCESS;
	}
};

}
