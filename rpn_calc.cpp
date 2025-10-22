#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <iomanip>
#include <map>

using namespace std;

class RPNCalculator {
private:
    stack<double> st;
    vector<string> history;
    map<string, int> opPrecedence;
    
    // 检查字符串是否为数字
    bool isNumber(const string& str) {
        if (str.empty()) return false;
        size_t start = 0;
        if (str[0] == '-') {
            if (str.size() == 1) return false;
            start = 1;
        }
        bool decimalFound = false;
        for (size_t i = start; i < str.size(); i++) {
            if (str[i] == '.') {
                if (decimalFound) return false; // 多个小数点
                decimalFound = true;
            } else if (!isdigit(str[i])) {
                return false;
            }
        }
        return true;
    }

    // 执行基本运算
    double performBasicOperation(double a, double b, const string& op) {
        if (op == "+") return a + b;
        if (op == "-") return a - b;
        if (op == "*") return a * b;
        if (op == "/") {
            if (b == 0) throw runtime_error("错误：除零错误");
            return a / b;
        }
        throw runtime_error("错误：未知操作符 '" + op + "'");
    }

    // 执行高级运算
    double performAdvancedOperation(double a, const string& op) {
        if (op == "sqrt") {
            if (a < 0) throw runtime_error("错误：负数不能开平方根");
            return sqrt(a);
        }
        if (op == "sin") return sin(a * M_PI / 180.0); // 角度转弧度
        if (op == "cos") return cos(a * M_PI / 180.0);
        if (op == "tan") return tan(a * M_PI / 180.0);
        if (op == "fib") {
            if (a < 0 || a != floor(a)) 
                throw runtime_error("错误：斐波那契数列需要非负整数");
            int n = static_cast<int>(a);
            if (n == 0) return 0;
            if (n == 1) return 1;
            return performAdvancedOperation(n-1, "fib") + 
                   performAdvancedOperation(n-2, "fib");
        }
        throw runtime_error("错误：未知操作符 '" + op + "'");
    }

public:
    RPNCalculator() {
        // 初始化操作符优先级
        opPrecedence["+"] = 1;
        opPrecedence["-"] = 1;
        opPrecedence["*"] = 2;
        opPrecedence["/"] = 2;
        opPrecedence["^"] = 3;
        opPrecedence["sqrt"] = 4;
        opPrecedence["sin"] = 4;
        opPrecedence["cos"] = 4;
        opPrecedence["tan"] = 4;
        opPrecedence["fib"] = 4;
    }

    // 清空栈
    void clearStack() {
        while (!st.empty()) {
            st.pop();
        }
    }

    // 显示当前栈内容
    void displayStack() const {
        if (st.empty()) {
            cout << "栈为空" << endl;
            return;
        }
        
        stack<double> temp = st;
        vector<double> elements;
        
        while (!temp.empty()) {
            elements.push_back(temp.top());
            temp.pop();
        }
        
        reverse(elements.begin(), elements.end());
        cout << "当前栈: ";
        for (size_t i = 0; i < elements.size(); i++) {
            cout << fixed << setprecision(6) << elements[i];
            if (i < elements.size() - 1) cout << " ";
        }
        cout << endl;
    }

    // 处理单个token
    void processToken(const string& token) {
        if (isNumber(token)) {
            st.push(stod(token));
        } else if (token == "clear") {
            clearStack();
            cout << "栈已清空" << endl;
        } else if (token == "show") {
            displayStack();
        } else if (token == "history") {
            showHistory();
        } else if (token == "help") {
            showHelp();
        } else {
            // 处理操作符
            if (token == "^" || token == "sqrt" || token == "sin" || 
                token == "cos" || token == "tan" || token == "fib") {
                // 单目运算符
                if (st.empty()) {
                    throw runtime_error("错误：栈空，需要操作数");
                }
                double a = st.top(); st.pop();
                double result = performAdvancedOperation(a, token);
                st.push(result);
                history.push_back(token + "(" + to_string(a) + ") = " + to_string(result));
            } else {
                // 双目运算符
                if (st.size() < 2) {
                    throw runtime_error("错误：操作数不足");
                }
                double b = st.top(); st.pop();
                double a = st.top(); st.pop();
                double result = performBasicOperation(a, b, token);
                st.push(result);
                history.push_back(to_string(a) + " " + token + " " + to_string(b) + " = " + to_string(result));
            }
        }
    }

    // 显示历史记录
    void showHistory() const {
        if (history.empty()) {
            cout << "暂无历史记录" << endl;
            return;
        }
        cout << "计算历史:" << endl;
        for (size_t i = 0; i < history.size(); i++) {
            cout << i+1 << ". " << history[i] << endl;
        }
    }

    // 显示帮助信息
    void showHelp() const {
        cout << "=== RPN计算器使用说明 ===" << endl;
        cout << "基本操作:" << endl;
        cout << "  +, -, *, /  - 四则运算" << endl;
        cout << "  ^           - 幂运算 (a ^ b)" << endl;
        cout << "  sqrt        - 平方根" << endl;
        cout << "  sin, cos, tan - 三角函数（角度制）" << endl;
        cout << "  fib         - 斐波那契数列 (fib n)" << endl;
        cout << "栈操作:" << endl;
        cout << "  clear       - 清空栈" << endl;
        cout << "  show        - 显示当前栈" << endl;
        cout << "  history     - 显示计算历史" << endl;
        cout << "  help        - 显示帮助" << endl;
        cout << "  quit        - 退出程序" << endl;
        cout << "示例:" << endl;
        cout << "  5 3 +       → 8" << endl;
        cout << "  9 sqrt      → 3" << endl;
        cout << "  5 fib       → 5" << endl;
    }

    // 批量计算表达式
    void batchCalculate(const vector<string>& expressions) {
        for (const auto& expr : expressions) {
            try {
                cout << "计算: " << expr << endl;
                vector<string> tokens = split(expr);
                for (const auto& token : tokens) {
                    processToken(token);
                }
                if (!st.empty()) {
                    cout << "结果: " << fixed << setprecision(6) << st.top() << endl;
                }
            } catch (const exception& e) {
                cout << e.what() << endl;
            }
            cout << "---" << endl;
        }
    }

    // 分割字符串为tokens
    vector<string> split(const string& str) {
        vector<string> tokens;
        istringstream iss(str);
        string token;
        
        while (iss >> token) {
            tokens.push_back(token);
        }
        
        return tokens;
    }

    // 运行计算器
    void run() {
        cout << "=== RPN计算器 ===" << endl;
        cout << "输入 'help' 查看使用说明" << endl;
        cout << "输入 'quit' 退出程序" << endl << endl;

        string input;
        while (true) {
            cout << "> ";
            getline(cin, input);
            
            if (input == "quit" || input == "exit") {
                break;
            }
            
            if (input.empty()) {
                continue;
            }

            try {
                vector<string> tokens = split(input);
                for (const auto& token : tokens) {
                    processToken(token);
                }
                
                // 显示最终结果
                if (!st.empty()) {
                    cout << "结果: " << fixed << setprecision(6) << st.top() << endl;
                }
                
            } catch (const exception& e) {
                cout << e.what() << endl;
            }
        }
        
        cout << "感谢使用！" << endl;
    }
};

int main() {
    RPNCalculator calculator;
    calculator.run();
    return 0;
}
