#include <iostream>
#include <string>
#include <vector>
using namespace std;

const vector<string> chineseDigits = {"��", "Ҽ", "��", "��", "��", "��", "½", "��", "��", "��"};

const vector<string> chineseUnits = {"", "ʰ", "��", "Ǫ", "��", "ʰ", "��", "Ǫ", "��"};

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
                // ֻ������Ҫ��ʱ�������
                result = chineseDigits[0] + result;
                needZero = false;
            }
            // ��������ڵ�λ
            if (unitIndex == 4 || unitIndex == 8) {
                result = chineseUnits[unitIndex] + result;
            }
        } else {
            // ������ֺ͵�λ
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