#include <bits/stdc++.h>

using namespace std;

int N;

vector<int> Minus;
vector<int> Add;

// 添加随机数
default_random_engine e;
uniform_int_distribution<int> NRand;

int initDiag(int N, vector<int> &NQueen)
{
    NRand = uniform_int_distribution<int>(0, N - 1);
    e = default_random_engine(100);
    Minus = vector<int>(2 * N - 1, 0);
    Add = vector<int>(2 * N - 1, 0);
    int t1, t2;
    int res = 0;
    for (int i = 0; i < N; i++)
    {
        t1 = abs(NQueen[i] + i);
        t2 = NQueen[i] - i + N - 1;
        Minus[t2]++;
        Add[t1]++;
        res += (Minus[t2] - 1);
        res += (Add[t1] - 1);
    }
    return res;
}

// 用于计算冲突
int GetConfilict(int row, int col)
{
    return Add[row + col] + Minus[col - row + N - 1];
}

int GetEvaluation(const vector<int> &Ch_Res)
{
    vector<int> minus = vector<int>(2 * N - 1, 0);
    vector<int> add = vector<int>(2 * N - 1, 0);
    int t1, t2;
    int res = 0;
    for (int i = 0; i < N; i++)
    {
        t1 = abs(Ch_Res[i] + i);
        t2 = Ch_Res[i] - i + N - 1;
        minus[t2]++;
        add[t1]++;
        res += (minus[t2] - 1);
        res += (add[t1] - 1);
    }
    return res;
}
int GetEvalution(int x1, int y1, int x2, int y2)
{
    Minus[y1 - x1 + N - 1]--;
    Minus[y2 - x2 + N - 1]--;
    Add[x1 + y1]--;
    Add[x2 + y2]--;
    Minus[y1 - x2 + N - 1]++;
    Minus[y2 - x1 + N - 1]++;
    Add[x2 + y1]++;
    Add[x1 + y2]++;
    int m = 0;
    if (y1 - x1 == y2 - x2 || x1 + y1 == x2 + y2) m = 2;
    return (Minus[y1 - x2 + N - 1] - 1 + //
            Minus[y2 - x1 + N - 1] - 1 + //
            Add[x2 + y1] - 1 +           //
            Add[x1 + y2] - 1) -
           (Minus[y1 - x1 + N - 1] + //
            Minus[y2 - x2 + N - 1] + //
            Add[x1 + y1] +           //
            Add[x2 + y2]) - m;
}

// 以下为大规模的随机化求解算法，用于解决1k以上的问题，对于小规模的问题适用性不高

void Cal(int num, int times)
{
    double total_time = 0;
    N = num;
    int hitCnt = 0;
    for (int t = 0; t < times; t++)
    {
        vector<int> NQueen;
        for (int i = 0; i < N; i++)
            NQueen.push_back(i);
        random_shuffle(NQueen.begin(), NQueen.end());
        int CurBest = initDiag(N, NQueen);
        int eva;
        clock_t t1 = clock();
        int index1;
        while (CurBest != 0)
        {
            int NextBest = CurBest;
            int Cnt = 0;
            for (int i = 0; i < N; i++)
            {
                int Conf = GetConfilict(i, NQueen[i]);
                if (Conf > 2)
                {   
                    Cnt++;
                    while(true) {
                        index1 = NRand(e);
                        while(index1 == i) index1 = NRand(e);
                        eva = GetEvalution(index1, NQueen[index1], i, NQueen[i]);
                        swap(NQueen[index1], NQueen[i]);
                        if (eva < 0) {
                            NextBest = NextBest + eva;
                            break;
                        }
                        else {
                            eva = GetEvalution(index1, NQueen[index1], i, NQueen[i]);
                            swap(NQueen[index1], NQueen[i]);
                        }
                    }
                }
            }
            // cout <<"cnt: " << Cnt << endl;
            // cout << NextBest << "Real" << GetEvaluation(NQueen) << endl;
            // cout << CurBest << endl;
            CurBest = NextBest;
        }
        cout << "The " << t << " time: " << N << " Queen: Elapse time is: " << clock() - t1 << " ms"
             << " TestCnt: " << hitCnt << endl;
        total_time += (clock() - t1);
    }
    cout << "The average time is: " << total_time / (double)times << " ms\n\n";
}

