#ifndef LEXICAL_ANALYZER_H_
#define LEXICAL_ANALYZER_H_

#include "two_tuple.h"


using std::string;

namespace gcp {
	#define ifdebug true

	class LexicalAnalyzer {
	private:
		//FLAG
		int INPUT_STRING_FLAG;
		int POINTER;

		string m_input_string;

		twoTuple m_current_input;
	public:
		LexicalAnalyzer();
		~LexicalAnalyzer();

		void init();
		void getStringFromTxt(string);
		void writeStringToTxt(string);
		int getPointer();
		void setPointer(int POINTER);
		string correctKeyWord(string);
		string getString();

		twoTuple nextInput();
		void start();
	};
}


#endif