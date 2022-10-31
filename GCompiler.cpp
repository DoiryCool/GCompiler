#include "GCompiler.h"

using std::cout;
using std::endl;

int main() {
	gcp::GrammaAnalyzer GA;
	GA.P();
	GA.m_identifer_table.Print();
	GA.m_middle_code_table.Print();
	GA.m_temp_var_table.Print();

}