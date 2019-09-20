#include <iostream>
#include "Database.h"
using namespace std;

istream& operator>>(istream& is, stu_t& elem) {
    string temp;
    is >> elem.id >> elem.name >> elem.gender >> elem.age >> elem.type;
    return is;
}

ostream& operator<<(ostream& os, const stu_t& elem) {
    os << left << setw(10) << elem.id;
    os << left << setw(10) << elem.name;
    os << left << setw(10) << elem.gender;
    os << left << setw(10) << elem.age;
    os << left << setw(10) << elem.type;
    return os;
}

bool database::Build() {
    int count = 0;
    stu_t elem;

    cout << "�����뽨��������Ϣϵͳ" << endl;
    cout << "�����뿼��������";
    cin >> count;
    cout << "���������뿼���Ŀ��ţ��������Ա����估�������" << endl;
    for (int i = 0; i < count; i++) {
        cin >> elem;
		if (!m_student.insert(elem)) {
			cout << "�����ظ�" << endl;
		}
    }
    Print();
	cout << "��ѡ����Ҫ���еĲ�����1Ϊ���룬2Ϊɾ����3Ϊ���ң�4Ϊ�޸ģ�5Ϊͳ�ƣ�0Ϊȡ��������\n\n";
    return true;
};

bool database::Insert() {
    stu_t elem;
    /*cout << "��ѡ����Ҫ����Ŀ�����λ�ã�";
    cin >> index;
	if (index <= 0) {
		cout << "����̫С��" << endl;
		return -1;
	}
	if (index > m_student.size() + 1) {
		cout << "����̫����" << endl;
		return -1;
	}
    */
    cout << "����������Ҫ����Ŀ����Ŀ��ţ��������Ա����估�������" << endl;
    cin >> elem;
    if (!m_student.insert(elem))
    {
        cout << "ѧ���ظ�,������������" << endl;
	return false;
    }
    return true;
}

bool database::Remove() {
    stu_t elem; 
    cout << "��ѡ����Ҫɾ���Ŀ����Ŀ��ţ�";
    cin >> elem.id;
	auto it = m_student.find(elem);
    if (it == m_student.end()) {
        cout << "�Ҳ�������Ϊ" << elem.id << "�Ŀ���" << endl;
        return false;
    }
    cout << "��ɾ���Ŀ�����Ϣ�ǣ�" << *it << endl;
	m_student.erase(it);//ʹ�õ�����ɾ��
    return true;
}

bool database::Search() const {
    stu_t elem;
    cout << "��ѡ����Ҫ���ҵĿ����Ŀ��ţ�";
    cin >> elem.id;
	auto it = m_student.find(elem);
	if (it == m_student.end()) {
		cout << "�Ҳ�������Ϊ" << elem.id << "�Ŀ���" << endl;
		return false;
	}
    cout << "����      ����      �Ա�      ����      �������  " << endl;
    cout << *it << endl;
    return true;
}

bool database::Update() {
    stu_t elem;
    cout << "��ѡ����Ҫ�޸ĵĿ����Ŀ��ţ�";
    cin >> elem.id;
	if (!m_student.erase(elem)) {
		cout << "�Ҳ�������Ϊ" << elem.id << "�Ŀ���" << endl;
		return false;
	}
    cout << "����������Ҫ�޸ĵĿ����Ŀ��ţ��������Ա����估�������" << endl;
    cin >> elem;
	m_student.insert(elem);
    return true;
}

void database::Stat() const {
    cout << "��ǰϵͳ��һ���� " << m_student.size() << " ������" << endl;
}

void database::Print() const {
	cout << endl;
	cout << "����      ����      �Ա�      ����      �������  " << endl;
    for (auto it = m_student.begin(); it != m_student.end(); ++it){
	cout << *it << endl;
    }
}

bool database::Loop() {
    string operation;
    cout << "��ѡ����Ҫ���еĲ�����";
    cin >> operation;
    switch (operation[0]) {
        case '0':
            return false;
        case '1':
            Insert();
            break;
        case '2':
            Remove();
            break;
        case '3':
            Search();
            break;
        case '4':
            Update();
            break;
        case '5':
            Stat();
            break;
        default:
			cout << "δ֪����,����������" << endl;
			cout << "������ʾ: 1Ϊ���룬2Ϊɾ����3Ϊ���ң�4Ϊ�޸ģ�5Ϊͳ�ƣ�0Ϊȡ������" << endl;
			break;
    }
    Print();
    return true;
}
/*
1234 a �� 1 aaa    8782 b Ů 2 bbb    5314 c �� 3 ccc    2456 d �� 1 ddd    7253 e Ů 2 eee    9432 f �� 3 fff

*/
