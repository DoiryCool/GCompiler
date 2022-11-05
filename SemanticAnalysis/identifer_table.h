#ifndef IDENTIFY_TABLE_H_
#define IDENTIFY_TABLE_H_

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

namespace gcp {
	class IdentiferTable {

	private:
		

	public:
		IdentiferTable();
		~IdentiferTable();

		struct identifier {
			std::string name = "";
			std::string type = "";
			std::string value = "";
		};

		int GetIndex(std::string name);
		bool UpdateType(std::string name, std::string type);
		bool UpdateValue(std::string name, std::string value);
		bool AddName(std::string name);
		identifier getIdentifer(std::string name);
		void Print();

	

		std::vector< identifier> table;
	};


}
#endif // !IDENTIFY_TABLE_H_