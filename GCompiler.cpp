#include "GCompiler.h"

using std::cout;
using std::endl;

int main(int argc, char** argv) {
	string file = "in.txt";
	bool file_in = false;
	bool  mid_code_opt = false;
	for (int i = 1; i < argc; i++) {
		//cout << "argv[" << i << "]:" << argv[i] << endl;
		char* inputArg = argv[i];
		switch (inputArg[0]) {
			case '-': {
				switch (inputArg[1]) {
					case 'i': {
						file_in = true;
						break;
					}
					case 'v': {
						cout << "GCompiler Vision : 1.0.0-BETA" << endl;
						break;
					}
					case 'h': {
						cout << "-i\tChoose the input file." << endl;
						cout << "-v\tCheck the version." << endl;
						cout << endl;
						cout << "-O\tOpen the optimization." << endl;
						break;
					}
					case 'O': {
						mid_code_opt = true;
						break;
					}
				default:
					cout << "No such argument, you can use -h for help." << endl;
				}
				break;
			}
			default: {
				if (file_in) {
					file = inputArg;
					file_in = false;
				}
				break;
			}
		}
			
	}
	gcp::GrammaAnalyzer GA(file, mid_code_opt);
	GA.P();
	GA.m_identifer_table.Print();
	GA.m_middle_code_table.Print();
	GA.m_temp_var_table.Print();

}