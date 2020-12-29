#include <bits/stdc++.h>

using namespace std;

int N;

vector<int> Minus;
vector<int> Add;
int initDiag(int N, vector<int> &NQueen) {
    Minus = vector<int> (2 * N-1, 0);
    Add = vector<int> (2 * N - 1, 0);
    int t1, t2;
    int res = 0;
    for(int i = 0; i < N; i++) {
        t1 = abs(NQueen[i] + i);
        t2 = NQueen[i] - i + N - 1;
        Minus[t2]++;
        Add[t1]++;
        res += (Minus[t2] - 1);
        res += (Add[t1] - 1);
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

void Cal(int num, int times) {
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
       for(auto k: NQueen) {
           cout << k << " ";
       }
       cout << endl;
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


int main() {
//    vector<int> num_array{5, 50, 100, 200, 300, 400, 500, 1000, 2000, 3000, 4000, 5000, 10000};
//    for(auto num: num_array) {
//        Cal(num, 5);
//    }
    Cal(8, 100);
}