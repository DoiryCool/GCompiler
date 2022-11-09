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
		tempIden.IN = IN++;
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
		std::ofstream dfs("debug.txt", std::ios::app);
		dfs << "-------(标识符名 , 类型 , 值)-------" << std::endl;
		for (int i = 0; i < table.size(); i++) {
			dfs << "(" + table[i].name + " , " << table[i].type + " , " << table[i].value + ")" << std::endl;
		}
		dfs.close();

	}

	IdentiferTable::~IdentiferTable() {

	}
}