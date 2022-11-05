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
		std::ofstream ofs("out.txt", std::ios::app);
		std::ofstream dfs("debug.txt", std::ios::app);
		//std::cout << "---------��ʱ������---------" << std::endl;
		ofs << "---------��ʱ������---------" << std::endl;
		dfs << "---------��ʱ������---------" << std::endl;
		//std::cout << "---------(��ʶ���� �� ���� �� ֵ)---------" << std::endl;
		ofs << "---------(��ʶ���� �� ���� �� ֵ)---------" << std::endl;
		dfs << "---------(��ʶ���� �� ���� �� ֵ)---------" << std::endl;
		for (int i = 0; i < table.size(); i++) {
			//std::cout << "(" + table[i].name + " , " << table[i].type + " , " << table[i].value + ")" << std::endl;
			ofs << "(" + table[i].name + " , " << table[i].type + " , " << table[i].value + ")" << std::endl;
			dfs << "(" + table[i].name + " , " << table[i].type + " , " << table[i].value + ")" << std::endl;
		}
		ofs.close();
		dfs.close();
	}

	TempVarTable::~TempVarTable() {

	}
}