#include "temp_var_table.h"

namespace gcp {
	TempVarTable::TempVarTable() {

	}

	IdentiferTable::identifier TempVarTable::tempVar() {
		int index = table.size() + 1;
		IdentiferTable::identifier t;
		t.name = "T" + std::to_string(index);
		return t;
	}

	bool TempVarTable::addTempVar(IdentiferTable::identifier temp) {
		table.push_back(temp);
		return true;
	}

	void TempVarTable::Print() {
		std::cout << "---------��ʱ������---------" << std::endl;
		std::cout << "---------(��ʶ���� �� ���� �� ֵ)---------" << std::endl;
		for (int i = 0; i < table.size(); i++) {
			std::cout << "(" + table[i].name + " , " << table[i].type + " , " << table[i].value + ")" << std::endl;
		}
	}

	TempVarTable::~TempVarTable() {

	}
}