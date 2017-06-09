#ifndef BOOST_LIMITS_HPP
#define BOOST_LIMITS_HPP

#include <string>
#include <cstdlib>

namespace boost
{

namespace limits
{

/**
 * Sets the maximum number of threads which can be created to \p count for the current process.
 * \param count The maximum number of threads which can be created for the current process.
 * \return Returns true if the assignment succeeded. Otherwise it returns false.
 */
bool set_maximum_threads(std::size_t count);

/**
 * Gets the maximum number of threads which can be created for the current process.
 * \param count This value be set to the maximum number of threads.
 * \return Returns true if the value has been set successfully. Otherwise it returns false.
 */
bool get_maximum_threads(std::size_t &count);

/**
 * Sets the maximum stack size for threads of the current process to \p count.
 * \param count Maximum stack size in bytes for threads of the current process.
 * \return Returns true if the assignment succeded. Otherwise it returns false.
 */
bool set_stack_size(std::size_t count);

/**
 * Gets the maximum stack size of the current process and its threads into \p count.
 * \param count This value will be set to the maximum stack size in bytes of the current process and its threads.
 * \return Returns true if the assignment succeded. Otherwise it returns false.
 */
bool get_stack_size(std::size_t &count);

/**
 * Gets the free memory on the system and writes it into \p count. This includes the free swap.
 * \param count This value will be set to the free memory in bytes.
 * \return Returns true if the assignment succeded. Otherwise it returns false.
 */
bool get_free_memory(std::size_t &count);

/**
 * Converts the value \p bytes to a readable value with its unit.
 * \param bytes The number of bytes which is converted into a readable string.
 * \return Returns a readable value if \p bytes with the biggest possible unit that is supported.
 *
 * For example if you pass 1024 you will get "1 KiByte".
 */
std::string value_as_biggest_unit(std::size_t bytes);

/**
 * Prints the current system and proces limits on std::cout.
 *
 * \return Returns true if retrieval succeded. Otherwise it returns false.
 */
bool print_limits();

}

}

#endif // BOOST_LIMITS_HPP