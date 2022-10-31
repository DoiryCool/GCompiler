#ifndef TWO_TUPLE_H_
#define TWO_TUPLE_H_

#include <iostream>
#include <string>
#include <fstream>

using std::string;
namespace gcp {

	class twoTuple {
	private:
		string m_type;
		string m_value;
	public:
		twoTuple();
		twoTuple(string, string);

		string getTwoTuple();

		void setType(string);
		string getType();
		void setValue(string);
		string getValue();
	};

}
#endif