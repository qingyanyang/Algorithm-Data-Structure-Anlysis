#include <iostream>
#include <string>
using namespace std;

//method to reverse a string variable
string reverse(string sum){
    int len = sum.length();
    string sumReverse = "";
    while(len > 0){
        //add from the last digit in sequence
        sumReverse.push_back(sum[len - 1]);
        len--;
    }
    return sumReverse;
};
//method to clear "0" from rear of the string variable and reverse it
string off0FromBack(string result){
    int len0 = result.length();
    int lenTmp = len0;
    //if length is 0 then return "0"
    if(lenTmp==0){
        return "0";
    }else{
        int count0Digit = 0;
        //count "0" from back until reach nun"0"
        while((result[lenTmp-1]-'0') == 0){
            count0Digit++;
            lenTmp--;
        }
        //left string
        result = result.substr(0,len0-count0Digit);
        return reverse(result);
    }
};
//method to equal digits of two string by adding "0" in front of the short one
string* equalDigits(string l1, string l2){
    static string lSet[2];
    int len1 = l1.length();
    int len2 = l2.length();
    string l1FullDigit = "";
    string l2FullDigit = "";
    int subtract = abs(len1-len2);
    string supplementaryDigit = "";
    //fill "0" to a new variable supplementaryDigit according to the length difference of two strings
    while(subtract != 0){
        supplementaryDigit += "0";
        subtract--;
    }
    //add supplementaryDigit in front of the short string
    if(len1>len2){
        l2FullDigit = supplementaryDigit.append(l2);
        l1FullDigit = l1;
    }else {
        l1FullDigit = supplementaryDigit.append(l1);
        l2FullDigit = l2;
    }
    //put the equal digits' strings into an array
    lSet[0]=l1FullDigit;
    lSet[1]=l2FullDigit;
    //return the pointer to the array's first element
    return lSet;
};
//method to compare two numerical strings
bool compare(string l1, string l2){
    //make them equal digits Cuz there are possibilities to compare 001 23...
    string* lSet = equalDigits(l1,l2);
    string l1FullDigit = *lSet;
    string l2FullDigit = *(lSet+1);
    int lenLong = l1FullDigit.length();
    //compare digit by digit from front to back, while the digits in the same place are different, two results
    //the third result is when all digits they have are equal
    // l1>=l2 if true, otherwise is false
    for(int i = 0; i < lenLong; i++){
        if((l1FullDigit[i]-'0')<(l2FullDigit[i]-'0')){
            return false;
        }else if((l1FullDigit[i]-'0')>(l2FullDigit[i]-'0')){
            return true;
        }
    }
    return true;
};
//school method to sum two big numerical strings with different bases
string addSchool(string l1, string l2, int base){
    string sum = "";
    int sumDigit = 0;
    int carry = 0;
    //equal digits of two strings by adding "0" in front of the short one
    string* lSet = equalDigits(l1,l2);
    string l1FullDigit = *lSet;
    string l2FullDigit = *(lSet+1);
    int len = l1FullDigit.length();
    //add digit by digit
    //get sum of carry(initial 0) together with 2 digits in the same place
    //the sumDigit equals to the reminder of the sum
    //the dynamic carry equals to the quotient of the sum
    while(len > 0){
        int temp = (l1FullDigit[len - 1]-'0') + (l2FullDigit[len - 1]-'0') + carry;
        sumDigit = temp % base;
        carry = temp / base;
        sum = sum.append(to_string(sumDigit));
        len--;
    }
    //add the last carry
    sum = sum.append(to_string(carry));
    //cuz the sum is reverse of real sum, we need to reverse it back and clear "0" behind it
    return off0FromBack(sum);
};
//school method of subtraction
string subtractSchool(string l1, string l2, int base){
    //equal digits of two strings by adding "0" in front of the short one
    string* lSet = equalDigits(l1,l2);
    string l1FullDigit = *lSet;
    string l2FullDigit = *(lSet+1);
    int lenLong = l1FullDigit.length();
    int carry = 0;
    string subtractResult = "";
    int subtractDigit = 0;
    //subtract digit by digit
    while(lenLong > 0){
        int l1Digit = l1FullDigit[lenLong-1]-'0';
        int l2Digit = l2FullDigit[lenLong-1]-'0';
        //if carry(initial "0") + l1Digit >= l2Digit, subtractDigit = their difference, carry change to 0
        //else minuend needs to plus base, then get subtractDigit, carry change to -1
        if((l1Digit+carry) >= l2Digit){
            subtractDigit = carry + l1Digit - l2Digit;
            carry = 0;
        }else{
            subtractDigit = carry + base + l1Digit - l2Digit;
            carry = -1;
        }
        lenLong--;
        subtractResult += to_string(subtractDigit);
    }
    return off0FromBack(subtractResult);
};

