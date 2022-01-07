#ifndef _circularBuffer_HPP_
#define _circularBuffer_HPP_

#include <mutex>
#include <map>
#include <thread>
#include <unistd.h>
#include <iostream>
#include <fstream>


class CircularBuffer {
public:
	/**
	 * @brief initialize CicularBuffer
	 * 
	 * @param size_of_buffer 
	 * @param max_num_of_writes_into_buffer 
	 * @param num_of_producer_threads 
	 * @param number_of_consumer_threads 
	 */
	static void Initialize (int size_of_buffer, int max_num_of_writes_into_buffer, int num_of_producer_threads, int number_of_consumer_threads);
	/**
	 * @brief Produce and consume items
	 * 
	 */
	static void ProduceAndConsumeItems();
	
private:
	static int* buffer;
	static std::mutex buffer_mutex;
	static int size_of_buffer;
	static int num_of_consumer_threads;
	static int num_of_producer_threads;
	static int max_num_of_writes_into_buffer;
	static int buffer_head; 
	static int buffer_tail;
	
	// item to be consumed by consumer
	static int item_to_be_consumed; 
	
	// item to be produced by producer
	static int item_to_be_produced; 

	/**
	 * @brief write item into file
	 * 
	 * @param item item to write
	 * @param file_name name of the file
	 */
	static void WriteToFile (int item, std::string file_name);

	/**
	 * @brief Produce items
	 * 
	 */
	static void Produce();

	/**
	 * @brief Consume items
	 * 
	 * @param thread_name name of the consumer thread
	 */
	static void Consume(std::string thread_name);

	/**
	 * @brief Create a Consumer Threads object
	 * 
	 * @return std::thread* array of consumer threads
	 */
	static std::thread* CreateConsumerThreads();

	/**
	 * @brief Create a Producer Threads object
	 * 
	 * @return std::thread* array of producer threads
	 */
	static std::thread* CreateProducerThreads();	

	/**
	 * @brief join producer and consumer threads so that program waits untill all threads completes execution
	 * 
	 * @param producer_threads array of producer threads
	 * @param consumer_threads array of consumer threads
	 */
	static void JoinProducerAndConsumerThreads(std::thread* producer_threads, std::thread* consumer_threads);
};

#endif