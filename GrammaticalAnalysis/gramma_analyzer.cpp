#include "gramma_analyzer.h"

namespace gcp {

	GrammaAnalyzer::GrammaAnalyzer() {
		LA.getStringFromTxt("in.txt");
		m_current_input = LA.nextInput();
		WriteToDebugFile(level + "[词] : 识别到第一个单词为：" + m_current_input.getTwoTuple());
	}

	bool GrammaAnalyzer::WriteToDebugFile(string cont) {
		std::ofstream dfs("debug.txt", std::ios::app);
		dfs << cont << std::endl;
		//std::cout << cont << std::endl;
		dfs.close();
		return true;
	}

	bool GrammaAnalyzer::WriteToOutFile(string cont) {
		std::ofstream cls("out.txt", std::ios::end);
		cls.close();
		std::ofstream ofs("out.txt", std::ios::app);
		ofs << cont << std::endl;
		//std::cout << cont << std::endl;
		ofs.close();
		return true;
	}

	bool GrammaAnalyzer::match(string type) {
		if (type == m_current_input.getType()) {
			WriteToDebugFile(level + "[语] : 【" + type + "】与读头下【" + m_current_input.getType() + "】匹配成功");
			m_current_input = LA.nextInput();
			WriteToDebugFile(level + "[词] : 识别到下一个单词为：" + m_current_input.getTwoTuple());
			return true;
		}
		WriteToDebugFile(level + "[语] ERROR : 【" + type + "】与读头下【" + m_current_input.getType() + "】不相同，匹配失败");
		m_current_input = LA.nextInput();
		return false;
	}

	bool GrammaAnalyzer::P() {
		// P -> string I ; D
		WriteToDebugFile(level + "[语] : <程序> → <变量说明部分>;<语句部分>");
		if (!match("string")) return false;
		m_output_program += "string ";
		WriteToDebugFile(level + "[语] : <变量说明部分> → [string]<标识符列表>");
		level += "----";
		if (!I("string")) return false;
		level = level.substr(0, level.length() - 4);
		if (!match("分号")) return false;
		m_output_program += ";\n";
		if (!D()) return false;
		WriteToDebugFile(level + "[done][语] : <程序> → <变量说明部分>;<语句部分>");
		WriteToOutFile(m_output_program);
		WriteToDebugFile("-------------------------语法分析结束--------------------");
		return true;
	}

	bool GrammaAnalyzer::I(string type) {
		// I -> i Ip 
		WriteToDebugFile(level + "[语] : <标识符列表> → <标识符列表><标识符列表prime>");
		string name = m_current_input.getValue();
		if (!match("标识符")) return false;
		m_identifer_table.AddName(name);
		m_identifer_table.UpdateType(name, type);
		m_output_program += name;
		WriteToDebugFile(level + "[翻] : 填写标识符 【" + name + "】类型为" + type);
		level += "----";
		if (!Ip(type)) return false;
		level = level.substr(0, level.length() - 4);
		return true;
	}

	bool GrammaAnalyzer::D() {
		// D -> S ; Dp
		WriteToDebugFile(level + "[语] : <语句部分> → <语句><语句部分prime>");
		level += "----";
		if (!S()) return false;
		level = level.substr(0, level.length() - 4);
		if (!match("分号")) return false;
		m_output_program += ";\n";
		if (!Dp()) return false;
		return true;
	}

	bool GrammaAnalyzer::Ip(string type) {
		// Ip -> , i Ip | ε
		if (m_current_input.getType() == "逗号") {
			// Ip -> , i Ip
			WriteToDebugFile(level + "[语] : 选择 <标识符列表prime> →,<标识符列表><标识符列表prime>");
			if (!match("逗号")) return false;
			m_output_program += ", ";
			string name = m_current_input.getValue();
			m_identifer_table.AddName(name);
			if (!match("标识符")) return false;
			m_output_program += name;
			WriteToDebugFile(level + "[翻] : 填写标识符 【" + name + "】类型为" + type);
			m_identifer_table.UpdateType(name, type);
			if (!Ip(type)) return false;
			return true;
		}
		if (m_current_input.getType() == "分号") {
			// Ip -> ε
			WriteToDebugFile(level + "[语] : 选择 <标识符列表prime> → ε");
			return true;
		}
		return false;
	}

	bool GrammaAnalyzer::S() {
		// S -> A | C | L
		if (m_current_input.getType() == "标识符") {
			// S -> A
			WriteToDebugFile(level + "[语] : 选择 <语句> → <赋值语句>");
			level += "----";
			if(!A()) return false;
			level = level.substr(0, level.length() - 4);
			return true;
		}
		if (m_current_input.getType() == "if") {
			// S -> C
			WriteToDebugFile(level + "[语] : 选择 <语句> → <条件语句>");
			level += "----";
			if (!C()) return false;
			level = level.substr(0, level.length() - 4);
			return true;
		}
		if (m_current_input.getType() == "do") {
			// S -> L
			WriteToDebugFile(level + "[语] : 选择 <语句> → <循环语句>");
			level += "----";
			if (!L()) return false;
			level = level.substr(0, level.length() - 4);
			return true;
		}
		return false;
	}

