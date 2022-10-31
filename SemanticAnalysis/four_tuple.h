#ifndef FOUR_TUPLE_H_
#define FOUR_TUPLE_H_

#include <iostream>
#include <string>
#include <fstream>

using std::string;

namespace gcp {

	class FourTuple {
	private:
		string op1;
		string op2;
		string op3;
		string op4;
	public:
		FourTuple(string op1, string op2, string op3, string op4);
		~FourTuple();

		string getFourTuple();
		bool updateOp4(string op4);
		
	};

}
#endif