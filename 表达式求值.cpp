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
		// 判断是否为运算符
		bool isOperator(char c) {
			return c == '+' || c == '-' || c == '*' || c == '/';
		}

		// 判断是否为数字或小数点
		bool isNumberChar(char c) {
			return isdigit(c) || c == '.';
		}

		// 获取运算符优先级
		int getPriority(char op) {
			if (op == '+' || op == '-')
				return 1;
			if (op == '*' || op == '/')
				return 2;
			return 0;
		}

		// 执行运算
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
						throw runtime_error("除数不能为零");
					return a / b;
				default:
					throw runtime_error("无效的运算符");
			}
		}

		// 从字符串中提取数字
		double extractNumber(const string &expr, size_t &i) {
			string num;
			bool hasDecimalPoint = false;

			// 处理负数
			if (i > 0 && expr[i - 1] == '(' && expr[i] == '-') {
				num += '-';
				i++;
			}

			while (i < expr.length() && (isNumberChar(expr[i]))) {
				if (expr[i] == '.') {
					if (hasDecimalPoint) {
						throw runtime_error("数字格式错误：多个小数点");
					}
					hasDecimalPoint = true;
				}
				num += expr[i];
				i++;
			}
			i--; // 回退一个位置，因为for循环会再加1

			if (num.empty() || num == "." || num == "-") {
				throw runtime_error("数字格式错误");
			}

			// 使用 stringstream 转换字符串到 double
			stringstream ss(num);
			double value;
			ss >> value;
			if (ss.fail()) {
				throw runtime_error("数字格式错误");
			}
			return value;
		}

	public:
		double evaluate(string expr) {
			// 移除空格
			expr.erase(remove(expr.begin(), expr.end(), ' '), expr.end());

			if (expr.empty()) {
				throw runtime_error("表达式为空");
			}

			stack<double> numbers;
			stack<char> operators;

			for (size_t i = 0; i < expr.length(); i++) {
				char c = expr[i];

				// 处理数字
				if (isdigit(c) || (c == '-' && (i == 0 || expr[i - 1] == '('))) {
					try {
						numbers.push(extractNumber(expr, i));
					} catch (const exception &e) {
						throw runtime_error("数字格式错误: " + string(e.what()));
					}
				}
				// 处理左括号
				else if (c == '(') {
					operators.push(c);
				}
				// 处理右括号
				else if (c == ')') {
					while (!operators.empty() && operators.top() != '(') {
						if (numbers.size() < 2) {
							throw runtime_error("表达式格式错误：运算数不足");
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
						throw runtime_error("括号不匹配：缺少左括号");
					}
					operators.pop(); // 弹出左括号
				}
				// 处理运算符
				else if (isOperator(c)) {
					while (!operators.empty() && operators.top() != '(' &&
					        getPriority(operators.top()) >= getPriority(c)) {
						if (numbers.size() < 2) {
							throw runtime_error("表达式格式错误：运算数不足");
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
					throw runtime_error("非法字符: " + string(1, c));
				}
			}

			// 处理剩余的运算符
			while (!operators.empty()) {
				if (operators.top() == '(') {
					throw runtime_error("括号不匹配：缺少右括号");
				}

				if (numbers.size() < 2) {
					throw runtime_error("表达式格式错误：运算数不足");
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
				throw runtime_error("表达式格式错误：运算数过多");
			}

			return numbers.top();
		}
};

int main() {
	ExpressionEvaluator evaluator;
	string expression;

	while (true) {
		cout << "\n请输入表达式（输入'exit'退出）：";
		getline(cin, expression);

		if (expression == "exit") {
			break;
		}

		try {
			double result = evaluator.evaluate(expression);
			cout << "结果：" << result << endl;
		} catch (const exception &e) {
			cout << "错误：" << e.what() << endl;
		}
	}

	return 0;
}