	bool GrammaAnalyzer::Dp() {
		// Dp -> S ; Dp | ε
		if (m_current_input.getType() == "标识符" || m_current_input.getType() == "if" || m_current_input.getType() == "do") {
			// Dp -> S ; Dp 
			WriteToDebugFile(level + "[语] : 选择 <语句部分prime> → <语句> ; <语句部分prime>");
			level += "----";
			if (!S()) return false;
			level = level.substr(0, level.length() - 4);
			if (!match("分号")) return false;
			m_output_program += ";\n";
			if (!Dp()) return false;
			return true;
		}
		if (m_current_input.getType() == "end" || m_current_input.getType() == "#") {
			// Dp -> ε
			WriteToDebugFile("-------------------------词法分析结束--------------------");
			// Dp -> ε
			WriteToDebugFile("[语] : 选择 <语句部分prime> → ε");
			WriteToDebugFile("-------------------------词法分析结束--------------------");
			std::cout << "-------------------------词法分析结束--------------------" << std::endl;
			return true;
		}
		return false;
	}

	bool GrammaAnalyzer::A() {
		// A -> i = E
		WriteToDebugFile(level + "[语] : <赋值语句> → <标识符> = <表达式>");
		string name = m_current_input.getValue();
		if (!match("标识符")) return false;
		m_output_program += outLevel + name;
		if (!match("赋值号")) return false;
		m_output_program += " = ";
		level += "----";
		IdentiferTable::identifier E1 = E();
		if (E1.name == "ERROR") {
			return false;
		}
		level = level.substr(0, level.length() - 4);
		m_identifer_table.UpdateValue(name, E1.value);
		WriteToDebugFile(level + "[翻] : 更新标识符 【" + name + "】的值为"+ E1.value);
		m_middle_code_table.AddItem("=", E1.name != "" ? E1.name : E1.value, "null", name);
		WriteToDebugFile("[翻] ： 产生赋值语句四元式");
		WriteToDebugFile("[翻] : 更新标识符 【" + name + "】的值为"+ E1.value);
		m_middle_code_table.AddItem("=", E1.value, "null", name);
		WriteToDebugFile("[翻] ： 产生赋值语句四元式");
		return true;
	}

	bool GrammaAnalyzer::C() {
		// C -> if ( F ) N else N 
		WriteToDebugFile(level + "[语] :  if （<条件>）<嵌套语句> else <嵌套语句>");
		if (!match("if")) return false;
		m_output_program += outLevel + "if";
		if (!match("左括号")) return false;
		m_output_program += "(";
		level += "----";
		IdentiferTable::identifier F1 = F();
		if (F1.name == "ERROR") {
			return false;
		}
		level = level.substr(0, level.length() - 4);

		int trueExit = m_middle_code_table.getNXQ() + 2;
		m_middle_code_table.AddItem("jnz", F1.name, "null", std::to_string(trueExit));
		WriteToDebugFile(level + "[翻] ： 产生if语句真出口四元式");
		int falseExit = m_middle_code_table.getNXQ();
		m_middle_code_table.AddItem("j", "null", "null", "0");
		WriteToDebugFile(level + "[翻] ： 产生if语句假出口四元式");
		if (!match("右括号")) return false;
		m_output_program += ")\n";
		level += "----";
		outLevel += "  ";
		if (!N()) return false;
		outLevel = outLevel.substr(0, outLevel.length() - 2);
		level = level.substr(0, level.length() - 4);
		int Exit = m_middle_code_table.getNXQ();
		m_middle_code_table.AddItem("j", "null", "null", "0");
		WriteToDebugFile(level + "[翻] ： 产生if语句假出口四元式");
		m_middle_code_table.backPath(falseExit, std::to_string(m_middle_code_table.getNXQ()));
		if (!match("else")) return false;
		m_output_program += outLevel + "else\n";
		level += "----";
		outLevel += "  ";
		if (!N()) return false;
		outLevel = outLevel.substr(0, outLevel.length() - 2);
		level = level.substr(0, level.length() - 4);
		m_middle_code_table.backPath(Exit, std::to_string(m_middle_code_table.getNXQ()));
		return true;
	}

