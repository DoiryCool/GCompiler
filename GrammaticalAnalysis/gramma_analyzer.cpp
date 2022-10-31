#include "gramma_analyzer.h"

namespace gcp {

	GrammaAnalyzer::GrammaAnalyzer()
	{
		std::ofstream ofs("out.txt", std::ios::end);
		std::ofstream dfs("debug.txt", std::ios::end);

		LA.getStringFromTxt("in.txt");
		m_current_input = LA.nextInput();
		// output
		ofs << m_current_input.getTwoTuple() << std::endl;
		std::cout << "[词] : 识别到第一个单词为：" + m_current_input.getTwoTuple() << std::endl;
	}

	bool GrammaAnalyzer::WriteToFile(string cont) {
		std::ofstream ofs("out.txt", std::ios::end);
		std::ofstream dfs("debug.txt", std::ios::end);
		ofs << cont << std::endl;
		dfs << cont << std::endl;
		ofs.close();
		dfs.close();
		return true;
	}

	bool GrammaAnalyzer::match(string type) {
		if (type == m_current_input.getType()) {
			WriteToFile("[语] : 【" + type + "】与读头下【" + m_current_input.getType() + "】匹配成功");
			std::cout << "[语] : 【" + type + "】与读头下【" + m_current_input.getType() + "】匹配成功" << std::endl;
			m_current_input = LA.nextInput();
			std::cout << "[词] : 识别到下一个单词为：" + m_current_input.getTwoTuple() << std::endl;
			return true;
		}
		std::cout << "[语] : 【" + type + "】与读头下【" + m_current_input.getType() + "】不相同，匹配失败" << std::endl;
		m_current_input = LA.nextInput();
		return false;
	}

	bool GrammaAnalyzer::P() {
		// P -> string I ; D
		std::cout << "[语] : 推导 <程序> → <变量说明部分>;<语句部分>" << std::endl;
		match("string");
		I("string");
		match("分号");
		D();
		return true;
	}

	bool GrammaAnalyzer::I(string type) {
		// I -> i Ip 
		std::cout << "[语] : 推导 <标识符列表> → <标识符列表>,<标识符>|<标识符>" << std::endl;
		string name = m_current_input.getValue();
		m_identifer_table.AddName(name);
		m_identifer_table.UpdateType(name, type);
		match("标识符");
		Ip(type);
		return true;
	}

	bool GrammaAnalyzer::D() {
		// D -> S ; Dp
		S();
		match("分号");
		Dp();
		return true;
	}

	bool GrammaAnalyzer::Ip(string type) {
		// Ip -> , i Ip | ε
		if (m_current_input.getType() == "逗号") {
			// Ip -> , i Ip
			match("逗号");
			string name = m_current_input.getValue();
			m_identifer_table.AddName(name);
			match("标识符");
			m_identifer_table.UpdateType(name, type);
			Ip(type);
			return true;
		}
		if (m_current_input.getType() == "分号") {
			// Ip -> ε
			return true;
		}
		return false;
	}

	bool GrammaAnalyzer::S() {
		// S -> A | C | L
		if (m_current_input.getType() == "标识符") {
			A();
			return true;
		}
		if (m_current_input.getType() == "if") {
			C();
			return true;
		}
		if (m_current_input.getType() == "do") {
			L();
			return true;
		}
		return false;
	}

	bool GrammaAnalyzer::Dp() {
		// Dp -> S ; Dp | ε
		if (m_current_input.getType() == "标识符" || m_current_input.getType() == "if" || m_current_input.getType() == "do") {
			S();
			match("分号");
			Dp();
			return true;
		}
		if (m_current_input.getType() == "#" || m_current_input.getType() == "end") {
			// Dp -> ε
			return true;
		}
		return false;
	}

	bool GrammaAnalyzer::A() {
		// A -> i = E
		string name = m_current_input.getValue();
		match("标识符");
		match("赋值号");
		IdentiferTable::identifier E1 = E();
		m_identifer_table.UpdateValue(name, E1.value);
		m_middle_code_table.AddItem("=", E1.value, "null", name);
		std::cout << "[语义] ： 产生四元式" << std::endl;
		return true;
	}

	bool GrammaAnalyzer::C() {
		// C -> if ( F ) N else N 
		match("if");
		match("左括号");
		IdentiferTable::identifier F1 = F();

		int trueExit = m_middle_code_table.getNXQ() + 2;
		m_middle_code_table.AddItem("jnz", F1.name, "null", std::to_string(trueExit));
		int falseExit = m_middle_code_table.getNXQ();
		m_middle_code_table.AddItem("j", "null", "null", "0");
		match("右括号");
		N();
		int Exit = m_middle_code_table.getNXQ();
		m_middle_code_table.AddItem("j", "null", "null", "0");
		m_middle_code_table.backPath(falseExit, std::to_string(m_middle_code_table.getNXQ()));
		match("else");
		N();
		m_middle_code_table.backPath(Exit, std::to_string(m_middle_code_table.getNXQ()));
		return true;
	}

