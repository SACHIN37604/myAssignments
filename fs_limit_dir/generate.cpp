#include <iostream>
#include <fstream>
#include <filesystem>
#include <random>
#include <math.h>

#include "generate.hpp"


// Generates random number 
std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

Generate::Generate() {}

Generate::Generate(std::filesystem::path _root , int _max_depth , int _max_num_of_char_in_file , int _max_num_of_files) 
	:root(_root),
	 max_depth(_max_depth),
	 max_num_of_char_in_file(_max_num_of_char_in_file),
	 max_num_of_files(_max_num_of_files) {}

int Generate::GenerateRandomNumber(int l , int r){
	return l + rng() % (r - l + 1);
}

std::string Generate::GetName(int number , char base){
	std::string res = "";
	while(number > 0){
		res += (number % 10) + base ;
		number /= 10;
	}
	return res;
}

char Generate::RandomChar(){
	return (rng() % 26) + 'a';
}

void Generate::Create(){
	create_directories(this->root);		
	this->MakeDir(this->root , max_depth);
}

std::string Generate::RandomString(){
	int len = GenerateRandomNumber(10 , max_num_of_char_in_file);
	std::string res = "";
	for(int i = 0; i < len; i++){
		res += RandomChar();
	}
	return res;	
}

void Generate::CreateFile(const std::filesystem::path file_path){

	std::ofstream fout(file_path);

	fout << RandomString();
	
	fout.close();
}

void Generate::MakeDir(const std::filesystem::path curr_path , int curr_depth){
	// Reaches the Maximum Depth
	if(curr_depth == 0){
		return ;
	}

	// Creates Directory 
	static int dir_num = 1;
	int num_of_dirs = GenerateRandomNumber(0 , curr_depth);
	for	(int i = 0; i < num_of_dirs; i++)	{
		std::string dir_name =  GetName(dir_num , 'A');
		dir_num++;
		std::filesystem::path new_dir_path = curr_path/dir_name;
		create_directories(new_dir_path);	
		
		//recursive call to MakeDir with (new directory path) and (maximum depth - 1)
		MakeDir(new_dir_path, curr_depth - 1);
	}
	
	// Creates files
	static int file_num = 10000;
	int num_of_files = GenerateRandomNumber(1 , max_num_of_files);
	for (int i = 0; i < num_of_files; i++) {
		std::string file_name = GetName(file_num , 'a') + ".txt";
		file_num++;
		CreateFile(curr_path/file_name);
	}
}

void Generate::RemoveDirectory() {
	std::uintmax_t n = std::filesystem::remove_all(this->root);
}

Generate::~Generate() {
	RemoveDirectory();
}