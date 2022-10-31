#include "two_tuple.h"

namespace gcp {

	twoTuple::twoTuple() {
	}

	twoTuple::twoTuple(string Type, string Value) {
		this->m_type = Type;
		this->m_value = Value;
	}

	string twoTuple::getTwoTuple() {
		return "(" + m_type + "," + m_value + ")";
	}

	void twoTuple::setType(string Type) {
		this->m_type = Type;
	}

	string twoTuple::getType() {
		return m_type;
	}

	void twoTuple::setValue(string Value) {
		this->m_value = Value;
	}

	string twoTuple::getValue() {
		return m_value;
	}
}