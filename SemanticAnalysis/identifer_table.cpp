#include "identifer_table.h"

namespace gcp {
	IdentiferTable::IdentiferTable() {

	}

	int IdentiferTable::GetIndex(std::string name) {
		for (int i = 0; i < table.size(); i++) {
			if (table[i].name == name) return i;
		}
		return -1;
	}

	bool IdentiferTable::UpdateType(std::string name, std::string type) {
		int index = this->GetIndex(name);
		if (index == -1) return false;
		else table[index].type = type;
		return true;
	}

	bool IdentiferTable::UpdateValue(std::string name, std::string value) {
		int index = this->GetIndex(name);
		if (index == -1) return false;
		else table[index].value = value;
		return true;
	}

	bool IdentiferTable::AddName(std::string name) {
		for (int i = 0; i < table.size(); i++) {
			if (table[i].name == name) return false;
		}

		identifier tempIden;
		tempIden.name = name;
		table.push_back(tempIden);
		return true;
	}

	IdentiferTable::identifier IdentiferTable::getIdentifer(std::string name){
		for (int i = 0; i < table.size(); i++) {
			if (table[i].name == name) return table[i];
		}
		identifier null;
		return null;
	}

	void IdentiferTable::Print() {
		std::cout << "---------(标识符名 ， 类型 ， 值)---------" << std::endl;
		for (int i = 0; i < table.size(); i++) {
			std::cout << "(" + table[i].name + " , " << table[i].type + " , " << table[i].value + ")" << std::endl;
		}
	}

	IdentiferTable::~IdentiferTable() {

	}
}