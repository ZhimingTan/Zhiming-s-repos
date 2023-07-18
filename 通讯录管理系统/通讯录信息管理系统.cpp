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
	cout << "----------- 1、添加联系人----------" << endl;
	cout << "----------- 2、显示联系人----------" << endl;
	cout << "----------- 3、删除联系人----------" << endl;
	cout << "----------- 4、查找联系人----------" << endl;
	cout << "----------- 5、修改联系人----------" << endl;
	cout << "----------- 6、清空联系人----------" << endl;
	cout << "----------- 0、退出通讯录----------" << endl;
	cout << "-----------------------------------" << endl;
}

void exit_system() {
	cout << "欢迎下次使用" << endl;
	system("pause");
	exit(0);
}
void add_person(Book *book) {
	if (book->b_size==P_MAX) {
		cout << "通讯录已满，无法继续添加" << endl;
	}
	else {
		cout << "请输入姓名:";
		string name;
		cin >> name;
		book->p_arr[book->b_size].p_name = name;

		cout << "请输入年龄:";
		int age;
		cin >> age;
		book->p_arr[book->b_size].p_age = age;

		cout << "请选择性别:" << endl;
		cout << "1、男   2、女"<<endl;
		int sex;
		while (1) {
			cin >> sex;
			if (sex==1||sex==2) {
				book->p_arr[book->b_size].p_sex = sex;
				break;
			}
				cout << "输入有误" << endl;
		}

		cout << "请输入手机号码:";
		string num;
		cin >> num;
		book->p_arr[book->b_size].p_num = num;

		cout << "请输入住址:";
		string address;
		cin >> address;
		book->p_arr[book->b_size].p_address = address;

		book->b_size++;

		cout << "添加成功!!!" << endl;
		
	}
	system("pause");
	system("cls");
}

void show_person(Book *book) {
	if (book->b_size==0) {
		cout << "通讯录为空" << endl;
	}
	else {
		for (int i = 0; i < book->b_size;i++) {
			cout << "姓名:" << book->p_arr[i].p_name << "  年龄:" << book->p_arr[i].p_age
				<< "  性别:" << (book->p_arr[i].p_sex==1?'男':'女') << "  手机号码:" << book->p_arr[i].p_num
				<< " 住址:" << book->p_arr[i].p_address << endl;
		}
	}
	system("pause");
	system("cls");
}



void del_person(Book *book) {
	cout << "请输入您要删除的人:";
	string name;
	cin >> name;

	int ret = is_exit(book,name);
	
	if (ret!=-1) {
		for (int i = ret; i < book->b_size;i++) {
			book->p_arr[i] = book->p_arr[i + 1];
		}
		book->b_size--;
		cout << "删除成功!!" << endl;
	}
	else {
		cout << "查无此人" << endl;
	}
	system("pause");
	system("cls");

}

void find_person(Book *book) {
	cout << "请输入您要查找的人:";
	string name;
	cin >> name;
	int ret = is_exit(book,name);
	if (ret==-1) {
		cout << "查无此人" << endl;
	}
	else {
		cout << "姓名:" << book->p_arr[ret].p_name << "  年龄:" << book->p_arr[ret].p_age
			<< "  性别:" << (book->p_arr[ret].p_sex==1?'男':'女') << "  手机号码:" << book->p_arr[ret].p_num
			<< " 住址:" << book->p_arr[ret].p_address << endl;
	}
	system("pause");
	system("cls");

}

void change_person(Book *book) {
	cout << "请输入您要修改的人:";
	string name;
	cin >> name;
	int ret = is_exit(book,name);
	if (ret == -1) {
		cout << "查无此人" << endl;
	}
	else {
		string name;
		int age;
		int sex;
		string num;
		string address;

		cout << "请输入新的姓名:";
		cin >> name;
		book->p_arr[ret].p_name = name;

		cout << "请输入新的年龄:";
		cin >> age;
		book->p_arr[ret].p_age = age;

		cout << "请选择新的性别:"<<endl;
		cout << "1、男     2、女" << endl;
		cin >> sex;
		while (1) {
			if (sex==1||sex==2) {
				book->p_arr[ret].p_sex = sex;
				break;
			}
			cout << "输入有误，请重新输入" << endl;
		}

		cout << "请输入新的手机号码:";
		cin >> num;
		book->p_arr[ret].p_num = num;

		cout << "请输入新的住址:";
		cin >> address;
		book->p_arr[ret].p_address = address;

		cout << "修改联系人信息成功" << endl;
	}

	system("pause");
	system("cls");
}

void clear_person(Book *book) {
	if (book->b_size == 0){
		cout << "通讯录为空" << endl;
	}
	else {
		book->b_size == 0;
		cout << "已清空" << endl;
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
		cout << "请输入您的选择:";
		cin >> choice;

		switch (choice)
		{
		case 1:		//添加
			add_person(&b);
			break;		
		case 2:		//显示
			show_person(&b);
			break;
		case 3:		//删除
			del_person(&b);
			break;
		case 4:		//查找
			find_person(&b);
			break;
		case 5:		//修改
			change_person(&b);
			break;
		case 6:		//清空
			clear_person(&b);
			break;
		case 0:		//退出
			exit_system();
			break;
		default:
			cout << "输入有误" << endl;
			break;
		}
	}
	system("pause");
	system("cls");
	return 0;
}