	bool GrammaAnalyzer::L() {
		// L -> do N while ( F )
		match("do");
		N();
		match("while");
		match("左括号");
		F();
		match("右括号");
		return true;
	}

	IdentiferTable::identifier GrammaAnalyzer::E() {
		// E -> G Ep
		IdentiferTable::identifier tempG = G();
		IdentiferTable::identifier tempEp = Ep(tempG);
		return tempEp;
	}

	IdentiferTable::identifier GrammaAnalyzer::F() {
		// F -> E g E
		IdentiferTable::identifier E1 = E();
		string operater = m_current_input.getValue();

		match("关系运算符");
		IdentiferTable::identifier E2 = E();

		IdentiferTable::identifier temp = m_temp_var_table.tempVar();
		temp.type = "bool";
		m_middle_code_table.AddItem(operater, E1.name, E2.name, temp.name);
		//temp.value
		return temp;
	}

	bool GrammaAnalyzer::N() {
		// N -> S | M
		if (m_current_input.getType() == "标识符" || m_current_input.getType() == "if" || m_current_input.getType() == "do") {
			S();
			return true;
		}
		if (m_current_input.getType() == "start") {
			M();
			return true;
		}
		return false;
	}

	IdentiferTable::identifier GrammaAnalyzer::G() {
		// G -> Y Gp
		IdentiferTable::identifier tempY = Y();
		IdentiferTable::identifier tempGp = Gp(tempY);
		return tempGp;
	}

	IdentiferTable::identifier GrammaAnalyzer::Ep(IdentiferTable::identifier tempId) {
		// Ep -> + G Ep | ε
		if (m_current_input.getType() == "连接运算符") {
			match("连接运算符");
			IdentiferTable::identifier tempG = G();
			IdentiferTable::identifier temp = m_temp_var_table.tempVar();
			temp.type = tempId.type;
			temp.value = tempId.value.substr(0, tempId.value.length() - 2) + tempG.value.substr(1, tempG.value.length() - 1);
			m_temp_var_table.addTempVar(temp);
			m_middle_code_table.AddItem("+", tempId.name != "" ?tempId.name : tempId.value, tempG.name != "" ? tempG.name : tempG.value, temp.name);
			std::cout << "[语义] ： 产生四元式" << std::endl;
			IdentiferTable::identifier tempEp = Ep(temp);
			return tempEp;
		}
		if (m_current_input.getType() == "关系运算符" || m_current_input.getType() == "分号" || m_current_input.getType() == "右括号") {
			// Ep -> ε
			return tempId;
		}
		return tempId;
	}

	IdentiferTable::identifier GrammaAnalyzer::Y() {
		// Y -> i | s | ( E )
		if (m_current_input.getType() == "标识符") {
			string name = m_current_input.getValue();
			match("标识符");
			return m_identifer_table.getIdentifer(name);
		}
		if (m_current_input.getType() == "字符串") {
			string value = m_current_input.getValue();
			match("字符串");
			IdentiferTable::identifier tempid;
			tempid.value = value;
			tempid.type = "string";
			return tempid;
		}
		if (m_current_input.getType() == "左括号") {
			match("左括号");
			IdentiferTable::identifier tempE = E();
			match("右括号");
			return tempE;
		}
		IdentiferTable::identifier null;
		return null;
	}

	IdentiferTable::identifier GrammaAnalyzer::Gp(IdentiferTable::identifier tempIn) {
		// Gp -> j n Gp | ε
		if (m_current_input.getType() == "重复运算符") {
			match("重复运算符");
			IdentiferTable::identifier tempId;
			tempId.value = m_current_input.getValue();
			match("数字");
			IdentiferTable::identifier temp = m_temp_var_table.tempVar();
			temp.type = tempIn.type;
			temp.value = tempIn.value;
			for (int i = 0; i < std::stoi(tempId.value); i++) {
				temp.value = temp.value.substr(0, temp.value.length() - 2) + temp.value.substr(1, temp.value.length()-1);
			}
			m_temp_var_table.addTempVar(temp);
			m_middle_code_table.AddItem("*", tempIn.name,tempId.value, temp.name);
			std::cout << "[语义] ： 产生四元式" << std::endl;
			IdentiferTable::identifier Gp1 = Gp(temp);
			return Gp1;
		}
		if (m_current_input.getType() == "连接运算符") {
			return tempIn;
		}
		return tempIn;
	}

	bool GrammaAnalyzer::M() {
		// M -> start D end
		match("start");
		D();
		match("end");
	 	return true;
	}

	GrammaAnalyzer::~GrammaAnalyzer()
	{

	}
}