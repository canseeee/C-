#include <iostream>
#include <string>
#include <vector>
using namespace std;

const vector<string> chineseDigits = {"零", "壹", "贰", "叁", "肆", "伍", "陆", "柒", "捌", "玖"};

const vector<string> chineseUnits = {"", "拾", "佰", "仟", "万", "拾", "佰", "仟", "亿"};

string numberToChinese(int num) {
    if (num == 0) {
        return chineseDigits[0];
    }
    
    string result;
    bool needZero = false;
    int unitIndex = 0;
    
    while (num > 0) {
        int digit = num % 10;
        
        if (digit == 0) {
            if (needZero) {
                // 只有在需要零时才添加零
                result = chineseDigits[0] + result;
                needZero = false;
            }
            // 处理万和亿单位
            if (unitIndex == 4 || unitIndex == 8) {
                result = chineseUnits[unitIndex] + result;
            }
        } else {
            // 添加数字和单位
            string current = chineseDigits[digit] + chineseUnits[unitIndex];
            result = current + result;
            needZero = true;
        }
        
        num /= 10;
        unitIndex++;
    }
    
    return result;
}

int main() {
    int number;
    cin >> number;
    
    
    cout << numberToChinese(number) << endl;
    system("pause");
    
    return 0;
}