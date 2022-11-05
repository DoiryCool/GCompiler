#include "gramma_analyzer.h"

namespace gcp {

	GrammaAnalyzer::GrammaAnalyzer() {
		LA.getStringFromTxt("in.txt");
		m_current_input = LA.nextInput();
		WriteToDebugFile(level + "[��] : ʶ�𵽵�һ������Ϊ��" + m_current_input.getTwoTuple());
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
			WriteToDebugFile(level + "[��] : ��" + type + "�����ͷ�¡�" + m_current_input.getType() + "��ƥ��ɹ�");
			m_current_input = LA.nextInput();
			WriteToDebugFile(level + "[��] : ʶ����һ������Ϊ��" + m_current_input.getTwoTuple());
			return true;
		}
		WriteToDebugFile(level + "[��] ERROR : ��" + type + "�����ͷ�¡�" + m_current_input.getType() + "������ͬ��ƥ��ʧ��");
		m_current_input = LA.nextInput();
		return false;
	}

	bool GrammaAnalyzer::P() {
		// P -> string I ; D
		WriteToDebugFile(level + "[��] : <����> �� <����˵������>;<��䲿��>");
		if (!match("string")) return false;
		m_output_program += "string ";
		WriteToDebugFile(level + "[��] : <����˵������> �� [string]<��ʶ���б�>");
		level += "----";
		if (!I("string")) return false;
		level = level.substr(0, level.length() - 4);
		if (!match("�ֺ�")) return false;
		m_output_program += ";\n";
		if (!D()) return false;
		WriteToDebugFile(level + "[done][��] : <����> �� <����˵������>;<��䲿��>");
		WriteToOutFile(m_output_program);
		WriteToDebugFile("-------------------------�﷨��������--------------------");
		return true;
	}

	bool GrammaAnalyzer::I(string type) {
		// I -> i Ip 
		WriteToDebugFile(level + "[��] : <��ʶ���б�> �� <��ʶ���б�><��ʶ���б�prime>");
		string name = m_current_input.getValue();
		if (!match("��ʶ��")) return false;
		m_identifer_table.AddName(name);
		m_identifer_table.UpdateType(name, type);
		m_output_program += name;
		WriteToDebugFile(level + "[��] : ��д��ʶ�� ��" + name + "������Ϊ" + type);
		level += "----";
		if (!Ip(type)) return false;
		level = level.substr(0, level.length() - 4);
		return true;
	}

	bool GrammaAnalyzer::D() {
		// D -> S ; Dp
		WriteToDebugFile(level + "[��] : <��䲿��> �� <���><��䲿��prime>");
		level += "----";
		if (!S()) return false;
		level = level.substr(0, level.length() - 4);
		if (!match("�ֺ�")) return false;
		m_output_program += ";\n";
		if (!Dp()) return false;
		return true;
	}

	bool GrammaAnalyzer::Ip(string type) {
		// Ip -> , i Ip | ��
		if (m_current_input.getType() == "����") {
			// Ip -> , i Ip
			WriteToDebugFile(level + "[��] : ѡ�� <��ʶ���б�prime> ��,<��ʶ���б�><��ʶ���б�prime>");
			if (!match("����")) return false;
			m_output_program += ", ";
			string name = m_current_input.getValue();
			m_identifer_table.AddName(name);
			if (!match("��ʶ��")) return false;
			m_output_program += name;
			WriteToDebugFile(level + "[��] : ��д��ʶ�� ��" + name + "������Ϊ" + type);
			m_identifer_table.UpdateType(name, type);
			if (!Ip(type)) return false;
			return true;
		}
		if (m_current_input.getType() == "�ֺ�") {
			// Ip -> ��
			WriteToDebugFile(level + "[��] : ѡ�� <��ʶ���б�prime> �� ��");
			return true;
		}
		return false;
	}

	bool GrammaAnalyzer::S() {
		// S -> A | C | L
		if (m_current_input.getType() == "��ʶ��") {
			// S -> A
			WriteToDebugFile(level + "[��] : ѡ�� <���> �� <��ֵ���>");
			level += "----";
			if(!A()) return false;
			level = level.substr(0, level.length() - 4);
			return true;
		}
		if (m_current_input.getType() == "if") {
			// S -> C
			WriteToDebugFile(level + "[��] : ѡ�� <���> �� <�������>");
			level += "----";
			if (!C()) return false;
			level = level.substr(0, level.length() - 4);
			return true;
		}
		if (m_current_input.getType() == "do") {
			// S -> L
			WriteToDebugFile(level + "[��] : ѡ�� <���> �� <ѭ�����>");
			level += "----";
			if (!L()) return false;
			level = level.substr(0, level.length() - 4);
			return true;
		}
		return false;
	}

	bool GrammaAnalyzer::Dp() {
		// Dp -> S ; Dp | ��
		if (m_current_input.getType() == "��ʶ��" || m_current_input.getType() == "if" || m_current_input.getType() == "do") {
			// Dp -> S ; Dp 
			WriteToDebugFile(level + "[��] : ѡ�� <��䲿��prime> �� <���> ; <��䲿��prime>");
			level += "----";
			if (!S()) return false;
			level = level.substr(0, level.length() - 4);
			if (!match("�ֺ�")) return false;
			m_output_program += ";\n";
			if (!Dp()) return false;
			return true;
		}
		if (m_current_input.getType() == "end" || m_current_input.getType() == "#") {
			// Dp -> ��
			WriteToDebugFile("-------------------------�ʷ���������--------------------");
			// Dp -> ��
			WriteToDebugFile("[��] : ѡ�� <��䲿��prime> �� ��");
			WriteToDebugFile("-------------------------�ʷ���������--------------------");
			std::cout << "-------------------------�ʷ���������--------------------" << std::endl;
			return true;
		}
		return false;
	}

	bool GrammaAnalyzer::A() {
		// A -> i = E
		WriteToDebugFile(level + "[��] : <��ֵ���> �� <��ʶ��> = <���ʽ>");
		string name = m_current_input.getValue();
		if (!match("��ʶ��")) return false;
		m_output_program += outLevel + name;
		if (!match("��ֵ��")) return false;
		m_output_program += " = ";
		level += "----";
		IdentiferTable::identifier E1 = E();
		if (E1.name == "ERROR") {
			return false;
		}
		level = level.substr(0, level.length() - 4);
		m_identifer_table.UpdateValue(name, E1.value);
		WriteToDebugFile(level + "[��] : ���±�ʶ�� ��" + name + "����ֵΪ"+ E1.value);
		m_middle_code_table.AddItem("=", E1.name != "" ? E1.name : E1.value, "null", name);
		WriteToDebugFile("[��] �� ������ֵ�����Ԫʽ");
		WriteToDebugFile("[��] : ���±�ʶ�� ��" + name + "����ֵΪ"+ E1.value);
		m_middle_code_table.AddItem("=", E1.value, "null", name);
		WriteToDebugFile("[��] �� ������ֵ�����Ԫʽ");
		return true;
	}

	bool GrammaAnalyzer::C() {
		// C -> if ( F ) N else N 
		WriteToDebugFile(level + "[��] :  if ��<����>��<Ƕ�����> else <Ƕ�����>");
		if (!match("if")) return false;
		m_output_program += outLevel + "if";
		if (!match("������")) return false;
		m_output_program += "(";
		level += "----";
		IdentiferTable::identifier F1 = F();
		if (F1.name == "ERROR") {
			return false;
		}
		level = level.substr(0, level.length() - 4);

		int trueExit = m_middle_code_table.getNXQ() + 2;
		m_middle_code_table.AddItem("jnz", F1.name, "null", std::to_string(trueExit));
		WriteToDebugFile(level + "[��] �� ����if����������Ԫʽ");
		int falseExit = m_middle_code_table.getNXQ();
		m_middle_code_table.AddItem("j", "null", "null", "0");
		WriteToDebugFile(level + "[��] �� ����if���ٳ�����Ԫʽ");
		if (!match("������")) return false;
		m_output_program += ")\n";
		level += "----";
		outLevel += "  ";
		if (!N()) return false;
		outLevel = outLevel.substr(0, outLevel.length() - 2);
		level = level.substr(0, level.length() - 4);
		int Exit = m_middle_code_table.getNXQ();
		m_middle_code_table.AddItem("j", "null", "null", "0");
		WriteToDebugFile(level + "[��] �� ����if���ٳ�����Ԫʽ");
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
		WriteToDebugFile(level + "[��] : do <Ƕ�����> while ��<����>��");
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
		if (!match("������")) return false;
		m_output_program += "(";
		level += "----";
		IdentiferTable::identifier F1 = F();
		if (F1.name == "ERROR") {
			return false;
		}
		level = level.substr(0, level.length() - 4);
		m_middle_code_table.AddItem("jnz", F1.name, "null", std::to_string(NEXT));
		WriteToDebugFile(level + "[��] �� ����while�����ת��Ԫʽ");
		if (!match("������")) return false;
		m_output_program += ")";
		return true;
	}

	IdentiferTable::identifier GrammaAnalyzer::E() {
		// E -> G Ep
		WriteToDebugFile(level + "[��] : <���ʽ> �� <��><���ʽprime>");
		IdentiferTable::identifier tempG = G();
		IdentiferTable::identifier tempEp = Ep(tempG);
		return tempEp;
	}

	IdentiferTable::identifier GrammaAnalyzer::F() {
		// F -> E g E
		WriteToDebugFile(level + "[��] : <����> �� <���ʽ><��ϵ�����><���ʽ>");
		level += "----";
		IdentiferTable::identifier E1 = E();
		level = level.substr(0, level.length() - 4);
		string operater = m_current_input.getValue();

		if (!match("��ϵ�����")) {
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
			WriteToDebugFile(level + "[��] �� ����С����Ԫʽ");
			if (E1.value.length() < E2.value.length()) {
				temp.value = "true";
			}
			else {
				temp.value = "false";
			}
		}
		if (operater == "<=") {
			WriteToDebugFile(level + "[��] �� ����С�ڵ�����Ԫʽ");
			if (E1.value.length() <= E2.value.length()) {
				temp.value = "true";
			}
			else {
				temp.value = "false";
			}
		}
		if (operater == ">") {
			WriteToDebugFile(level + "[��] �� ����������Ԫʽ");
			if (E1.value.length() > E2.value.length()) {
				temp.value = "true";
			}
			else {
				temp.value = "false";
			}
		}
		if (operater == ">=") {
			WriteToDebugFile(level + "[��] �� �������ڵ�����Ԫʽ");
			if (E1.value.length() >= E2.value.length()) {
				temp.value = "true";
			}
			else {
				temp.value = "false";
			}
		}
		if (operater == "==") {
			WriteToDebugFile(level + "[��] �� ����������Ԫʽ");
			if (E1.value == E2.value) {
				temp.value = "true";
			}
			else {
				temp.value = "false";
			}
		}
		if (operater == "<>") {
			WriteToDebugFile(level + "[��] �� ������������Ԫʽ");
			if (E1.value != E2.value) {
				temp.value = "true";
			}
			else {
				temp.value = "false";
			}
		}
		m_middle_code_table.AddItem(operater, E1.name, E2.name, temp.name);
		WriteToDebugFile("[��] �� ������ϵ������Ԫʽ");
		return temp;
	}

	bool GrammaAnalyzer::N() {
		// N -> S | M
		if (m_current_input.getType() == "��ʶ��" || m_current_input.getType() == "if" || m_current_input.getType() == "do") {
			// N -> S 
			WriteToDebugFile(level + "[��] : ѡ�� <Ƕ�����> �� <���>");
			level += "----";
			if (!S()) return false;
			level = level.substr(0, level.length() - 4);
			m_output_program += "\n";
			return true;
		}
		if (m_current_input.getType() == "start") {
			// N -> M
			WriteToDebugFile(level + "[��] : ѡ�� <Ƕ�����> �� <�������>");
			level += "----";
			if (!M()) return false;
			level = level.substr(0, level.length() - 4);
			return true;
		}
		return false;
	}

	IdentiferTable::identifier GrammaAnalyzer::G() {
		// G -> Y Gp
		WriteToDebugFile(level + "[��] : <��> �� <����><��prime>");
		IdentiferTable::identifier tempY = Y();
		IdentiferTable::identifier tempGp = Gp(tempY);
		return tempGp;
	}

	IdentiferTable::identifier GrammaAnalyzer::Ep(IdentiferTable::identifier tempId) {
		// Ep -> + G Ep | ��
		if (m_current_input.getType() == "���������") {
			// Ep -> + G Ep
			WriteToDebugFile(level + "[��] : ѡ�� <���ʽprime> �� <���������><��><���ʽprime>");
			string name = m_current_input.getValue();
			if (!match("���������")) {
				return errorId;
			}
			m_output_program += " + ";
			level += "----";
			IdentiferTable::identifier tempG = G();
			level = level.substr(0, level.length() - 4);
			IdentiferTable::identifier temp = m_temp_var_table.tempVar();
			WriteToDebugFile(level + "[��] : ��������������ʱ����");
			temp.type = tempId.type;
			temp.value = tempId.value + tempG.value;
			m_temp_var_table.addTempVar(temp);
			m_middle_code_table.AddItem("+", tempId.name != "" ?tempId.name : tempId.value, tempG.name != "" ? tempG.name : tempG.value, temp.name);
			WriteToDebugFile(level + "[��] �� ��������������Ԫʽ");
			IdentiferTable::identifier tempEp = Ep(temp);
			return tempEp;
		}
		if (m_current_input.getType() == "��ϵ�����" || m_current_input.getType() == "�ֺ�" || m_current_input.getType() == "������") {
			// Ep -> ��
			WriteToDebugFile(level + "[��] : ѡ�� <���ʽprime> �� ��");
			return tempId;
		}
		return tempId;
	}

	IdentiferTable::identifier GrammaAnalyzer::Y() {
		// Y -> i | s | ( E )
		if (m_current_input.getType() == "��ʶ��") {
			// Y -> i
			WriteToDebugFile(level + "[��] : ѡ�� <����> �� <��ʶ��>");
			string name = m_current_input.getValue();
			if (!match("��ʶ��")) {
				return errorId;
			}
			m_output_program += "\"" + name + "\"";
			WriteToDebugFile(level + "[��] : ��ȡ���ʽ�б�ʶ����" + name + "��ֵ" + m_identifer_table.getIdentifer(name).value);
			return m_identifer_table.getIdentifer(name);
		}
		if (m_current_input.getType() == "�ַ���") {
			// Y -> s
			WriteToDebugFile(level + "[��] : ѡ�� <����> �� <�ַ���>");
			string value = m_current_input.getValue();
			if (!match("�ַ���")) {
				return errorId;
			}
			m_output_program += "\"" + value + "\"";
			IdentiferTable::identifier tempid;
			WriteToDebugFile(level + "[��] : �����ַ�����ʱ����");
			tempid.value = value;
			tempid.type = "string";
			return tempid;
		}
		if (m_current_input.getType() == "������") {
			// Y -> i
			WriteToDebugFile(level + "[��] : ѡ�� <����> �� <������>");
			if (!match("������")) {
				return errorId;
			}
			m_output_program += "(";
			level += "----";
			IdentiferTable::identifier tempE = E();
			level = level.substr(0, level.length() - 4);
			if (!match("������")) {
				return errorId;
			}
			m_output_program += ")";
			return tempE;
		}
		IdentiferTable::identifier null;
		return null;
	}

	IdentiferTable::identifier GrammaAnalyzer::Gp(IdentiferTable::identifier tempIn) {
		// Gp -> * n Gp | ��
		if (m_current_input.getType() == "�ظ������") {
			// Gp -> j n Gp
			WriteToDebugFile("[��] : ѡ�� <��prime> �� <�ظ������><����><��prime>");
			match("�ظ������");
			IdentiferTable::identifier tempId;
			WriteToDebugFile(level + "[��] : �����ظ�������ʱ����");
			tempId.value = m_current_input.getValue();
			if (!match("����")) {
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
			WriteToDebugFile(level + "[��] �� �����ظ�������Ԫʽ");
			IdentiferTable::identifier Gp1 = Gp(temp);
			return Gp1;
		}
		if (m_current_input.getType() == "���������") {
			// Gp -> ��
			WriteToDebugFile(level + "[��] : ѡ�� <��prime> �� ��");
			return tempIn;
		}
		return tempIn;
	}

	bool GrammaAnalyzer::M() {
		// M -> start D end
		WriteToDebugFile("[��] : ѡ�� <��prime> �� <�ظ������><����><��prime>");
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