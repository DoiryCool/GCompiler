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
		WriteToDebugFile("[��] : ʶ�𵽵�һ������Ϊ��" + m_current_input.getTwoTuple());
	}

	bool GrammaAnalyzer::WriteToDebugFile(string cont) {
		std::ofstream dfs("debug.txt", std::ios::app);
		dfs << cont << std::endl;
		std::cout << cont << std::endl;
		dfs.close();
		return true;
	}

	bool GrammaAnalyzer::WriteToOutFile(string cont) {
		std::ofstream ofs("out.txt", std::ios::app);
		ofs << cont << std::endl;
		std::cout << cont << std::endl;
		ofs.close();
		return true;
	}

	bool GrammaAnalyzer::match(string type) {
		if (type == m_current_input.getType()) {
			WriteToDebugFile("[��] : ��" + type + "�����ͷ�¡�" + m_current_input.getType() + "��ƥ��ɹ�");
			m_current_input = LA.nextInput();
			WriteToDebugFile("[��] : ʶ����һ������Ϊ��" + m_current_input.getTwoTuple());
			return true;
		}
		WriteToDebugFile("[��] ERROR : ��" + type + "�����ͷ�¡�" + m_current_input.getType() + "������ͬ��ƥ��ʧ��");
		m_current_input = LA.nextInput();
		return false;
	}

	bool GrammaAnalyzer::P() {
		// P -> string I ; D
		WriteToDebugFile("[��] : <����> �� <����˵������>;<��䲿��>");
		if (!match("string")) return false;
		WriteToDebugFile("[��] : <����˵������> �� [string]<��ʶ���б�>");
		I("string");
		if (!match("�ֺ�")) return false;
		D();
		WriteToDebugFile("-------------------------�﷨��������--------------------");
		return true;
	}

	bool GrammaAnalyzer::I(string type) {
		// I -> i Ip 
		WriteToDebugFile("[��] : <��ʶ���б�> �� <��ʶ���б�><��ʶ���б�prime>");
		string name = m_current_input.getValue();
		m_identifer_table.AddName(name);
		m_identifer_table.UpdateType(name, type);
		if (!match("��ʶ��")) return false;
		WriteToDebugFile("[��] : ��д��ʶ�� ��" + name + "������Ϊ" + type);
		Ip(type);
		return true;
	}

	bool GrammaAnalyzer::D() {
		// D -> S ; Dp
		WriteToDebugFile("[��] : <��䲿��> �� <���><��䲿��prime>");
		S();
		if (!match("�ֺ�")) return false;
		Dp();
		return true;
	}

	bool GrammaAnalyzer::Ip(string type) {
		// Ip -> , i Ip | ��
		if (m_current_input.getType() == "����") {
			// Ip -> , i Ip
			WriteToDebugFile("[��] : ѡ�� <��ʶ���б�prime> ��,<��ʶ���б�><��ʶ���б�prime>");
			if (!match("����")) return false;
			string name = m_current_input.getValue();
			m_identifer_table.AddName(name);
			if (!match("��ʶ��")) return false;
			WriteToDebugFile("[��] : ��д��ʶ�� ��" + name + "������Ϊ" + type);
			m_identifer_table.UpdateType(name, type);
			Ip(type);
			return true;
		}
		if (m_current_input.getType() == "�ֺ�") {
			// Ip -> ��
			WriteToDebugFile("[��] : ѡ�� <��ʶ���б�prime> �� ��");
			return true;
		}
		return false;
	}

	bool GrammaAnalyzer::S() {
		// S -> A | C | L
		if (m_current_input.getType() == "��ʶ��") {
			// S -> A
			WriteToDebugFile("[��] : ѡ�� <���> �� <��ֵ���>");
			A();
			return true;
		}
		if (m_current_input.getType() == "if") {
			// S -> C
			WriteToDebugFile("[��] : ѡ�� <���> �� <�������>");
			C();
			return true;
		}
		if (m_current_input.getType() == "do") {
			// S -> L
			WriteToDebugFile("[��] : ѡ�� <���> �� <ѭ�����>");
			L();
			return true;
		}
		return false;
	}

	bool GrammaAnalyzer::Dp() {
		// Dp -> S ; Dp | ��
		if (m_current_input.getType() == "��ʶ��" || m_current_input.getType() == "if" || m_current_input.getType() == "do") {
			// Dp -> S ; Dp 
			WriteToDebugFile("[��] : ѡ�� <��䲿��prime> �� <���> ; <��䲿��prime>");
			S();
			if (!match("�ֺ�")) return false;
			Dp();
			return true;
		}
		if (m_current_input.getType() == "end") {
			// Dp -> ��
			WriteToDebugFile("[��] : ѡ�� <��䲿��prime> �� ��");
			return true;
		}
		if (m_current_input.getType() == "#") {
			// Dp -> ��
			WriteToDebugFile("[��] : ѡ�� <��䲿��prime> �� ��");
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
		WriteToDebugFile("[��] : <��ֵ���> �� <��ʶ��> = <���ʽ>");
		string name = m_current_input.getValue();
		if (!match("��ʶ��")) return false;
		if (!match("��ֵ��")) return false;
		IdentiferTable::identifier E1 = E();
		m_identifer_table.UpdateValue(name, E1.value);
		WriteToDebugFile("[��] : ���±�ʶ�� ��" + name + "����ֵΪ"+ E1.value);
		m_middle_code_table.AddItem("=", E1.name != "" ? E1.name : E1.value, "null", name);
		WriteToDebugFile("[��] �� ������ֵ�����Ԫʽ");
		WriteToDebugFile("[��] : ���±�ʶ�� ��" + name + "����ֵΪ"+ E1.value);
		m_middle_code_table.AddItem("=", E1.value, "null", name);
		WriteToDebugFile("[��] �� ������ֵ�����Ԫʽ");
		return true;
	}

	bool GrammaAnalyzer::C() {
		// C -> if ( F ) N else N 
		WriteToDebugFile("[��] :  if ��<����>��<Ƕ�����> else <Ƕ�����>");
		if (!match("if")) return false;
		if (!match("������")) return false;
		IdentiferTable::identifier F1 = F();

		int trueExit = m_middle_code_table.getNXQ() + 2;
		m_middle_code_table.AddItem("jnz", F1.name, "null", std::to_string(trueExit));
		WriteToDebugFile("[��] �� ����if����������Ԫʽ");
		int falseExit = m_middle_code_table.getNXQ();
		m_middle_code_table.AddItem("j", "null", "null", "0");
		WriteToDebugFile("[��] �� ����if���ٳ�����Ԫʽ");
		if (!match("������")) return false;
		N();
		int Exit = m_middle_code_table.getNXQ();
		m_middle_code_table.AddItem("j", "null", "null", "0");
		WriteToDebugFile("[��] �� ����if���ٳ�����Ԫʽ");
		m_middle_code_table.backPath(falseExit, std::to_string(m_middle_code_table.getNXQ()));
		if (!match("else")) return false;
		N();
		m_middle_code_table.backPath(Exit, std::to_string(m_middle_code_table.getNXQ()));
		return true;
	}

	bool GrammaAnalyzer::L() {
		// L -> do N while ( F )
		WriteToDebugFile("[��] : do <Ƕ�����> while ��<����>��");
		NEXTR : int nextP = LA.getPointer();
		if (!match("do")) return false;
		int NEXT = m_middle_code_table.getNXQ();
		N();
		if (!match("while")) return false;
		if (!match("������")) return false;
		IdentiferTable::identifier F1 = F();
		m_middle_code_table.AddItem("jnz", F1.name, "null", std::to_string(NEXT));
		WriteToDebugFile("[��] �� ����while�����ת��Ԫʽ");
		if (!match("������")) return false;
		if (F1.value == "true") {
			LA.setPointer(nextP);
			goto NEXTR;
		}
		return true;
	}

	IdentiferTable::identifier GrammaAnalyzer::E() {
		// E -> G Ep
		WriteToDebugFile("[��] : <���ʽ> �� <��><���ʽprime>");
		IdentiferTable::identifier tempG = G();
		IdentiferTable::identifier tempEp = Ep(tempG);
		return tempEp;
	}

	IdentiferTable::identifier GrammaAnalyzer::F() {
		// F -> E g E
		WriteToDebugFile("[��] : <����> �� <���ʽ><��ϵ�����><���ʽ>");
		IdentiferTable::identifier E1 = E();
		string operater = m_current_input.getValue();

		match("��ϵ�����");
		IdentiferTable::identifier E2 = E();

		IdentiferTable::identifier temp = m_temp_var_table.tempVar();
		temp.type = "bool";
		temp.value = "false";
		if (operater == "<") {
			WriteToDebugFile("[��] �� ����С����Ԫʽ");
			if (E1.value.length() < E2.value.length()) {
				temp.value = "true";
			}
			else {
				temp.value = "false";
			}
		}
		if (operater == "<=") {
			WriteToDebugFile("[��] �� ����С�ڵ�����Ԫʽ");
			if (E1.value.length() <= E2.value.length()) {
				temp.value = "true";
			}
			else {
				temp.value = "false";
			}
		}
		if (operater == ">") {
			WriteToDebugFile("[��] �� ����������Ԫʽ");
			if (E1.value.length() > E2.value.length()) {
				temp.value = "true";
			}
			else {
				temp.value = "false";
			}
		}
		if (operater == ">=") {
			WriteToDebugFile("[��] �� �������ڵ�����Ԫʽ");
			if (E1.value.length() >= E2.value.length()) {
				temp.value = "true";
			}
			else {
				temp.value = "false";
			}
		}
		if (operater == "==") {
			WriteToDebugFile("[��] �� ����������Ԫʽ");
			if (E1.value == E2.value) {
				temp.value = "true";
			}
			else {
				temp.value = "false";
			}
		}
		if (operater == "<>") {
			WriteToDebugFile("[��] �� ������������Ԫʽ");
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
			WriteToDebugFile("[��] : ѡ�� <Ƕ�����> �� <���>");
			S();
			return true;
		}
		if (m_current_input.getType() == "start") {
			// N -> M
			WriteToDebugFile("[��] : ѡ�� <Ƕ�����> �� <�������>");
			M();
			return true;
		}
		return false;
	}

	IdentiferTable::identifier GrammaAnalyzer::G() {
		// G -> Y Gp
		WriteToDebugFile("[��] : <��> �� <����><��prime>");
		IdentiferTable::identifier tempY = Y();
		IdentiferTable::identifier tempGp = Gp(tempY);
		return tempGp;
	}

	IdentiferTable::identifier GrammaAnalyzer::Ep(IdentiferTable::identifier tempId) {
		// Ep -> + G Ep | ��
		if (m_current_input.getType() == "���������") {
			// Ep -> + G Ep
			WriteToDebugFile("[��] : ѡ�� <���ʽprime> �� <���������><��><���ʽprime>");
			match("���������");
			IdentiferTable::identifier tempG = G();
			IdentiferTable::identifier temp = m_temp_var_table.tempVar();
			WriteToDebugFile("[��] : ��������������ʱ����");
			temp.type = tempId.type;
			temp.value = tempId.value.substr(0, tempId.value.length() - 1) + tempG.value.substr(1, tempG.value.length() - 1);
			m_temp_var_table.addTempVar(temp);
			m_middle_code_table.AddItem("+", tempId.name != "" ?tempId.name : tempId.value, tempG.name != "" ? tempG.name : tempG.value, temp.name);
			WriteToDebugFile("[��] �� ��������������Ԫʽ");
			IdentiferTable::identifier tempEp = Ep(temp);
			return tempEp;
		}
		if (m_current_input.getType() == "��ϵ�����" || m_current_input.getType() == "�ֺ�" || m_current_input.getType() == "������") {
			// Ep -> ��
			WriteToDebugFile("[��] : ѡ�� <���ʽprime> �� ��");
			return tempId;
		}
		return tempId;
	}

	IdentiferTable::identifier GrammaAnalyzer::Y() {
		// Y -> i | s | ( E )
		if (m_current_input.getType() == "��ʶ��") {
			// Y -> i
			WriteToDebugFile("[��] : ѡ�� <����> �� <��ʶ��>");
			string name = m_current_input.getValue();
			match("��ʶ��");
			WriteToDebugFile("[��] : ��ȡ���ʽ�б�ʶ����" + name + "��ֵ" + m_identifer_table.getIdentifer(name).value);
			return m_identifer_table.getIdentifer(name);
		}
		if (m_current_input.getType() == "�ַ���") {
			// Y -> s
			WriteToDebugFile("[��] : ѡ�� <����> �� <�ַ���>");
			string value = m_current_input.getValue();
			match("�ַ���");
			IdentiferTable::identifier tempid;
			WriteToDebugFile("[��] : �����ַ�����ʱ����");
			tempid.value = value;
			tempid.type = "string";
			return tempid;
		}
		if (m_current_input.getType() == "������") {
			// Y -> i
			WriteToDebugFile("[��] : ѡ�� <����> �� <������>");
			match("������");
			IdentiferTable::identifier tempE = E();
			match("������");
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
			WriteToDebugFile("[��] : �����ظ�������ʱ����");
			tempId.value = m_current_input.getValue();
			match("����");
			IdentiferTable::identifier temp = m_temp_var_table.tempVar();
			temp.type = tempIn.type;
			temp.value = tempIn.value;
			for (int i = 0; i < std::stoi(tempId.value); i++) {
				temp.value = temp.value.substr(0, temp.value.length() - 1) + tempIn.value.substr(1, tempIn.value.length() - 1);
			}
			m_temp_var_table.addTempVar(temp);
			m_middle_code_table.AddItem("*", tempIn.name,tempId.value, temp.name);
			WriteToDebugFile("[��] �� �����ظ�������Ԫʽ");
			IdentiferTable::identifier Gp1 = Gp(temp);
			return Gp1;
		}
		if (m_current_input.getType() == "���������") {
			// Gp -> ��
			WriteToDebugFile("[��] : ѡ�� <��prime> �� ��");
			return tempIn;
		}
		return tempIn;
	}

	bool GrammaAnalyzer::M() {
		// M -> start D end
		WriteToDebugFile("[��] : ѡ�� <��prime> �� <�ظ������><����><��prime>");
		if (!match("start")) return false;
		D();
		if (!match("end")) return false;
	 	return true;
	}

	GrammaAnalyzer::~GrammaAnalyzer()
	{

	}
}