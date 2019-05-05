#include <iostream>
#include <vector>

using namespace std;

vector<int> naive(string p, string t) {
    // Return alignments of p in t using naive matching
    vector<int> alignments;

    int p_len = p.length();
    int t_len = t.length();
    for (int i = 0; i < t_len - p_len + 1; i++) {
        // check if this alignment works
        bool match = true;
        for (int j = 0; j < p_len; j++) {
            if (p[j] != t[j + i]) {
                match = false;
                break;
            }
        }
        if (match) {
            alignments.push_back(i);
        }
    }

    return alignments;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        cerr << "Usage: [p] [t]" << endl;
        return -1;
    } else if (strlen(argv[1]) > strlen(argv[2])) {
        cerr << "[p] must be no longer than [t]" << endl;
        return -2;
    }

    vector<int> alignments = naive(argv[1], argv[2]);

    for (auto const& i : alignments) {
        cout << i << " ";
    }
    cout << endl;
    return 0;
}
