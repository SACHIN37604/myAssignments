#include <gtest/gtest.h>
#include <filesystem>

#include "limitDirSize.hpp"
#include "generate.hpp"


struct Test : public testing::Test {
	Generate *g;
	// creates the directory structure
	void SetUp() {
		int max_depth = 5;
		int num_of_char_in_file = 1000;
		int max_num_of_files = 5;
		g = new Generate(std::filesystem::path("root"), max_depth , num_of_char_in_file , max_num_of_files); 
		g->Create();
		std::cout << "created directory\n";
	}
	//destroys the directory structure
	void TearDown() {
		delete g;
	}
};

TEST_F (Test , CheckSize) {
	
	int given_dir_size = 1e4;
	
	auto dir_path = std::filesystem::path("root");

	LimitDirSize *limit_dir_size = new LimitDirSize(given_dir_size , dir_path);

	int before = limit_dir_size->DirSize();
	std::cout << "size before Deletion : " << before << "\n";

	limit_dir_size->DeleteFiles();

	int after = limit_dir_size->DirSize();
	std::cout << "size after Deletion : " << after << "\n";

	ASSERT_LE(after , before);

}

TEST_F (Test , OldestFileCheck) {
	
	int given_dir_size = 1e4;

	auto dir_path = std::filesystem::path("root");

	LimitDirSize *limit_dir_size = new LimitDirSize(given_dir_size , dir_path);

	auto modification_time_of_deleted_files = limit_dir_size->DeleteFiles();

	for (int i = 1 ; i < modification_time_of_deleted_files.size(); i++) {
		ASSERT_LE(modification_time_of_deleted_files[i - 1] , modification_time_of_deleted_files[i + 1]);
	}

	ASSERT_EQ(1 , 1);
}