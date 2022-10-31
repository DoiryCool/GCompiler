#ifndef MIDDLE_CODE_TABLE_H_
#define MIDDLE_CODE_TABLE_H_

#include <iostream>
#include <string>
#include <vector>
#include "four_tuple.h"

namespace gcp {
	class MiddleCodeTable {
	public:
		MiddleCodeTable();
		~MiddleCodeTable();

		bool AddItem(string op1, string op2, string op3, string op4);
		int getNXQ();
		bool backPath(int index, string op4);
		void Print();

	private:
		std::vector<FourTuple> table;
		int NXQ;
	};


}
#endif // !MIDDLE_CODE_TABLE_H_