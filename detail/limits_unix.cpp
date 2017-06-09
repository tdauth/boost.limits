#include <iostream>
#include <cmath>
#include <sstream>
#include <cstring>
#include <cerrno>

/*
 * Required for sysinfo().
 */
#include <sys/sysinfo.h>

/*
 * Required for getrlmit()
 */
#include <sys/time.h>
#include <sys/resource.h>

/*
 * getlogin
 */
#include <unistd.h>

#include <boost/numeric/conversion/cast.hpp>

#include "../limits.hpp"

namespace boost
{

namespace limits
{

bool set_maximum_threads(std::size_t count)
{
	/*
	 * Gets the maximum number of threads which can be spawned
	 * to assign it with a different current number.
	 */
	struct rlimit rlim_nproc;

	if (getrlimit(RLIMIT_NPROC, &rlim_nproc) != 0)
	{
		return false;
	}

	rlim_nproc.rlim_cur = boost::numeric_cast<rlim_t>(count);

	if (setrlimit(RLIMIT_NPROC, &rlim_nproc) != 0)
	{
		return false;
	}

	if (getrlimit(RLIMIT_NPROC, &rlim_nproc) != 0)
	{
		return false;
	}

	return true;
}

bool get_maximum_threads(std::size_t &count)
{
	/*
	 * Gets the maximum number of threads which can be spawned.
	 */
	struct rlimit rlim_nproc;

	if (getrlimit(RLIMIT_NPROC, &rlim_nproc) != 0)
	{
		return false;
	}

	count = boost::numeric_cast<std::size_t>(rlim_nproc.rlim_cur);

	return true;
}

bool set_stack_size(std::size_t count)
{
	/**
	 * Each thread has a custom stack.
	 * When spawning many tasks using the thread backend each task uses the current stack size in memory.
	 * Therefore the number of tasks must be limited to not exceed the memory with the overall stack size.
	 *
	 * The current stack size for each thread is adjusted to \ref count.
	 */
	struct rlimit rlim_stack;

	if (getrlimit(RLIMIT_STACK, &rlim_stack) != 0)
	{
		return false;
	}

	rlim_stack.rlim_cur = boost::numeric_cast<rlim_t>(count);

	if (setrlimit(RLIMIT_STACK, &rlim_stack) != 0)
	{
		std::cerr << "Error on setting stack size to " << value_as_biggest_unit(count) << ": " << std::strerror(errno) << std::endl << "Upper limit is " << value_as_biggest_unit(rlim_stack.rlim_max) << std::endl;

		return false;
	}

	return true;
}

bool get_stack_size(std::size_t &count)
{
	struct rlimit rlim_stack;

	if (getrlimit(RLIMIT_STACK, &rlim_stack) != 0)
	{
		return false;
	}

	count = boost::numeric_cast<std::size_t>(rlim_stack.rlim_cur);

	return true;
}

bool get_free_memory(std::size_t &count)
{
	/*
	 * Get the maximum RAM.
	 * Through copy-on-write methods more might be useable but on 32 bit machines a limit of 3 GiBytes must be used.
	 *
	 * http://blog.scoutapp.com/articles/2010/10/06/determining-free-memory-on-linux
	 *
	 * The buffered memory can be included since the buffered stuff will be written on the disk if used for something else.
	 */
	struct sysinfo info;

	if (sysinfo(&info) != 0)
	{
		return false;
	}

	unsigned long result = info.freeram * info.mem_unit;
	result += info.freeswap * info.mem_unit;
	result += info.bufferram * info.mem_unit;

	count = boost::numeric_cast<std::size_t>(result);

	return true;
}

namespace
{

struct Unit
{
	std::string name;
	std::size_t size;
};

static const std::size_t maxUnits = 5;
static Unit units[maxUnits] =
{
	{"bytes", 1},
	{"KiBytes", 1024},
	{"MiBytes", 1024 * 1024},
	{"GiBytes", 1024 * 1024 * 1024},
};

}

std::string value_as_biggest_unit(std::size_t bytes)
{
	if (bytes == 0)
	{

		return "0 " + units[0].name;
	}

	std::ostringstream sstream;

	for (int i = maxUnits - 1; i >= 0; --i)
	{
		const long double result = ((long double)bytes) / ((long double)units[i].size);

		// TODO cut the values after the separator
		if ((std::size_t)result >= 1)
		{
			sstream << result << " " << units[i].name;

			break;
		}
	}

	const std::string result = sstream.str();

	return result;
}

bool print_limits()
{
	std::size_t freeMemory = 0;

	if (!get_free_memory(freeMemory))
	{
		return false;
	}

	struct rlimit rlim_stack;

	if (getrlimit(RLIMIT_STACK, &rlim_stack) != 0)
	{
		return false;
	}

	struct rlimit rlim_nproc;

	if (getrlimit(RLIMIT_NPROC, &rlim_nproc) != 0)
	{
		return false;
	}

	struct sysinfo info;

	if (sysinfo(&info) != 0)
	{
		return false;
	}

	std::cout << "===================== Limits =====================" << std::endl;
	std::cout << "Available memory (with swap): " << value_as_biggest_unit(info.totalram + info.totalswap) << std::endl;
	std::cout << "Free memory: " << value_as_biggest_unit(freeMemory) << std::endl;
	std::cout << "Current maximum stack size of process: " << value_as_biggest_unit(rlim_stack.rlim_cur) << std::endl;
	std::cout << "Maximum possible stack size: " << value_as_biggest_unit(rlim_stack.rlim_max) << std::endl;
	std::cout << "Current maximum number of threads: " << rlim_nproc.rlim_cur << std::endl;
	std::cout << "Maximum possible number of threads: " << rlim_nproc.rlim_max << std::endl;
	std::cout << "===================== Limits =====================" << std::endl;


	return true;
}

}

}