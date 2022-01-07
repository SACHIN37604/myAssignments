#include <gtest/gtest.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <filesystem>
#include "circularBuffer.hpp"


struct Test : public testing::Test {
	int *hash , m , n , c;
	void SetUp() {
		m = 8;
		n = 50;
		c = 5;
		CircularBuffer::Initialize(m , n , 1 , c);
		CircularBuffer::ProduceAndConsumeItems();
		hash = new int[n + 1];
		for (int i = 1; i <= n; i++) {
			hash[i] = 0;
		}
	}
	void TearDown() { delete hash; }
};

TEST_F (Test, CheckDuplicates) {
	
	for (int i = 0; i < n; i++) {
		std::string file_name = "thread_"+std::to_string(i)+".txt";
		std::ifstream infile; 
		infile.open(file_name); 
		while (!infile.eof()) {
			std::string s;
           	std::getline( infile, s );
			if (!s.size()) {
				break;
			}
			int data = stoi( s );
		   	if (hash[data]) {
				ASSERT_FALSE(hash[data]);
		   	}
		   	hash[data] = 1;
        }   
        infile.close();
		remove(std::filesystem::path(file_name)); 
	}
	ASSERT_EQ(1 , 1);
}


TEST_F (Test , CheckForAllNumbers) {
	
	for (int i = 0; i < n; i++) {
		std::string file_name = "thread_"+std::to_string(i)+".txt";
		std::ifstream infile; 
		infile.open(file_name); 
		while (!infile.eof()) {
			std::string s;
           	std::getline( infile, s );
			if (!s.size()) {
				break;
			}
			int data = stoi(s);
		   	hash[data] += 1;
        }   
        infile.close(); 
	}

	for (int i = 1; i <= n; i++) {
		ASSERT_TRUE(hash[i] == 1);
	}

	ASSERT_EQ(1 , 1);
}