#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int solve(string a, string b) {
    int I = (int)a.size() + 1;
    int J = (int)b.size() + 1;

    vector<vector<int>> dp;

    for(int i = 0; i < I; ++i) {
        vector<int> tmp;
        for(int j = 0; j < J; ++j) {
            if(i == 0) {
                tmp.push_back(j);
            }
            else if(j == 0) {
                tmp.push_back(i);
            }
            else {
                tmp.push_back(0);
            }
        }

        dp.push_back(tmp);
    }


    for(int i = 1; i < I; ++i) {
        for(int j = 1; j < J; ++j) {
            if(a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            }
            else {
                dp[i][j] = min(min(dp[i - 1][j], dp[i][j - 1]), dp[i - 1][j - 1]) + 1;
            }
            if(i > 1 && j > 1 && a[i - 1] == b[j - 2] && a[i - 2] == b[j - 1]) {
                dp[i][j] = min(dp[i][j], dp[i - 2][j - 2]  + 1);
            }
        }
    }
    
    return dp[I - 1][J - 1];
}

int main()
{
    string a = "aba";
    string b = "aab";
    
    int A = static_cast<int>(a.size());
    int B = static_cast<int>(b.size());
    
    double ratio = (1 - static_cast<double>(solve(a, b)) / A) * 100;

    cout << "======= RESULT =======" << endl;
    if(ratio < 50) {
        cout << b << " doesn't match " << a << endl;
    }
    else {
        cout << b << " matches " << a << endl;
    }
    cout << "Similarity is : " << ratio << "%" << endl; 

    return 0;
}

