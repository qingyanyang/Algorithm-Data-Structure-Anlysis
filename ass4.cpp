#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;
bool belong(int j, vector<string> vec_ready){
    for(int i = 0; i < vec_ready.size(); i++){
        if(j == stoi(vec_ready[i].substr(0,vec_ready[i].find(',')))){
            return true;
        }
    }
    return false;
}
string find_next(int id, int num_city, int* arr_country, int* arr_build, int* arr_destroy, vector<string> vec_ready){
    vector<int> connect;
    vector<int> disconnect;
    string unity;
    int next;
    for(int j = 0; j < num_city; j++){
        if(*(arr_country + id * num_city + j) == 1 && !belong(j,vec_ready)){
            connect.push_back(j);
        }else if(*(arr_country + id * num_city + j) == 0 && !belong(j,vec_ready)){
            disconnect.push_back(j);
        }
    }
    if(!connect.empty()){
        next = connect.front();
        for(int k = 1; k < connect.size(); k++){
            if(*(arr_destroy + id * num_city + connect[k]) > *(arr_destroy + id * num_city + next)){
                next = connect[k];
            }
        }
        unity = to_string(next) + "," + to_string(id) + ".0";
    }else{
        next = disconnect.front();
        for(int k = 1; k < disconnect.size(); k++){
            if(*(arr_build + id * num_city + disconnect[k]) < *(arr_build + id * num_city + next)){
                next = disconnect[k];
            }
        }
        unity = to_string(next) + "," + to_string(id) + "." + to_string(*(arr_build + id * num_city + next));
    }
    return unity;
}
string sort_next(vector<string> vec_next, int* arr_destroy, int num_city){
    int min_cost_id = 0;
    for(int i = 1; i < vec_next.size(); i++){
        int min = stoi(vec_next[min_cost_id].substr(vec_next[min_cost_id].find('.') + 1, vec_next[min_cost_id].length()-1));
        int min_id = stoi(vec_next[min_cost_id].substr(0, vec_next[min_cost_id].find(',')));
        int min_origin = stoi(vec_next[min_cost_id].substr(vec_next[min_cost_id].find(',') + 1, vec_next[min_cost_id].find('.')));
        int next = stoi(vec_next[i].substr(vec_next[i].find('.') + 1, vec_next[i].length()-1));
        int next_id = stoi(vec_next[i].substr(0, vec_next[i].find(',')));
        int next_origin = stoi(vec_next[i].substr(vec_next[i].find(',') + 1, vec_next[i].find('.')));
        if( min > next){
            min_cost_id = i;
        }else if(min == next){
            if(*(arr_destroy + min_origin * num_city + min_id) < *(arr_destroy + next_origin * num_city + next_id)){
                min_cost_id = i;
            }
        }
    }
    return vec_next[min_cost_id];
}
int main()
{
    string country;
    string build;
    string destroy;
    int lowest_cost = 0;
    cin >> country;
    cin >> build;
    cin >> destroy;
    //get the number of cities
    int num_city = count(country.begin(), country.end(), ',') + 1;
    if(num_city > 1){
        //create arr for country, build and destroy
        int arr_country[num_city][num_city];
        int arr_build[num_city][num_city];
        int arr_destroy[num_city][num_city];
        int arr_country_new[num_city][num_city];
        //list<int*> list_next;
        vector<string> vec_ready;
        vector<string> vec_next;
        //put input from user into these two-dimension arrays
        for(int i = 0; i < num_city; i++){
            for(int j = 0; j < num_city; j++){
                arr_country[i][j] = country[i*(num_city + 1)+j] - '0';
                char cost_cons_char = build[i*(num_city + 1)+j];
                if((int)cost_cons_char > 96){
                    arr_build[i][j] = (int)cost_cons_char - 71;
                }else{
                    arr_build[i][j] = (int)cost_cons_char - 65;
                }
                char cost_des_char = destroy[i*(num_city + 1)+j];
                if((int)cost_des_char > 96){
                    arr_destroy[i][j] = (int)cost_des_char - 71;
                }else{
                    arr_destroy[i][j] = (int)cost_des_char - 65;
                }
                arr_country_new[i][j] = 0;
            }
        }
        //initialization
        vec_ready.push_back("0,0.0");
        string next;
        for(int l = 0; l < num_city - 1; l++) {
            for (int k = 0; k < vec_ready.size(); k++) {
                string unity = find_next(stoi(vec_ready[k].substr(0,vec_ready[k].find(','))), num_city, (int *) arr_country, (int *) arr_build, (int *) arr_destroy,
                                 vec_ready);
                vec_next.push_back(unity);
            }
            next = sort_next(vec_next, (int*)arr_destroy, num_city);
            arr_country_new[stoi(next.substr(next.find(',')+1,next.find('.')))][stoi(next.substr(0,next.find(',')))] = 1;
            arr_country_new[stoi(next.substr(0,next.find(',')))][stoi(next.substr(next.find(',')+1,next.find('.')))] = 1;
            vec_ready.push_back(next);
            vec_next.clear();
        }
        for(int o = 0; o < vec_ready.size(); o++){
            lowest_cost += stoi(vec_ready[o].substr(vec_ready[o].find('.')+1, vec_ready[o].length() -1));
        }
        int lowest_cost_destroy = 0;
        for(int i = 0; i < num_city; i++){
            for(int j = 0; j < num_city; j++){
                if(arr_country[i][j] == 1 && arr_country_new[i][j] == 0){
                    lowest_cost_destroy += arr_destroy[i][j];
                }
            }
        }
        lowest_cost = lowest_cost + lowest_cost_destroy / 2;
    }else{
        lowest_cost = 0;
    }
    cout << lowest_cost << endl;
}