	bool GrammaAnalyzer::L() {
		// L -> do N while ( F )
		WriteToDebugFile(level + "[语] : do <嵌套语句> while （<条件>）");
		if (!match("do")) return false;
		m_output_program += outLevel + "do\n";
		int NEXT = m_middle_code_table.getNXQ();
		level += "----";
		outLevel += "  ";
		if (!N()) return false;
		outLevel = outLevel.substr(0, outLevel.length() - 2);
		level = level.substr(0, level.length() - 4);
		if (!match("while")) return false;
		m_output_program += outLevel + "while";
		if (!match("左括号")) return false;
		m_output_program += "(";
		level += "----";
		IdentiferTable::identifier F1 = F();
		if (F1.name == "ERROR") {
			return false;
		}
		level = level.substr(0, level.length() - 4);
		m_middle_code_table.AddItem("jnz", F1.name, "null", std::to_string(NEXT));
		WriteToDebugFile(level + "[翻] ： 产生while语句跳转四元式");
		if (!match("右括号")) return false;
		m_output_program += ")";
		return true;
	}

	IdentiferTable::identifier GrammaAnalyzer::E() {
		// E -> G Ep
		WriteToDebugFile(level + "[语] : <表达式> → <项><表达式prime>");
		IdentiferTable::identifier tempG = G();
		IdentiferTable::identifier tempEp = Ep(tempG);
		return tempEp;
	}

	IdentiferTable::identifier GrammaAnalyzer::F() {
		// F -> E g E
		WriteToDebugFile(level + "[语] : <条件> → <表达式><关系运算符><表达式>");
		level += "----";
		IdentiferTable::identifier E1 = E();
		level = level.substr(0, level.length() - 4);
		string operater = m_current_input.getValue();

		if (!match("关系运算符")) {
			return errorId;
		}
		m_output_program += " " + operater + " ";

		level += "----";
		IdentiferTable::identifier E2 = E();
		level = level.substr(0, level.length() - 4);

		IdentiferTable::identifier temp = m_temp_var_table.tempVar();
		temp.type = "bool";
		temp.value = "false";
		if (operater == "<") {
			WriteToDebugFile(level + "[翻] ： 产生小于四元式");
			if (E1.value.length() < E2.value.length()) {
				temp.value = "true";
			}
			else {
				temp.value = "false";
			}
		}
		if (operater == "<=") {
			WriteToDebugFile(level + "[翻] ： 产生小于等于四元式");
			if (E1.value.length() <= E2.value.length()) {
				temp.value = "true";
			}
			else {
				temp.value = "false";
			}
		}
		if (operater == ">") {
			WriteToDebugFile(level + "[翻] ： 产生大于四元式");
			if (E1.value.length() > E2.value.length()) {
				temp.value = "true";
			}
			else {
				temp.value = "false";
			}
		}
		if (operater == ">=") {
			WriteToDebugFile(level + "[翻] ： 产生大于等于四元式");
			if (E1.value.length() >= E2.value.length()) {
				temp.value = "true";
			}
			else {
				temp.value = "false";
			}
		}
		if (operater == "==") {
			WriteToDebugFile(level + "[翻] ： 产生等于四元式");
			if (E1.value == E2.value) {
				temp.value = "true";
			}
			else {
				temp.value = "false";
			}
		}
		if (operater == "<>") {
			WriteToDebugFile(level + "[翻] ： 产生不等于四元式");
			if (E1.value != E2.value) {
				temp.value = "true";
			}
			else {
				temp.value = "false";
			}
		}
		m_middle_code_table.AddItem(operater, E1.name, E2.name, temp.name);
		WriteToDebugFile("[翻] ： 产生关系运算四元式");
		return temp;
	}

	bool GrammaAnalyzer::N() {
		// N -> S | M
		if (m_current_input.getType() == "标识符" || m_current_input.getType() == "if" || m_current_input.getType() == "do") {
			// N -> S 
			WriteToDebugFile(level + "[语] : 选择 <嵌套语句> → <语句>");
			level += "----";
			if (!S()) return false;
			level = level.substr(0, level.length() - 4);
			m_output_program += "\n";
			return true;
		}
		if (m_current_input.getType() == "start") {
			// N -> M
			WriteToDebugFile(level + "[语] : 选择 <嵌套语句> → <复合语句>");
			level += "----";
			if (!M()) return false;
			level = level.substr(0, level.length() - 4);
			return true;
		}
		return false;
	}

	IdentiferTable::identifier GrammaAnalyzer::G() {
		// G -> Y Gp
		WriteToDebugFile(level + "[语] : <项> → <因子><项prime>");
		IdentiferTable::identifier tempY = Y();
		IdentiferTable::identifier tempGp = Gp(tempY);
		return tempGp;
	}

