#include <iostream>
using namespace std;

int main() {
	int arr[5] = {300, 350, 200, 400, 250}; //定义数组
	cout << "逆置前的数组为" << endl;
	for (int i = 0; i < 5; i++) {
		cout << arr[i] << " ";
	}
	cout << endl;
	//计算数组长度
	int start = 0;
	int end = sizeof(arr) / sizeof(arr[0]) - 1;
	//进行数组元素逆置
	while (start < end) {
		int temp = arr[start];
		arr[start] = arr[end];
		arr[end] = temp;
		start++;
		end--;
	}
	//输出结果
	cout << "元素倒置后的数组为" << endl;
	for (int i = 0; i < 5; i++) {
		cout << arr[i] << " ";
	}
	return 0;
}