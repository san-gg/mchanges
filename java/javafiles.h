#pragma once
#ifndef __FILE_ITERATOR
#define __FILE_ITERATOR

#include <filesystem>
#include <fstream>
#include <string>
namespace stdfs = std::filesystem;

class java_files {
public:
	class java_file_iterator {
	public:
		using iterator_category = std::input_iterator_tag;
		using value_type = std::string;
		using difference_type = ptrdiff_t;
		using pointer = const std::string*;
		using reference = const std::string&;
		java_file_iterator() { }
		explicit java_file_iterator(std::string& dir) {
			this->data.reserve(4096);
			this->ditr = stdfs::recursive_directory_iterator(dir);
			all_java_path();
		}
		java_file_iterator(std::string& dir, std::string& list) {
			this->data.reserve(4096);
			this->ditr = stdfs::recursive_directory_iterator();
			this->parentDir = dir;
			this->list.open(list);
			java_path_from_list();
		}
		~java_file_iterator() {
			this->list.close();
			this->data.clear();
		}
		void operator++() {
			this->data.clear();
			if (this->ditr != stdfs::recursive_directory_iterator()) all_java_path();
			else if (this->list.is_open()) java_path_from_list();
		}
		void operator++(int) { this->data.clear(); ++(*this); }
		reference operator*() { return this->data; }
		bool operator==(java_file_iterator& other) const { return this->data.compare(other.data) == 0; }
		bool operator!=(java_file_iterator& other) const { return this->data.compare(other.data) != 0; }
		//pointer operator->() { return &this->data; }
	private:
		void all_java_path() {
			auto endItr = stdfs::recursive_directory_iterator();
			while (this->ditr != endItr) {
				if (!(*this->ditr).is_directory() && (*this->ditr).is_regular_file()) {
					const std::string& tmp = (*this->ditr).path().generic_string();
					if (tmp.find(".java") != -1) {
						this->data.assign(tmp);
						break;
					}
				}
				this->ditr++;
			}
			if (this->ditr != endItr) this->ditr++;
		}
		void java_path_from_list() {
			std::string tmp;
			while (std::getline(this->list, tmp)) {
				if (stdfs::exists(this->parentDir + tmp)) {
					this->data = this->parentDir + tmp;
					break;
				}
			}
		}
		std::ifstream list;
		std::string data;
		std::string parentDir;
		stdfs::recursive_directory_iterator ditr;
	};
	java_files(const std::string& dir) : baseDir(dir), isCustomList(false) { }
	void setCustomList(const std::string& list) {
		this->isCustomList = true;
		if (stdfs::exists(list)) this->customList.assign(list);
		else throw std::exception("custom list file not found");
	}
	java_file_iterator begin() {
		if(this->isCustomList) return java_file_iterator(this->baseDir, this->customList);
		return java_file_iterator(this->baseDir);
	}
	java_file_iterator end() { return java_file_iterator(); }
private:
	std::string baseDir;
	std::string customList;
	bool isCustomList;
};

#endif // !__FILE_ITERATOR

