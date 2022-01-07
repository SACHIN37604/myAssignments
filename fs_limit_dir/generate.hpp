#ifndef _generate_HPP_
#define _generate_HPP_

#include<iostream>
#include <fstream>
#include <filesystem>



class Generate {
	// Maximum number of files in any directory
	int max_num_of_files;
	
	// Maximum number of directories from root directory to leaf files 
	int max_depth;

	int max_num_of_char_in_file;

	// Path of root Directory 
	std::filesystem::path root;

public:

	Generate();

	Generate(std::filesystem::path, int, int, int);

	/**
	 * @brief Generates random number within range
	 *
	 * @param l left range of number
	 * @param r right range of number
	 * 
	 * @return random number
	 */
	int GenerateRandomNumber(int l, int r);

	/**
	 * @brief Converts number into string according to base
	 *
	 * @param number directory/file number 
	 * @param base 'A' or 'a' 
	 * 
	 * @return equivalent string of number    
	 */
	std::string GetName(int number, char base);

	/** 
	 * @brief Generates Random character
	 * 
	 */
	char RandomChar();
	
	/**
	 *  @brief Generates a Random string of length(>=10 and <= max_num_of_char_in_file
	 * 	 
	 *  @return random string
	 */
	std::string RandomString();

	/**
	 * @brief Write content in the file   
	 * 
	 * @param file_path path of the file 
	 */
	void CreateFile(const std::filesystem::path file_path);

	/**
	 * @brief Recursive function to create directory structure
	 *
	 * @param curr_path current path
	 * @param curr_depth (maximum depth - current depth from root)
	 */
	void MakeDir(const std::filesystem::path curr_path, int curr_depth);
	
	/**
	 * @brief Calling MakeDir function
	 * 
	 */
	void Create();

	/**
	 *  @brief Destroys the structure of the directory
	 * 
	 */
	void RemoveDirectory();
	
	~Generate();

};

#endif