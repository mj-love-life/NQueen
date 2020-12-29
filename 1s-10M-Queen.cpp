#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

int N;
vector<int> emptyRows;
vector<int> Adds;
vector<int> Minus;
vector<int> NQueen; // NQueen[i]表示i行NQueen[i]列放置一个皇后
vector<int> RowCnt;
default_random_engine e;
uniform_int_distribution<int> NRand;
vector<int>::iterator emptyRowsTemp;
int AddsIndex(int row, int col) {return row + col;}
int MinusIndex(int row, int col) {return row - col + N - 1;}
int inConflict(int row, int col) {
    return RowCnt[col] + Adds[AddsIndex(row, col)] + Minus[MinusIndex(row, col)];
}

void ResetOrInit(int queenNum) {
    // 初始化赋值
    N = queenNum;
    emptyRows = vector<int> ();
    Adds = vector<int> (2 * N - 1, 0);
    Minus = vector<int> (2 * N - 1, 0);
    NQueen = vector<int> (N, 0);
    RowCnt = vector<int> (N, 0);

    // 对皇后进行放置
    int t = N-1;
    for(int i = 0; i < N; i++) {
        if (t < 0) t = N - 2;
        NQueen[i] = t;
        RowCnt[t]++;
        Adds[AddsIndex(i, t)]++;
        Minus[MinusIndex(i, t)]++;
        t -= 2;
    }
    // 初始化随机数
    e = default_random_engine(100);
    NRand = uniform_int_distribution<int> (0, N-1);
}

// 
int KaSi = 0;
int FindMinConfictY(int x) {
    KaSi = 0;
    for(int i = 0; i < emptyRows.size(); i++) {
        if (inConflict(x, emptyRows[i]) == 0) {
            // 此处添加删除操作？减少寻找时间开销：没有必要，时间开销小，emptyRows基本长度极小
            return emptyRows[i];
        }
    }
    while(true){
        int y = NRand(e);
        KaSi++;
        if (inConflict(x, y) == 1 && y != NQueen[x]) {
            // cout << "Kasile:" << KaSi << endl;
            return y; 
        }
    }
}

// 小规模的计算
int SmallFindMinConfictY(int x) {
    int y = NQueen[x];
    int MinConficts;
    int MinY;
    if (y + 1 < N) {
        MinConficts = Adds[AddsIndex(x, y + 1)] + Minus[MinusIndex(x, y + 1)] + RowCnt[y + 1];
        MinY = y + 1;
    }
    else {
        MinConficts = Adds[AddsIndex(x, y - 1)] + Minus[MinusIndex(x, y - 1)] + RowCnt[y - 1];
        MinY = y - 1;
    }
    ++y;
    int t1;
    int m1;
    while(y < N) {
        t1 = Adds[AddsIndex(x, y)] + Minus[MinusIndex(x, y)] + RowCnt[y];
        if (t1 == 0) return y;
        else if (t1 < MinConficts) {
            MinConficts = t1;
            MinY = y;
        }
        ++y;
    }
    y = NQueen[x] - 1;
    while(y >= 0) {
        t1 = Adds[AddsIndex(x, y)] + Minus[MinusIndex(x, y)] + RowCnt[y];
        if (t1 == 0) return y;
        else if (t1 < MinConficts) {
            MinConficts = t1;
            MinY = y;
        }
        --y;
    }
    return MinY;
}

void MinConfictsHC(int N){
    int change = 1;
    int cnt = 0;
    ResetOrInit(N);
    clock_t t1 = clock();
    // cout << t1 << endl;
    while(change != 0 || cnt > 100) {
        change = 0;
        for(int i = 0; i < N; i++) {
            if (inConflict(i, NQueen[i]) > 3) {
                int newY = FindMinConfictY(i);
                --Adds[AddsIndex(i, NQueen[i])];
                --Minus[MinusIndex(i, NQueen[i])];
                --RowCnt[NQueen[i]];
                if (RowCnt[NQueen[i]] == 0) {
                    emptyRows.push_back(NQueen[i]);
                }
                ++Adds[AddsIndex(i, newY)];
                ++Minus[MinusIndex(i, newY)];
                ++RowCnt[newY];
                emptyRowsTemp = find(emptyRows.begin(), emptyRows.end(), newY);
                if (emptyRowsTemp != emptyRows.end()) {
                    emptyRows.erase(emptyRowsTemp);
                }
                NQueen[i] = newY;
                ++change;
            }
        }
        ++cnt;
    }
    if (change == 0){
        cout << N << " Queen: Elapse time is: " << clock() - t1 << " ms\n";
    }
    else cout << "False" << endl;
}

void SmallMinConfictsHC(int N) {
    int change = 1;
    int cnt = 0;
    ResetOrInit(N);
    clock_t t1 = clock();
    // cout << t1 << endl;
    while(change != 0 || cnt > 100) {
        change = 0;
        for(int i = 0; i < N; i++) {
            if (inConflict(i, NQueen[i]) > 3) {
                int newY = SmallFindMinConfictY(i);

                --Adds[AddsIndex(i, NQueen[i])];
                --Minus[MinusIndex(i, NQueen[i])];
                --RowCnt[NQueen[i]];
                ++Adds[AddsIndex(i, newY)];
                ++Minus[MinusIndex(i, newY)];
                ++RowCnt[newY];

                NQueen[i] = newY;
                ++change;
            }
        }
        ++cnt;
    }
    if (change == 0){
        cout << N << " Queen: Elapse time is: " << clock() - t1 << " ms\n";
    }
    else cout << "False" << endl;
}

int main() {
    vector<int> list{10, 50, 100, 1000, 10000, 100000, 1000000, 10000000, 20000000, 30000000, 40000000, 50000000, 100000000};
    for(auto i:list) {
        N = i;
        if(N > 1000) {
            MinConfictsHC(N);
        }
        else {
            SmallMinConfictsHC(N);
        }
        cout << endl;
    }
}