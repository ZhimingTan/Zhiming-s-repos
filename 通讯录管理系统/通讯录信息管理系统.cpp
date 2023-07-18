#include <iostream>
#include <string>
#define P_MAX 1000
using namespace std;

struct Person
{
	string p_name;
	int p_sex;
	int p_age;
	string p_num;
	string p_address;
};

struct Book
{
	struct Person p_arr[P_MAX];
	int b_size;
};

int is_exit(Book* book, string name);

void show_menu() {
	cout << "-----------------------------------" << endl;
	cout << "----------- 1�������ϵ��----------" << endl;
	cout << "----------- 2����ʾ��ϵ��----------" << endl;
	cout << "----------- 3��ɾ����ϵ��----------" << endl;
	cout << "----------- 4��������ϵ��----------" << endl;
	cout << "----------- 5���޸���ϵ��----------" << endl;
	cout << "----------- 6�������ϵ��----------" << endl;
	cout << "----------- 0���˳�ͨѶ¼----------" << endl;
	cout << "-----------------------------------" << endl;
}

void exit_system() {
	cout << "��ӭ�´�ʹ��" << endl;
	system("pause");
	exit(0);
}
void add_person(Book *book) {
	if (book->b_size==P_MAX) {
		cout << "ͨѶ¼�������޷��������" << endl;
	}
	else {
		cout << "����������:";
		string name;
		cin >> name;
		book->p_arr[book->b_size].p_name = name;

		cout << "����������:";
		int age;
		cin >> age;
		book->p_arr[book->b_size].p_age = age;

		cout << "��ѡ���Ա�:" << endl;
		cout << "1����   2��Ů"<<endl;
		int sex;
		while (1) {
			cin >> sex;
			if (sex==1||sex==2) {
				book->p_arr[book->b_size].p_sex = sex;
				break;
			}
				cout << "��������" << endl;
		}

		cout << "�������ֻ�����:";
		string num;
		cin >> num;
		book->p_arr[book->b_size].p_num = num;

		cout << "������סַ:";
		string address;
		cin >> address;
		book->p_arr[book->b_size].p_address = address;

		book->b_size++;

		cout << "��ӳɹ�!!!" << endl;
		
	}
	system("pause");
	system("cls");
}

void show_person(Book *book) {
	if (book->b_size==0) {
		cout << "ͨѶ¼Ϊ��" << endl;
	}
	else {
		for (int i = 0; i < book->b_size;i++) {
			cout << "����:" << book->p_arr[i].p_name << "  ����:" << book->p_arr[i].p_age
				<< "  �Ա�:" << (book->p_arr[i].p_sex==1?'��':'Ů') << "  �ֻ�����:" << book->p_arr[i].p_num
				<< " סַ:" << book->p_arr[i].p_address << endl;
		}
	}
	system("pause");
	system("cls");
}



void del_person(Book *book) {
	cout << "��������Ҫɾ������:";
	string name;
	cin >> name;

	int ret = is_exit(book,name);
	
	if (ret!=-1) {
		for (int i = ret; i < book->b_size;i++) {
			book->p_arr[i] = book->p_arr[i + 1];
		}
		book->b_size--;
		cout << "ɾ���ɹ�!!" << endl;
	}
	else {
		cout << "���޴���" << endl;
	}
	system("pause");
	system("cls");

}

void find_person(Book *book) {
	cout << "��������Ҫ���ҵ���:";
	string name;
	cin >> name;
	int ret = is_exit(book,name);
	if (ret==-1) {
		cout << "���޴���" << endl;
	}
	else {
		cout << "����:" << book->p_arr[ret].p_name << "  ����:" << book->p_arr[ret].p_age
			<< "  �Ա�:" << (book->p_arr[ret].p_sex==1?'��':'Ů') << "  �ֻ�����:" << book->p_arr[ret].p_num
			<< " סַ:" << book->p_arr[ret].p_address << endl;
	}
	system("pause");
	system("cls");

}

void change_person(Book *book) {
	cout << "��������Ҫ�޸ĵ���:";
	string name;
	cin >> name;
	int ret = is_exit(book,name);
	if (ret == -1) {
		cout << "���޴���" << endl;
	}
	else {
		string name;
		int age;
		int sex;
		string num;
		string address;

		cout << "�������µ�����:";
		cin >> name;
		book->p_arr[ret].p_name = name;

		cout << "�������µ�����:";
		cin >> age;
		book->p_arr[ret].p_age = age;

		cout << "��ѡ���µ��Ա�:"<<endl;
		cout << "1����     2��Ů" << endl;
		cin >> sex;
		while (1) {
			if (sex==1||sex==2) {
				book->p_arr[ret].p_sex = sex;
				break;
			}
			cout << "������������������" << endl;
		}

		cout << "�������µ��ֻ�����:";
		cin >> num;
		book->p_arr[ret].p_num = num;

		cout << "�������µ�סַ:";
		cin >> address;
		book->p_arr[ret].p_address = address;

		cout << "�޸���ϵ����Ϣ�ɹ�" << endl;
	}

	system("pause");
	system("cls");
}

void clear_person(Book *book) {
	if (book->b_size == 0){
		cout << "ͨѶ¼Ϊ��" << endl;
	}
	else {
		book->b_size == 0;
		cout << "�����" << endl;
	}
	system("pause");
	system("cls");
}

int is_exit(Book* book, string name) {
	for (int i = 0; i < book->b_size; i++) {
		if (book->p_arr[i].p_name == name) {
			return i;
		}
	}
	return -1;
}
int main() {
	Book b;
	b.b_size = 0;
	int choice = 0;
	while (true)
	{
		show_menu();
		cout << "����������ѡ��:";
		cin >> choice;

		switch (choice)
		{
		case 1:		//���
			add_person(&b);
			break;		
		case 2:		//��ʾ
			show_person(&b);
			break;
		case 3:		//ɾ��
			del_person(&b);
			break;
		case 4:		//����
			find_person(&b);
			break;
		case 5:		//�޸�
			change_person(&b);
			break;
		case 6:		//���
			clear_person(&b);
			break;
		case 0:		//�˳�
			exit_system();
			break;
		default:
			cout << "��������" << endl;
			break;
		}
	}
	system("pause");
	system("cls");
	return 0;
}