#include "lexical_analyzer.h"

namespace gcp {

	LexicalAnalyzer::LexicalAnalyzer() {
		init();
	}

	void LexicalAnalyzer::init() {
		INPUT_STRING_FLAG = 0;									// Determine whether the file was successfully read,0 == success , 1 == fail
		POINTER = 0;
	}

	void LexicalAnalyzer::getStringFromTxt(string filePath) {
		std::ifstream ifs;
		ifs.open(filePath);
		getline(ifs, m_input_string);
		std::ofstream debugFile("debug.txt", std::ios::end);

		if (m_input_string != "") {
			m_input_string = m_input_string + "#";
			INPUT_STRING_FLAG = 1;
			debugFile << "success : Got the string from file \"" + filePath << "\"" << std::endl;
		}
		else debugFile << "failed : No such file in current direction." << std::endl;
		debugFile.close();
	}

	void LexicalAnalyzer::writeStringToTxt(string s) {
		std::ofstream ofs("debug.txt", std::ios::app);
		ofs << s << std::endl;
		ofs.close();
	}

	int LexicalAnalyzer::getPointer() {
		return POINTER;
	}

	void LexicalAnalyzer::setPointer(int POINTER) {
		this->POINTER = POINTER;
	}

	string LexicalAnalyzer::getString() {
		if (INPUT_STRING_FLAG == 1) return std::string(1, m_input_string[POINTER]);
		else return "failed : The string is NULL.";
	}

	string LexicalAnalyzer::correctKeyWord(string) {
		return "";
	}

	string LexicalAnalyzer::checkKeyWord(string Word) {
		return "";
	}

