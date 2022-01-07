#include "circularBuffer.hpp"
#include <fstream>
#include <unistd.h> 
#include <thread>
#include <iostream>
#include <map>

void CircularBuffer::Initialize(int size_of_buffer, int max_num_of_writes_into_buffer, int num_of_producer_threads, int num_of_consumer_threads) {
	CircularBuffer::buffer = new int[size_of_buffer];
	CircularBuffer::size_of_buffer = size_of_buffer;
	CircularBuffer::max_num_of_writes_into_buffer = max_num_of_writes_into_buffer;
	CircularBuffer::num_of_consumer_threads = num_of_consumer_threads;
	CircularBuffer::num_of_producer_threads = num_of_producer_threads;
	CircularBuffer::buffer_head = 0; 
	CircularBuffer::buffer_tail = 1;
	CircularBuffer::item_to_be_consumed = 1;
	CircularBuffer::item_to_be_produced = 1;
}


void CircularBuffer::ProduceAndConsumeItems() {
	std::thread* producer_threads  = CreateProducerThreads();
	std::thread* consumer_threads = CreateConsumerThreads();
	JoinProducerAndConsumerThreads(producer_threads , consumer_threads);
}

void CircularBuffer::WriteToFile(int item, std::string file_name) {
	try {
		std::ofstream file(file_name, std::ios_base::app);
		file << item << "\n";
		file.close();
	} catch (...) {
		std::cout<<"file operation failed!!";
	}
}

void CircularBuffer::Produce () {
	while (true) {
		if (item_to_be_produced > max_num_of_writes_into_buffer) {
			return;
		}
		buffer_mutex.lock();
		// buffer is full
		if( buffer_head == buffer_tail ) {
			buffer_mutex.unlock();
			sleep(1);
			continue;
		}
		buffer[buffer_tail] = item_to_be_produced++;
		buffer_tail = (buffer_tail + 1) % size_of_buffer;
		buffer_mutex.unlock();
		sleep(1);
	}
}

void CircularBuffer::Consume(std::string thread_name) {
	while (true) {
		buffer_mutex.lock();
		if (item_to_be_consumed > max_num_of_writes_into_buffer) {
			buffer_mutex.unlock();
			return;
		}
		// buffer is empty
		if ((buffer_head + 1) % size_of_buffer == buffer_tail) {
			buffer_mutex.unlock();
			sleep(1); 
			continue;
		}
		buffer_head = ( buffer_head + 1 ) % size_of_buffer;
		WriteToFile(buffer[buffer_head], thread_name + ".txt");
		item_to_be_consumed += 1;
		buffer_mutex.unlock();
		sleep(1);
	}
}

std::thread* CircularBuffer :: CreateConsumerThreads() {
	std::thread* consumer_threads = new std::thread[num_of_consumer_threads];
	for (int i = 0; i < num_of_consumer_threads; i++) {
		consumer_threads[i] = std::thread(CircularBuffer::Consume, "thread_"+std::to_string(i));
	}
	return consumer_threads;
}

std::thread* CircularBuffer::CreateProducerThreads() {
	std::thread* producer_threads = new std::thread[num_of_producer_threads];
	for (int i = 0; i < num_of_producer_threads; i++) {
		producer_threads[i] = std::thread(CircularBuffer::Produce);
	}
	return producer_threads;
}

void CircularBuffer::JoinProducerAndConsumerThreads(std::thread* producer_threads, std::thread* consumer_threads) {
	for (int i = 0; i < num_of_producer_threads; i++) {
		producer_threads[i].join();	
	}
	for (int i = 0; i < num_of_consumer_threads; i++) {
		consumer_threads[i].join();	
	}
}


int* CircularBuffer::buffer;
std::mutex CircularBuffer::buffer_mutex;
int CircularBuffer::size_of_buffer;
int CircularBuffer::max_num_of_writes_into_buffer;
int CircularBuffer::num_of_consumer_threads;
int CircularBuffer::num_of_producer_threads;
int CircularBuffer::buffer_head; 
int CircularBuffer::buffer_tail;
int CircularBuffer::item_to_be_consumed;
int CircularBuffer::item_to_be_produced;