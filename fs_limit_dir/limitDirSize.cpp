#include<filesystem>
#include<algorithm>
#include<iostream>

#include "limitDirSize.hpp"

LimitDirSize::LimitDirSize(){}

LimitDirSize::LimitDirSize(int size , std::filesystem::path root_path_) 
	: given_dir_size(size) , root_path(root_path_) {}

std::vector<std::filesystem::path> LimitDirSize::FilePaths() {
	std::vector<std::filesystem::path> file_paths;
	for (auto const& dir_entry: std::filesystem::recursive_directory_iterator{root_path}) {
		if (is_regular_file(dir_entry) && std::filesystem::path(dir_entry).extension() != "") {
			file_paths.push_back(dir_entry);
		}
	}
	return file_paths;
}

int LimitDirSize::DirSize() {
	int dir_size = 0;
	for (auto const& dir_entry: std::filesystem::recursive_directory_iterator{root_path}) {
		if (is_regular_file(dir_entry) && std::filesystem::path(dir_entry).extension() != "") {
			uintmax_t size = file_size(dir_entry);
			dir_size += size;
		}
	}
	return dir_size;
}

std::vector<long long> LimitDirSize::DeleteFiles() {
	std::vector<std::filesystem::path> file_paths = FilePaths();	
	
	std::vector<long long> modification_time_of_deleted_files;

	// Return number of seconds of last modification time since epoch time 
	auto LastWriteTimeSinceEpoch = [](const std::filesystem::path file_path) {
		auto ftime = std::filesystem::last_write_time(file_path);
		auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(ftime);
		auto value = now_ms.time_since_epoch();
		return value.count();
	};

	// vector sortened according to last write time since epoch
	std::sort(file_paths.begin(), file_paths.end(), [&](const std::filesystem::path file1, const std::filesystem::path file2) {
 	 	return LastWriteTimeSinceEpoch(file1) < LastWriteTimeSinceEpoch(file2);
	});

	int curr_dir_size = DirSize();

	for (auto f : file_paths) {
		if(curr_dir_size > given_dir_size){
			curr_dir_size -= file_size(f);
			modification_time_of_deleted_files.push_back(LastWriteTimeSinceEpoch(f));
			remove(f);
		}
		else{
			break;
		}
	}
	return modification_time_of_deleted_files;
}