	twoTuple LexicalAnalyzer::nextInput() {
		string tempWord;
		string status = "0";

		while (m_input_string[POINTER] != '#') {
			char symbol = m_input_string[POINTER];
			if (symbol == ' ') { POINTER++; continue; }
			/*
			if ((symbol < 'a' || symbol > 'z') && (symbol < '0' || symbol > '9')
				&& symbol != '"' && symbol != '<' && symbol != '>' && symbol != '='
				&& symbol != ';' && symbol != ',' && symbol != '(' && symbol != ')' && symbol != '+' && symbol != '*') {
				writeStringToTxt("识别到与该文法不匹配的字符 : " + std::to_string(symbol));
				break;
			}*/
			if (status == "0") {
				// detect keywords
				if (symbol >= 'a' && symbol <= 'z') {
					switch (symbol) {
					case 's': {status = "0201"; tempWord += symbol; POINTER++; continue; }
					case 'i': {status = "0301"; tempWord += symbol; POINTER++; continue; }
					case 'd': {status = "0401"; tempWord += symbol; POINTER++; continue; }
					case 'w': {status = "0501"; tempWord += symbol; POINTER++; continue; }
					case 'e': {status = "0601"; tempWord += symbol; POINTER++; continue; }
					default: {status = "1"; tempWord += symbol; POINTER++; continue; }
					}
				}
				// detect number
				if (symbol >= '0' && symbol <= '9') {
					status = "9"; tempWord += symbol; POINTER++; continue;
				}
				// detect signals
				else {
					switch (symbol) {
					case '"': {status = "0901"; POINTER++; continue; }
					case '<': {status = "1001"; tempWord += symbol; POINTER++; continue; }
					case '>': {status = "1101"; tempWord += symbol; POINTER++; continue; }
					case '=': {status = "1201"; tempWord += symbol; POINTER++; continue; }
					case ';': {tempWord += symbol; POINTER++; return twoTuple("分号", tempWord); }
					case ',': {tempWord += symbol; POINTER++; return twoTuple("逗号", tempWord); }
					case '(': {tempWord += symbol; POINTER++; return twoTuple("左括号", tempWord); }
					case ')': {tempWord += symbol; POINTER++; return twoTuple("右括号", tempWord); }
					case '+': {tempWord += symbol; POINTER++; return twoTuple("连接运算符", tempWord); }
					case '*': {tempWord += symbol; POINTER++; return twoTuple("重复运算符", tempWord); }
					default: {status = 1; tempWord += symbol; POINTER++; continue; }
					}
				}
			}
			if (status == "1") {
				if ((symbol >= 'a' && symbol <= 'z') || (symbol >= '0' && symbol <= '9')) { tempWord += symbol; POINTER++; continue; }
				return twoTuple("标识符", tempWord);
			}
			if (status == "9") {
				if (symbol >= '0' && symbol <= '9') { tempWord += symbol; POINTER++; continue; }
				return twoTuple("数字", tempWord);
			}
			if (status == "0201") {
				if (symbol == 't') { status = "0202"; tempWord += symbol; POINTER++; continue; }
				if (symbol >= 'a' && symbol <= 'z') { status = "1"; tempWord += symbol; POINTER++; continue; }
				status = "1"; continue;
			}
			if (status == "0202") {
				if (symbol == 'r') { status = "0203"; tempWord += symbol; POINTER++; continue; }
				if (symbol == 'a') { status = "0703"; tempWord += symbol; POINTER++; continue; }
				if (symbol >= 'a' && symbol <= 'z') {
					status = "1"; tempWord += symbol; POINTER++; continue;
				}
				status = "1"; continue;
			}

			if (status == "0203") {
				if (symbol == 'i') { status = "0204"; tempWord += symbol; POINTER++; continue; }
				if (symbol >= 'a' && symbol <= 'z') { status = "1"; tempWord += symbol; POINTER++; continue; }
				status = "1"; continue;
			}
			if (status == "0204") {
				if (symbol == 'n') { status = "0205"; tempWord += symbol; POINTER++; continue; }
				if (symbol >= 'a' && symbol <= 'z') { status = "1"; tempWord += symbol; POINTER++; continue; }
				status = "1"; continue;
			}
			if (status == "0205") {
				if (symbol == 'g') { tempWord += symbol; POINTER++; return twoTuple("string", tempWord); }
				if (symbol >= 'a' && symbol <= 'z') { status = "1"; tempWord += symbol; POINTER++; continue; }
				status = "1"; continue;
			}
			if (status == "0703") {
				if (symbol == 'r') { status = "0704"; tempWord += symbol; POINTER++; continue; }
				if (symbol >= 'a' && symbol <= 'z') { status = "1"; tempWord += symbol; POINTER++; continue; }
				status = "1"; continue;
			}
			if (status == "0704") {
				if (symbol == 't') { tempWord += symbol; POINTER++; return twoTuple("start", tempWord); }
				if (symbol >= 'a' && symbol <= 'z') { status = "1"; tempWord += symbol; POINTER++; continue; }
				status = "1"; continue;
			}
			if (status == "0301") {
				if (symbol == 'f') { tempWord += symbol; POINTER++; return twoTuple("if", tempWord); }
				if (symbol >= 'a' && symbol <= 'z') { status = "1"; tempWord += symbol; POINTER++; continue; }
				status = "1"; continue;
			}
			if (status == "0401") {
				if (symbol == 'o') { tempWord += symbol; POINTER++; return twoTuple("do", tempWord); }
				if (symbol >= 'a' && symbol <= 'z') { status = "1"; tempWord += symbol; POINTER++; continue; }
				status = "1"; continue;
			}
			if (status == "0501") {
				if (symbol == 'h') { status = "0502"; tempWord += symbol; POINTER++; continue; }
				if (symbol >= 'a' && symbol <= 'z') { status = "1"; tempWord += symbol; POINTER++; continue; }
				status = "1"; continue;
			}
			if (status == "0502") {
				if (symbol == 'i') { status = "0503"; tempWord += symbol; POINTER++; continue; }
				if (symbol >= 'a' && symbol <= 'z') { status = "1"; tempWord += symbol; POINTER++; continue; }
				status = "1"; continue;
			}
			if (status == "0503") {
				if (symbol == 'l') { status = "0504"; tempWord += symbol; POINTER++; continue; }
				if (symbol >= 'a' && symbol <= 'z') { status = "1"; tempWord += symbol; POINTER++; continue; }
				status = "1"; continue;
			}
			if (status == "0504") {
				if (symbol == 'e') { tempWord += symbol; POINTER++; return twoTuple("while", tempWord); }
				if (symbol >= 'a' && symbol <= 'z') { status = "1"; tempWord += symbol; POINTER++; continue; }
				status = "1"; continue;
			}

			if (status == "0601") {
				if (symbol == 'n') { status = "0602"; tempWord += symbol; POINTER++; continue; }
				if (symbol == 'l') { status = "0802"; tempWord += symbol; POINTER++; continue; }
				if (symbol >= 'a' && symbol <= 'z') { status = "1"; tempWord += symbol; POINTER++; continue; }
				status = "1"; continue;
			}
			if (status == "0602") {
				if (symbol == 'd') { tempWord += symbol; POINTER++; return twoTuple("end", tempWord); }
				if (symbol >= 'a' && symbol <= 'z') { status = "1"; tempWord += symbol; POINTER++; continue; }
				status = "1"; continue;
			}
			if (status == "0802") {
				if (symbol == 's') { status = "0803"; tempWord += symbol; POINTER++; continue; }
				if (symbol >= 'a' && symbol <= 'z') { status = "1"; tempWord += symbol; POINTER++; continue; }
				status = "1"; continue;
			}
			if (status == "0803") {
				if (symbol == 'e') { tempWord += symbol; POINTER++; return twoTuple("else", tempWord); }
				if (symbol >= 'a' && symbol <= 'z') { status = "1"; tempWord += symbol; POINTER++; continue; }
				status = "1"; continue;
			}
			if (status == "0901") {
				if ((symbol >= 'a' && symbol <= 'z') || (symbol >= '0' && symbol <= '9')) { tempWord += symbol; POINTER++; continue; }
				else if (symbol == '"') { status = "0902"; continue; }
			}
			if (status == "0902") {
				POINTER++; return twoTuple("字符串", tempWord);
			}
			if (status == "1001") {
				if (symbol == '=') { tempWord += symbol; POINTER++; return twoTuple("关系运算符", tempWord); }
				if (symbol == '>') { tempWord += symbol; POINTER++; return twoTuple("关系运算符", tempWord); }
				return twoTuple("关系运算符", tempWord);
			}
			if (status == "1101") {
				if (symbol == '=') { tempWord += symbol; POINTER++; return twoTuple("关系运算符", tempWord); }
				return twoTuple("关系运算符", tempWord);
			}
			if (status == "1201") {
				if (symbol == '=') { tempWord += symbol; POINTER++; return twoTuple("关系运算符", tempWord); }
				return twoTuple("赋值号", tempWord);
			}
			status = "0";
			tempWord = "";
			POINTER++;
		}
		return twoTuple("#", "#");
	}

	LexicalAnalyzer::~LexicalAnalyzer() {

	}
}