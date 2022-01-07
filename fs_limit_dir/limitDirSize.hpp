#ifndef _limitDirSize_HPP_
#define _limitDirSize_HPP_

#include <vector>
#include <filesystem>


class LimitDirSize {
public:
	
	LimitDirSize();
	
	LimitDirSize(int , std::filesystem::path );
	
	/**
	 * @brief Recursively Traverse the directory and get the paths of all files
	 * 
	 * @return return vector of path of all files
	 */
	std::vector<std::filesystem::path> FilePaths();

	/**
	 * @param Calculates the size of the Directory
	 * 
	 * @return size of the directory
	 */
	int DirSize();
		
	/**
	 * @param Delete the files in a directory until size of directory becomes <= given size
	 * 
	 * @return vector of modification time of deleted files 
	*/
	std::vector<long long> DeleteFiles();

	~LimitDirSize();

private:
	// Maximum Directory size 
	int given_dir_size;

	// Directory Path
	std::filesystem::path root_path;
};

#endif