	IdentiferTable::identifier GrammaAnalyzer::Ep(IdentiferTable::identifier tempId) {
		// Ep -> + G Ep | ε
		if (m_current_input.getType() == "连接运算符") {
			// Ep -> + G Ep
			WriteToDebugFile(level + "[语] : 选择 <表达式prime> → <连接运算符><项><表达式prime>");
			string name = m_current_input.getValue();
			if (!match("连接运算符")) {
				return errorId;
			}
			m_output_program += " + ";
			level += "----";
			IdentiferTable::identifier tempG = G();
			level = level.substr(0, level.length() - 4);
			IdentiferTable::identifier temp = m_temp_var_table.tempVar();
			WriteToDebugFile(level + "[翻] : 创建连接运算临时变量");
			temp.type = tempId.type;
			temp.value = tempId.value + tempG.value;
			m_temp_var_table.addTempVar(temp);
			m_middle_code_table.AddItem("+", tempId.name != "" ?tempId.name : tempId.value, tempG.name != "" ? tempG.name : tempG.value, temp.name);
			WriteToDebugFile(level + "[翻] ： 产生连接运算四元式");
			IdentiferTable::identifier tempEp = Ep(temp);
			return tempEp;
		}
		if (m_current_input.getType() == "关系运算符" || m_current_input.getType() == "分号" || m_current_input.getType() == "右括号") {
			// Ep -> ε
			WriteToDebugFile(level + "[语] : 选择 <表达式prime> → ε");
			return tempId;
		}
		return tempId;
	}

	IdentiferTable::identifier GrammaAnalyzer::Y() {
		// Y -> i | s | ( E )
		if (m_current_input.getType() == "标识符") {
			// Y -> i
			WriteToDebugFile(level + "[语] : 选择 <因子> → <标识符>");
			string name = m_current_input.getValue();
			if (!match("标识符")) {
				return errorId;
			}
			m_output_program += "\"" + name + "\"";
			WriteToDebugFile(level + "[翻] : 获取表达式中标识符【" + name + "】值" + m_identifer_table.getIdentifer(name).value);
			return m_identifer_table.getIdentifer(name);
		}
		if (m_current_input.getType() == "字符串") {
			// Y -> s
			WriteToDebugFile(level + "[语] : 选择 <因子> → <字符串>");
			string value = m_current_input.getValue();
			if (!match("字符串")) {
				return errorId;
			}
			m_output_program += "\"" + value + "\"";
			IdentiferTable::identifier tempid;
			WriteToDebugFile(level + "[翻] : 创建字符串临时变量");
			tempid.value = value;
			tempid.type = "string";
			return tempid;
		}
		if (m_current_input.getType() == "左括号") {
			// Y -> i
			WriteToDebugFile(level + "[语] : 选择 <因子> → <左括号>");
			if (!match("左括号")) {
				return errorId;
			}
			m_output_program += "(";
			level += "----";
			IdentiferTable::identifier tempE = E();
			level = level.substr(0, level.length() - 4);
			if (!match("右括号")) {
				return errorId;
			}
			m_output_program += ")";
			return tempE;
		}
		IdentiferTable::identifier null;
		return null;
	}

	IdentiferTable::identifier GrammaAnalyzer::Gp(IdentiferTable::identifier tempIn) {
		// Gp -> * n Gp | ε
		if (m_current_input.getType() == "重复运算符") {
			// Gp -> j n Gp
			WriteToDebugFile("[语] : 选择 <项prime> → <重复运算符><数字><项prime>");
			match("重复运算符");
			IdentiferTable::identifier tempId;
			WriteToDebugFile(level + "[翻] : 创建重复运算临时变量");
			tempId.value = m_current_input.getValue();
			if (!match("数字")) {
				return errorId;
			}
			m_output_program += tempId.value;
			IdentiferTable::identifier temp = m_temp_var_table.tempVar();
			temp.type = tempIn.type;
			temp.value = tempIn.value;
			for (int i = 0; i < std::stoi(tempId.value) - 1; i++) {
				temp.value = temp.value + tempIn.value;
			}
			m_temp_var_table.addTempVar(temp);
			m_middle_code_table.AddItem("*", tempIn.name,tempId.value, temp.name);
			WriteToDebugFile(level + "[翻] ： 产生重复运算四元式");
			IdentiferTable::identifier Gp1 = Gp(temp);
			return Gp1;
		}
		if (m_current_input.getType() == "连接运算符") {
			// Gp -> ε
			WriteToDebugFile(level + "[语] : 选择 <项prime> → ε");
			return tempIn;
		}
		return tempIn;
	}

	bool GrammaAnalyzer::M() {
		// M -> start D end
		WriteToDebugFile("[语] : 选择 <项prime> → <重复运算符><数字><项prime>");
		if (!match("start")) return false;
		m_output_program += outLevel + "start\n";
		outLevel += "  ";
		level += "----";
		if (!D()) return false;
		level = level.substr(0, level.length() - 4);
		if (!match("end")) return false;
		outLevel = outLevel.substr(0, outLevel.length() - 2);
		m_output_program += outLevel + "end\n";
	 	return true;
	}

	GrammaAnalyzer::~GrammaAnalyzer()
	{
	}
}