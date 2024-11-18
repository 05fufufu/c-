#include <iostream>
#include <stack>
#include <string>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <sstream>

using namespace std;

class ExpressionEvaluator {
	private:
		// �ж��Ƿ�Ϊ�����
		bool isOperator(char c) {
			return c == '+' || c == '-' || c == '*' || c == '/';
		}

		// �ж��Ƿ�Ϊ���ֻ�С����
		bool isNumberChar(char c) {
			return isdigit(c) || c == '.';
		}

		// ��ȡ��������ȼ�
		int getPriority(char op) {
			if (op == '+' || op == '-')
				return 1;
			if (op == '*' || op == '/')
				return 2;
			return 0;
		}

		// ִ������
		double calculate(double a, double b, char op) {
			switch (op) {
				case '+':
					return a + b;
				case '-':
					return a - b;
				case '*':
					return a * b;
				case '/':
					if (b == 0)
						throw runtime_error("��������Ϊ��");
					return a / b;
				default:
					throw runtime_error("��Ч�������");
			}
		}

		// ���ַ�������ȡ����
		double extractNumber(const string &expr, size_t &i) {
			string num;
			bool hasDecimalPoint = false;

			// ������
			if (i > 0 && expr[i - 1] == '(' && expr[i] == '-') {
				num += '-';
				i++;
			}

			while (i < expr.length() && (isNumberChar(expr[i]))) {
				if (expr[i] == '.') {
					if (hasDecimalPoint) {
						throw runtime_error("���ָ�ʽ���󣺶��С����");
					}
					hasDecimalPoint = true;
				}
				num += expr[i];
				i++;
			}
			i--; // ����һ��λ�ã���Ϊforѭ�����ټ�1

			if (num.empty() || num == "." || num == "-") {
				throw runtime_error("���ָ�ʽ����");
			}

			// ʹ�� stringstream ת���ַ����� double
			stringstream ss(num);
			double value;
			ss >> value;
			if (ss.fail()) {
				throw runtime_error("���ָ�ʽ����");
			}
			return value;
		}

	public:
		double evaluate(string expr) {
			// �Ƴ��ո�
			expr.erase(remove(expr.begin(), expr.end(), ' '), expr.end());

			if (expr.empty()) {
				throw runtime_error("���ʽΪ��");
			}

			stack<double> numbers;
			stack<char> operators;

			for (size_t i = 0; i < expr.length(); i++) {
				char c = expr[i];

				// ��������
				if (isdigit(c) || (c == '-' && (i == 0 || expr[i - 1] == '('))) {
					try {
						numbers.push(extractNumber(expr, i));
					} catch (const exception &e) {
						throw runtime_error("���ָ�ʽ����: " + string(e.what()));
					}
				}
				// ����������
				else if (c == '(') {
					operators.push(c);
				}
				// ����������
				else if (c == ')') {
					while (!operators.empty() && operators.top() != '(') {
						if (numbers.size() < 2) {
							throw runtime_error("���ʽ��ʽ��������������");
						}

						double b = numbers.top();
						numbers.pop();
						double a = numbers.top();
						numbers.pop();
						char op = operators.top();
						operators.pop();

						numbers.push(calculate(a, b, op));
					}

					if (operators.empty()) {
						throw runtime_error("���Ų�ƥ�䣺ȱ��������");
					}
					operators.pop(); // ����������
				}
				// ���������
				else if (isOperator(c)) {
					while (!operators.empty() && operators.top() != '(' &&
					        getPriority(operators.top()) >= getPriority(c)) {
						if (numbers.size() < 2) {
							throw runtime_error("���ʽ��ʽ��������������");
						}

						double b = numbers.top();
						numbers.pop();
						double a = numbers.top();
						numbers.pop();
						char op = operators.top();
						operators.pop();

						numbers.push(calculate(a, b, op));
					}
					operators.push(c);
				} else {
					throw runtime_error("�Ƿ��ַ�: " + string(1, c));
				}
			}

			// ����ʣ��������
			while (!operators.empty()) {
				if (operators.top() == '(') {
					throw runtime_error("���Ų�ƥ�䣺ȱ��������");
				}

				if (numbers.size() < 2) {
					throw runtime_error("���ʽ��ʽ��������������");
				}

				double b = numbers.top();
				numbers.pop();
				double a = numbers.top();
				numbers.pop();
				char op = operators.top();
				operators.pop();

				numbers.push(calculate(a, b, op));
			}

			if (numbers.size() != 1) {
				throw runtime_error("���ʽ��ʽ��������������");
			}

			return numbers.top();
		}
};

int main() {
	ExpressionEvaluator evaluator;
	string expression;

	while (true) {
		cout << "\n��������ʽ������'exit'�˳�����";
		getline(cin, expression);

		if (expression == "exit") {
			break;
		}

		try {
			double result = evaluator.evaluate(expression);
			cout << "�����" << result << endl;
		} catch (const exception &e) {
			cout << "����" << e.what() << endl;
		}
	}

	return 0;
}