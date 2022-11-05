#include "middle_code_table.h"

namespace gcp {
	MiddleCodeTable::MiddleCodeTable() {

	}

	bool MiddleCodeTable::AddItem(string op1, string op2, string op3, string op4) {
		table.push_back(FourTuple(op1, op2, op3, op4));
		return true;
	}

	int MiddleCodeTable::getNXQ() {
		return table.size();
	}

	bool MiddleCodeTable::backPath(int index, string op4) {
		table[index].updateOp4(op4);
		return true;
	}

	void MiddleCodeTable::Print() {
		std::ofstream dfs("debug.txt", std::ios::app);
		dfs << "---------�м�����---------" << std::endl;
		for (int i = 0; i < table.size(); i++) {
			dfs << "(" << i << ")" + table[i].getFourTuple() << std::endl;
		}
		dfs.close();
	}

	MiddleCodeTable::~MiddleCodeTable() {

	}
}