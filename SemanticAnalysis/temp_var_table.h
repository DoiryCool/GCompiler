#ifndef TEMP_VAR_TABLE_H_
#define TEMP_VAR_TABLE_H_

#include <iostream>
#include <string>
#include <vector>
#include "four_tuple.h"
#include "identifer_table.h"

namespace gcp {
	class TempVarTable {
	public:
		TempVarTable();
		~TempVarTable();

		IdentiferTable::identifier tempVar();
		bool addTempVar(IdentiferTable::identifier temp); 
		void Print();
	private:
		std::vector<IdentiferTable::identifier> table;
	};


}
#endif // !MIDDLE_CODE_TABLE_H_