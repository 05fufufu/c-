#include <iostream>
using namespace std;

int main() {
	int arr[5] = {300, 350, 200, 400, 250}; //��������
	cout << "����ǰ������Ϊ" << endl;
	for (int i = 0; i < 5; i++) {
		cout << arr[i] << " ";
	}
	cout << endl;
	//�������鳤��
	int start = 0;
	int end = sizeof(arr) / sizeof(arr[0]) - 1;
	//��������Ԫ������
	while (start < end) {
		int temp = arr[start];
		arr[start] = arr[end];
		arr[end] = temp;
		start++;
		end--;
	}
	//������
	cout << "Ԫ�ص��ú������Ϊ" << endl;
	for (int i = 0; i < 5; i++) {
		cout << arr[i] << " ";
	}
	return 0;
}