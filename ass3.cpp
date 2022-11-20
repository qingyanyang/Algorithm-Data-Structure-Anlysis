#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

class Hash_Table{
public:
    string arr[26];
    Hash_Table() {
        for(int i = 0; i < 26; i++){
            arr[i] = "never_used";
        }
    };
    int find(string key){
        int len = key.length();
        char value = key[len-1];
        int index = (int)value-97;
        int tempIndex = index;
        while(tempIndex < 26){
            if(arr[tempIndex] == "never_used"){
                return -1;
            }else if(arr[tempIndex] == key){
                return tempIndex;
            }else{
                tempIndex++;
                tempIndex = tempIndex % 26;
            }
        }
        return -1;
    }
    void insert(string key){
        int len = key.length();
        char value = key[len-1];
        int index = (int)value-97;
        int tempIndex = index;
        if(find(key) == -1){
            while(tempIndex < 26){
                if(arr[tempIndex] == "never_used" || arr[tempIndex] == "tomb_stone"){
                    arr[tempIndex] = key;
                    break;
                }else if(!arr[tempIndex].empty()){
                    tempIndex++;
                    tempIndex = tempIndex % 26;
                }
            }
        }
    };
    void remove(string key){
        if(find(key) != -1){
            arr[find(key)]="tomb_stone";
        }
    }
    void print(){
        for(string a : arr){
            if(a!="tomb_stone" && a!="never_used"){
                cout << a << " ";
            }
        }
    }
};

int main()
{
    vector<string> operationsSet;
    string line;
    getline(cin, line);
    istringstream iss(line);
    for (string d; iss >> d; operationsSet.push_back(d)) {};
    Hash_Table hashTable;
    for(string operation : operationsSet){
        string key = operation.substr(1, operation.find(" "));
        if(operation[0]=='A'){
            hashTable.insert(key);
        }else{
            hashTable.remove(key);
        }
    }
    hashTable.print();
}