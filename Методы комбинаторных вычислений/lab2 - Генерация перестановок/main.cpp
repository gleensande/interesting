#include <iostream>
#include <vector>

#define N 5

using namespace std;

void print_transposition( vector <int> transposition) {
    for (int i = 0; i < transposition.size(); i++) {
        cout << transposition[i];
    }
    cout << endl;
}


void print_TTS(vector <vector <int> > TTS) {
    for (int i = 0; i < TTS.size(); i++) {
        cout << i + 1 << ". "; 
        print_transposition(TTS[i]);
    }
    cout << endl;
}

vector<int> get_odd_transposition(int s, int pos_odd, int n, vector<int>& prev_transposition) {
    vector <int> new_transposition(s);
    new_transposition[pos_odd] = n;
    for (int j = 0, k = 0; j < s; j++) {
        if (j != pos_odd) {
            new_transposition[j] = prev_transposition[k];
            k++;
        }
    }

    return new_transposition;
}

vector<int> get_even_transposition(int s, int pos_even, int n, vector<int>& prev_transposition) {
    vector <int> new_transposition(s);
    new_transposition[pos_even] = n;
    for (int j = 0, k = 0; j < s; j++) {
        if (j != pos_even) {
            new_transposition[j] = prev_transposition[k];
            k++;
        }
    }

    return new_transposition;
}

int main() {
    // initial element of forming variety
    int n = 1;

    // generate minimal transposition
    vector<int> Pmin = {n};

    // create transpositive transposition sequence
    vector< vector<int> > TTS = { Pmin };


    while(TTS[0].size() != N) {
        // chose next element of forming veriety
        n = n + 1;

        // set new size of transposition = prev size + 1
        int s = TTS[0].size() + 1;
        

        // watch TTS and create new TTS
        vector < vector<int> > new_TTS;
        for (int i = 0; i < TTS.size(); i ++) {
            // set the fist position of adding a new element into transposition
            int pos_even = 0, pos_odd = s - 1;

            // get new transpositions and add it to new TTS
            if (i % 2 == 0) {
                while(pos_odd >= 0) {
                    new_TTS.push_back(get_odd_transposition(s, pos_odd, n, TTS[i]));
                    pos_odd -= 1;
                }
            } else {
                while(pos_even < s) {
                    new_TTS.push_back(get_even_transposition(s, pos_even, n, TTS[i]));
                    pos_even += 1;
                }
            }

        }

        // now work with new TTS
        TTS = new_TTS;
    }

    print_TTS(TTS);
    


    return 0;
}