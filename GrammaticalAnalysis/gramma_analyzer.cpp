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
		std::cout << "[��] : ʶ�𵽵�һ������Ϊ��" + m_current_input.getTwoTuple() << std::endl;
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
			WriteToFile("[��] : ��" + type + "�����ͷ�¡�" + m_current_input.getType() + "��ƥ��ɹ�");
			std::cout << "[��] : ��" + type + "�����ͷ�¡�" + m_current_input.getType() + "��ƥ��ɹ�" << std::endl;
			m_current_input = LA.nextInput();
			std::cout << "[��] : ʶ����һ������Ϊ��" + m_current_input.getTwoTuple() << std::endl;
			return true;
		}
		std::cout << "[��] ERROR : ��" + type + "�����ͷ�¡�" + m_current_input.getType() + "������ͬ��ƥ��ʧ��" << std::endl;
		WriteToFile("[��] ERROR : ��" + type + "�����ͷ�¡�" + m_current_input.getType() + "������ͬ��ƥ��ʧ��");
		m_current_input = LA.nextInput();
		return false;
	}

	bool GrammaAnalyzer::P() {
		// P -> string I ; D
		WriteToFile("[��] : ��ʼ���� <����> �� <����˵������>;<��䲿��>");
		std::cout << "[��] : ��ʼ���� <����> �� <����˵������>;<��䲿��>" << std::endl;
		if (!match("string")) return false;
		I("string");
		if (!match("�ֺ�")) return false;
		D();
		WriteToFile("-------------------------�﷨��������--------------------");
		std::cout << "-------------------------�﷨��������--------------------" << std::endl;
		return true;
	}

	bool GrammaAnalyzer::I(string type) {
		// I -> i Ip 
		std::cout << "[��] : ��ʼ���� <��ʶ���б�> �� <��ʶ���б�>,<��ʶ��>|<��ʶ��>" << std::endl;
		string name = m_current_input.getValue();
		m_identifer_table.AddName(name);
		m_identifer_table.UpdateType(name, type);
		if (!match("��ʶ��")) return false;
		Ip(type);
		return true;
	}

	bool GrammaAnalyzer::D() {
		// D -> S ; Dp
		S();
		if (!match("�ֺ�")) return false;
		Dp();
		return true;
	}

	bool GrammaAnalyzer::Ip(string type) {
		// Ip -> , i Ip | ��
		if (m_current_input.getType() == "����") {
			// Ip -> , i Ip
			if (!match("����")) return false;
			string name = m_current_input.getValue();
			m_identifer_table.AddName(name);
			if (!match("��ʶ��")) return false;
			m_identifer_table.UpdateType(name, type);
			Ip(type);
			return true;
		}
		if (m_current_input.getType() == "�ֺ�") {
			// Ip -> ��
			return true;
		}
		return false;
	}

	bool GrammaAnalyzer::S() {
		// S -> A | C | L
		if (m_current_input.getType() == "��ʶ��") {
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
		// Dp -> S ; Dp | ��
		if (m_current_input.getType() == "��ʶ��" || m_current_input.getType() == "if" || m_current_input.getType() == "do") {
			S();
			if (!match("�ֺ�")) return false;
			Dp();
			return true;
		}
		if (m_current_input.getType() == "end") {
			// Dp -> ��
			return true;
		}
		if (m_current_input.getType() == "#") {
			WriteToFile("-------------------------�ʷ���������--------------------");
			std::cout << "-------------------------�ʷ���������--------------------" << std::endl;
			return true;
		}
		return false;
	}

	bool GrammaAnalyzer::A() {
		// A -> i = E
		string name = m_current_input.getValue();
		if (!match("��ʶ��")) return false;
		if (!match("��ֵ��")) return false;
		IdentiferTable::identifier E1 = E();
		m_identifer_table.UpdateValue(name, E1.value);
		m_middle_code_table.AddItem("=", E1.value, "null", name);
		std::cout << "[����] �� ������Ԫʽ" << std::endl;
		return true;
	}

	bool GrammaAnalyzer::C() {
		// C -> if ( F ) N else N 
		if (!match("if")) return false;
		if (!match("������")) return false;
		IdentiferTable::identifier F1 = F();

		int trueExit = m_middle_code_table.getNXQ() + 2;
		m_middle_code_table.AddItem("jnz", F1.name, "null", std::to_string(trueExit));
		int falseExit = m_middle_code_table.getNXQ();
		m_middle_code_table.AddItem("j", "null", "null", "0");
		if (!match("������")) return false;
		N();
		int Exit = m_middle_code_table.getNXQ();
		m_middle_code_table.AddItem("j", "null", "null", "0");
		m_middle_code_table.backPath(falseExit, std::to_string(m_middle_code_table.getNXQ()));
		if (!match("else")) return false;
		N();
		m_middle_code_table.backPath(Exit, std::to_string(m_middle_code_table.getNXQ()));
		return true;
	}

	bool GrammaAnalyzer::L() {
		// L -> do N while ( F )
		NEXTR : int nextP = LA.getPointer();
		if (!match("do")) return false;
		int NEXT = m_middle_code_table.getNXQ();
		N();
		if (!match("while")) return false;
		if (!match("������")) return false;
		IdentiferTable::identifier F1 = F();
		m_middle_code_table.AddItem("jnz", F1.name, "null", std::to_string(NEXT));
		if (!match("������")) return false;
		if (F1.value == "true") {
			LA.setPointer(nextP);
			goto NEXTR;
		}
		std::cout << "[����] �� ������Ԫʽ" << std::endl;
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

		match("��ϵ�����");
		IdentiferTable::identifier E2 = E();

		IdentiferTable::identifier temp = m_temp_var_table.tempVar();
		temp.type = "bool";
		temp.value = "false";
		if (operater == "<") {
			if (E1.value.length() < E2.value.length()) {
				temp.value = "true";
			}
			else {
				temp.value = "false";
			}
		}
		if (operater == "<=") {
			if (E1.value.length() <= E2.value.length()) {
				temp.value = "true";
			}
			else {
				temp.value = "false";
			}
		}
		if (operater == ">") {
			if (E1.value.length() > E2.value.length()) {
				temp.value = "true";
			}
			else {
				temp.value = "false";
			}
		}
		if (operater == ">=") {
			if (E1.value.length() >= E2.value.length()) {
				temp.value = "true";
			}
			else {
				temp.value = "false";
			}
		}
		if (operater == "==") {
			if (E1.value == E2.value) {
				temp.value = "true";
			}
			else {
				temp.value = "false";
			}
		}
		if (operater == "<>") {
			if (E1.value != E2.value) {
				temp.value = "true";
			}
			else {
				temp.value = "false";
			}
		}
		m_middle_code_table.AddItem(operater, E1.name, E2.name, temp.name);
		std::cout << "[����] �� ������Ԫʽ" << std::endl;
		return temp;
	}

	bool GrammaAnalyzer::N() {
		// N -> S | M
		if (m_current_input.getType() == "��ʶ��" || m_current_input.getType() == "if" || m_current_input.getType() == "do") {
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
		// Ep -> + G Ep | ��
		if (m_current_input.getType() == "���������") {
			match("���������");
			IdentiferTable::identifier tempG = G();
			IdentiferTable::identifier temp = m_temp_var_table.tempVar();
			temp.type = tempId.type;
			temp.value = tempId.value.substr(0, tempId.value.length() - 2) + tempG.value.substr(1, tempG.value.length() - 1);
			m_temp_var_table.addTempVar(temp);
			m_middle_code_table.AddItem("+", tempId.name != "" ?tempId.name : tempId.value, tempG.name != "" ? tempG.name : tempG.value, temp.name);
			std::cout << "[����] �� ������Ԫʽ" << std::endl;
			IdentiferTable::identifier tempEp = Ep(temp);
			return tempEp;
		}
		if (m_current_input.getType() == "��ϵ�����" || m_current_input.getType() == "�ֺ�" || m_current_input.getType() == "������") {
			// Ep -> ��
			return tempId;
		}
		return tempId;
	}

	IdentiferTable::identifier GrammaAnalyzer::Y() {
		// Y -> i | s | ( E )
		if (m_current_input.getType() == "��ʶ��") {
			string name = m_current_input.getValue();
			match("��ʶ��");
			return m_identifer_table.getIdentifer(name);
		}
		if (m_current_input.getType() == "�ַ���") {
			string value = m_current_input.getValue();
			match("�ַ���");
			IdentiferTable::identifier tempid;
			tempid.value = value;
			tempid.type = "string";
			return tempid;
		}
		if (m_current_input.getType() == "������") {
			match("������");
			IdentiferTable::identifier tempE = E();
			match("������");
			return tempE;
		}
		IdentiferTable::identifier null;
		return null;
	}

	IdentiferTable::identifier GrammaAnalyzer::Gp(IdentiferTable::identifier tempIn) {
		// Gp -> j n Gp | ��
		if (m_current_input.getType() == "�ظ������") {
			match("�ظ������");
			IdentiferTable::identifier tempId;
			tempId.value = m_current_input.getValue();
			match("����");
			IdentiferTable::identifier temp = m_temp_var_table.tempVar();
			temp.type = tempIn.type;
			temp.value = tempIn.value;
			for (int i = 0; i < std::stoi(tempId.value); i++) {
				temp.value = temp.value.substr(0, temp.value.length() - 2) + temp.value.substr(1, temp.value.length()-1);
			}
			m_temp_var_table.addTempVar(temp);
			m_middle_code_table.AddItem("*", tempIn.name,tempId.value, temp.name);
			std::cout << "[����] �� ������Ԫʽ" << std::endl;
			IdentiferTable::identifier Gp1 = Gp(temp);
			return Gp1;
		}
		if (m_current_input.getType() == "���������") {
			return tempIn;
		}
		return tempIn;
	}

	bool GrammaAnalyzer::M() {
		// M -> start D end
		if (!match("start")) return false;
		D();
		if (!match("end")) return false;
	 	return true;
	}

	GrammaAnalyzer::~GrammaAnalyzer()
	{

	}
}