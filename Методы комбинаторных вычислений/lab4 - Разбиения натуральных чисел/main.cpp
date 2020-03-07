#include <iostream>
#include <vector>

#define A 21    // to count fragmentations
#define B 9     // to view fragmentations
#define M -1    // number of parts, -1 for all

using namespace std;

// count fragmentations of n by k parts
// all fragmentations = p(n, n)
int count_fragmentations(int n, int k) {
    if (k == 0) {
        if (n == 0) {
            return 1;
        }

        return 0;
    }

    if (k > n) {
        return count_fragmentations(n, n);
    }

    return count_fragmentations(n, k - 1) + count_fragmentations(n - k, k);
}

// create new fragmentation with Gindenburg algorithm
// by previous fragmentation and the whole number
vector<int> get_next_fragmentation_Gindenburg(vector<int> prev, int n) {
    int m = prev.size();
    vector<int> p = prev;

    if (p[m - 1] - p[0] <= 1) {
        for (int i = 0; i < m; i++) {
            p[i] = 1;
        }
        p.push_back(n - m);

        return p;
    }

    int i;
    for (i = m - 1; i >= 0; i--) {
        if (p[m - 1] - p[i] >= 2) {
            break;
        }
    }

    ++p[i];
    for (int j = i + 1; j <= m - 2; j++) {
        p[j] = p[i];
    }

    p[m - 1] = n;
    for (int k = 0; k <= m - 2; k++) {
        p[m - 1] -= p[k];
    }
    
    return p;
}

// print fragmentation in Gindenburg form
void print_gindenburg(vector<int> p, int i) {
    cout << "p_" << i << " = { ";
    for (int e : p) {
        cout << e << " ";
    }
    cout << "}" << endl;
}

// Gindenburg algorithm
void Gindenburg(int n, int m) {
    vector<int> p_prev = {n};
    if (p_prev.size() == m || m == -1) {
        print_gindenburg(p_prev, 1);
    }

    for (int i = 0; i < count_fragmentations(n, n) - 1; i++) {
        vector<int> p = get_next_fragmentation_Gindenburg(p_prev, n);
        
        if (p.size() == m || m == -1) {
            print_gindenburg(p, i + 2);
        }

        p_prev = p;
    }

    cout << endl;
}

// get index of last non zero element
int i_last_non_zero(vector<int> k, int before) {
    for (int i = before - 1; i >= 0; i--) {
        if (k[i] != 0) {
            return i;
        }
    }

    return -1;
}

// get the whole number by fragmentation in Ehrlich form
int num(vector<int> fragmentation, int n) {
    int num = 0;
    for (int i = 0; i < fragmentation.size(); i++) {
        if (fragmentation[i] != 0) {
            num += (n - i) * fragmentation[i];
        }
    }

    return num;
}

// create new fragmentation with Ehrlich algorithm
// by previous fragmentation and the whole number
vector<int> get_next_fragmentation_Ehrlich(vector<int> k_prev, int n) {
    vector<int> k = k_prev;

    int m = i_last_non_zero(k, k.size());

    if (k[m] > 1) {
        int ost = k[m] * (n - m) - (n - m + 1);
        k[m] = 0;
        k[m - 1] += 1;
        k[k.size() - 1] = ost;
    } else if (k[m] == 1) {
        int prev_non_zero = i_last_non_zero(k, m);
        k[prev_non_zero - 1] += 1;
        k[prev_non_zero] = 0;
        k[m] = 0;
        k[k.size() - 1] += n - num(k, n);
    }
            
    return k;
}

// print fragmentation in Ehrlich form
void print_ehrlich(vector<int> p, int i, int n) {
    cout << "p_" << i << " = { ";
    for (int j = 0; j < p.size(); j++) {
        if (p[j] != 0) {
            cout << p[j] << " • " << n - j << " + ";
        }
    }
    cout << "\b\b}" << endl;
}

// count part in fragmentation in Ehrlich form
int count_parts_ehrlich(vector<int> p) {
    int parts = 0;
    for (int e : p) {
        parts += e;
    }

    return parts;
}

// Ehrlich algorithm
void Ehrlich (int n, int m) {
    vector<int> p(n, 0);
    p[n - 1] = n;
    if (count_parts_ehrlich(p) == m || m == -1) {
        print_ehrlich(p, 1, n);
    }

    for (int i = 0; i < count_fragmentations(n, n) - 1; i++) {
        p = get_next_fragmentation_Ehrlich(p, n);

        if (count_parts_ehrlich(p) == m || m == -1) {
            print_ehrlich(p, i + 2, n);
        }
    }

    cout << endl;
}

int main() {
    cout << "p[" << A << "][" << A << "] = " << count_fragmentations(A, A) << endl << endl;

    cout << "Gindenburg algorithm:" << endl;
    Gindenburg(B, M);

    cout << "Ehrlich algorithm:" << endl;
    Ehrlich(B, M);

    return 0;
}