string multiplySchool(string l1, string l2, int base){
    //same as previous function
    string* lSet = equalDigits(l1,l2);
    string l1FullDigit = *lSet;
    string l2FullDigit = *(lSet+1);
    int lenLong = l1FullDigit.length();
    int carry = 0;
    int productDigit = 0;
    string product = "";
    string partialProduct = "";
    string postfix = "";
    //multiply digit by digit
    for(int i = lenLong - 1; i >= 0; i--){
        for(int j = lenLong - 1; j >= 0; j--){
            int tempProduct = carry + (l2FullDigit[i]-'0')*(l1FullDigit[j]-'0');
            carry = tempProduct / base;
            productDigit = tempProduct % base;
            partialProduct += to_string(productDigit);
        }
        partialProduct += to_string(carry);
        //"0" supplemented to every partial product
        if(i<(lenLong-1)){
            postfix += "0";
        }
        partialProduct = reverse(partialProduct).append(postfix);
        //sum all partial products together
        product = addSchool(product,partialProduct,base);
        partialProduct = "";
        carry = 0;
    }
    return product;
};
//method to do recursive Karatsuba multiplication
string multiplyKaratsuba(string l1, string l2, int base){
    string* lSet = equalDigits(l1,l2);
    string l1FullDigit = *lSet;
    string l2FullDigit = *(lSet+1);
    int lenLong = l1FullDigit.length();
    string product = "";
    //if the length of string is less than and equal to 3, using school multiply method
    if(lenLong <= 3){
        product = multiplySchool(l1FullDigit,l2FullDigit,base);
        return product;
    }else{
        //apply multiplyKaratsuba formula,
        //watch out using build-in(by me) addition, subtraction or multiplication methods to do computation
        //cuz need to be cautious about base issue
        int k1 = lenLong / 2;
        int k2 = lenLong - k1;
        string l11 = l1FullDigit.substr(0,k1);
        string l10 = l1FullDigit.substr(k1,k2);
        string l21 = l2FullDigit.substr(0,k1);
        string l20 = l2FullDigit.substr(k1,k2);

        string p1 = multiplyKaratsuba(l11,l21,base);
        string p2 = multiplyKaratsuba(l10,l20,base);

        string a0Pa1 = addSchool(l10, l11, base);
        string b0Pb1 = addSchool(l20, l21, base);
        string px = subtractSchool(subtractSchool(multiplyKaratsuba(a0Pa1,b0Pb1,base), p1, base), p2, base);
        //don't need to compute B^2k and B^k numerically, just do alignment
        int count = k2;
        while(count>0){
            p1 += "00";
            px += "0";
            count--;
        }
        return addSchool(addSchool(p1,px,base),p2,base);
    }
};
//division method which applies to the situation when the digits of dividend are similar to divisor's
string divisionSmall(string l1, string l2, int base){
    string* lSet = equalDigits(l1,l2);
    string l1FullDigit = *lSet;
    string l2FullDigit = *(lSet+1);
    string quotient = "0";
    //use subtraction to get times
    while(compare(l1FullDigit,l2FullDigit)) {
        l1FullDigit = subtractSchool(l1FullDigit, l2FullDigit, base);
        quotient = addSchool(quotient, "1", base);
    }
    return quotient;
};
//school division method, the run time is better than divisionSmall
//applies to the situation when the digits of dividend are quite bigger to divisor's
string divisionBig(string l1, string l2, int base){
    int len1 = l1.length();
    int len2 = l2.length();
    string quotient = "";
    string quotientDigit = "";
    string dividendTmp = "";
    string diff = "";
    if(len1<=len2){
        return divisionSmall(l1,l2,base);
    }else{
        dividendTmp=l1.substr(0,len2);
        for(int i = len2; i <= len1; i++){
            quotientDigit = divisionSmall(dividendTmp,l2,base);
            quotient += quotientDigit;
            diff = subtractSchool(dividendTmp, multiplySchool(quotientDigit,l2,base),base);
            dividendTmp = diff + l1[i];
        }
        quotient = off0FromBack(reverse(quotient));
        return quotient;
    }
};
//recursive division method applies to the situation when the digits of dividend are much longer than the divisor's
//the formula of the algorithm
// ⌊a/b⌋ = ⌊a0/b⌋*B^k + ⌊a1/b⌋ + ⌊((a0 - ⌊a0/b⌋*b)*B^k + (a1 - ⌊a1/b⌋*b))/b⌋
//the stop condition can be when the half of length of dividend is less than and equal to the length of divisor
string divisionRecursive(string l1, string l2, int base){
    int len1 = l1.length();
    int len2 = l2.length();
    if(len1 <= len2){
        return divisionSmall(l1,l2,base);
    }else{
        if((len1/2) <= len2){
            return divisionBig(l1,l2,base);
        }else{
            int k1 = len1 / 2;
            int k2 = len1 - k1;
            string l10 = l1.substr(0,k1);
            string l11 = l1.substr(k1,k2);

            string equation0 = divisionRecursive(l10,l2,base);
            string equation1 = divisionRecursive(l11,l2,base);

            string thirdPart0 = subtractSchool(l10,multiplyKaratsuba(equation0, l2, base),base);
            string thirdPart1 = subtractSchool(l11,multiplyKaratsuba(equation1, l2, base),base);

            int count = k2;
            while(count>0){
                equation0 += "0";
                thirdPart0 += "0";
                count--;
            }
            string thirdPart = addSchool(thirdPart0,thirdPart1,base);
            string equation2 = divisionRecursive(thirdPart,l2,base);
            return addSchool(addSchool(equation0,equation1,base),equation2,base);
        }
    }
};

int main()
{
    string l1, l2;
    int base;
    cin >> l1;
    cin >> l2;
    cin >> base;
    cout << addSchool(l1,l2,base) << " ";
    cout << multiplyKaratsuba(l1,l2,base) << " ";
    cout << divisionRecursive(l1,l2,base) << endl;
    return 0;
}