// 原来的算法：用来解决小规模问题

void SmallCal(int num, int times) {
   double total_time = 0;
   N = num;
   for(int t = 0; t < times; t++) {
       vector<int> NQueen;
       for(int i = 0; i < N; i++) NQueen.push_back(i);
       random_shuffle(NQueen.begin(), NQueen.end());
       int CurBest = initDiag(N, NQueen);
       int eva;
       clock_t t1 = clock();
       while(CurBest != 0) {
           int NextBest = CurBest;
           for (int i = 0; i < N -1; i++) {
               for (int j = i + 1; j < N; j++) {
                   eva = GetEvalution(i, NQueen[i], j, NQueen[j]);
                   swap(NQueen[i], NQueen[j]);
                   if (eva < 0) {
                       NextBest = NextBest+eva;
                   } else {
                       eva = GetEvalution(i, NQueen[i], j, NQueen[j]);
                       swap(NQueen[i], NQueen[j]);
                   }
               }
           }
           if (NextBest == CurBest) {
               NQueen.clear();
               for(int i = 0; i < N; i++) NQueen.push_back(i);
               random_shuffle(NQueen.begin(), NQueen.end());
               CurBest = initDiag(N, NQueen);
           }
           else CurBest = NextBest;
       }
       cout <<"The " << t << " time: " <<  N << " Queen: Elapse time is: " << clock() - t1 << " ms\n";
       total_time += (clock() - t1);
   }
   cout <<"The average time is: "  << total_time / (double) times << " ms\n\n";
}



/* 以下为原来的方法

int GetEvaluation(const vector<int> & Ch_Res) {
   vector<int> minus = vector<int>(2 * N - 1, 0);
   vector<int> add = vector<int> (2 * N - 1, 0);
   int t1, t2;
   int res = 0;
   for(int i = 0; i < N; i++) {
       t1 = abs(Ch_Res[i] + i);
       t2 = Ch_Res[i] - i + N - 1;
       minus[t2]++;
       add[t1]++;
       res += (minus[t2] - 1);
       res += (add[t1] - 1);
   }
   return res;
}

void Cal(int num, int times) {
   double total_time = 0;
   N = num;
   for(int t = 0; t < times; t++) {
       vector<int> NQueen;
       for(int i = 0; i < N; i++) NQueen.push_back(i);
       random_shuffle(NQueen.begin(), NQueen.end());
       int CurBest = GetEvaluation(NQueen);
       int eva;
       clock_t t1 = clock();
       while(CurBest != 0) {
           int NextBest = CurBest;
           for (int i = 0; i < N -1; i++) {
               for (int j = i + 1; j < N; j++) {
                   swap(NQueen[i], NQueen[j]);
                   eva = GetEvaluation(NQueen);
                   if (eva < NextBest) {
                       NextBest = eva;
                   } else {
                       swap(NQueen[i], NQueen[j]);
                   }
               }
           }
           if (NextBest == CurBest) {
               NQueen.clear();
               for(int i = 0; i < N; i++) NQueen.push_back(i);
               random_shuffle(NQueen.begin(), NQueen.end());
               CurBest = GetEvaluation(NQueen);
           }
           else CurBest = NextBest;
       }
       cout <<"The " << t << " time: " <<  N << " Queen: Elapse time is: " << clock() - t1 << " ms\n";
       total_time += (clock() - t1);
   }
   cout <<"The average time is: "  << total_time / (double) times << " ms\n\n";
}
*/

int main()
{
    vector<int> num_array{1000, 4000, 5000, 10000, 100000, 500000, 1000000, 2000000, 5000000, 10000000};
    for (auto num : num_array)
    {
        if (num < 1000) SmallCal(num, 5);
        else Cal(num, 5);
    }
}