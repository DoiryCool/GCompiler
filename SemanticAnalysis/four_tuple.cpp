#include "four_tuple.h"

namespace gcp {
	FourTuple::FourTuple(string op1, string op2, string op3, string op4) {
		this->op1 = op1;
		this->op2 = op2;
		this->op3 = op3;
		this->op4 = op4;
	}

	bool FourTuple::updateOp4(string op4) {
		this->op4 = op4;
		return true;
	}

	string FourTuple::getFourTuple() {
		return "(" + op1 + "," + op2 + "," + op3 + "," + op4 + ")";
	}

	FourTuple::~FourTuple() {

	}
}