#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

const int ALPHABET_SIZE = 4;
const char ALPHABET[] = {'A', 'C', 'T', 'G'};
const map<char, int> ALPHABET_MAP = {{'A', 0}, {'C', 1}, {'T', 2}, {'G', 3}};


// "fundamental preprocessing" as described at https://web.cs.ucdavis.edu/~gusfield/cs224f09/znotes.pdf
int* preprocess(string const& s) {
    const int s_len = s.length();

    int* z = new int{s_len - 1};
    int r = 0;
    int l = 0;

    // compute the base case
    for (z[0] = 0; ((z[0] < s_len - 1) && (s[z[0]] == s[z[0] + 1])); z[0]++) {}
    if (z[0] > 0) {
        r = z[0] + 1;
        l = 1;
    }

    // inductively compute the rest
    for (int k = 1; k < s_len - 1; k++) {
        if (k > r) {
            for (z[k] = 0; ((z[k] < s_len - 1) && (s[z[k]] == s[z[k] + 1])); z[k]++) {}
            if (z[k] > 0) {
                r = k + z[k] - 1;
                l = k;
            }
        } else {
            if (z[k - l + 1] < r - k + 1) {
                z[k] = z[k - l + 1];
            } else {
                for (z[k] = r + 1; ((z[k] + 1 - k < s_len) && (s[z[k]] == s[z[k] + 1 - k])); z[k]++)
                z[k] -= k;
                r = z[k] + k - 1;
                l = k;
            }
        }
    }

    return z;
}


// Return alignments of p in t using Boyer-Moore
vector<int> boyer_moore(string const& p, string const& t) {
    const int p_len = p.length();
    const int t_len = t.length();

    // index for bad character rule
    int r[ALPHABET_SIZE] = {-1};
    for (int i = p_len - 1; i >= 0; i--) {
        if (r[ALPHABET_MAP.at(p[i])] == -1) {
            r[ALPHABET_MAP.at(p[i])] = i;
        }
    }

    vector<int> alignments;
    for (int i = 0; i < t_len - p_len + 1; i++) {
        bool match = true;
        for (int j = p_len - 1; j >= 0; j--) {
            if (p[j] != t[j + i]) {
                i += max(1, j - r[ALPHABET_MAP.at(t[j + i])]) - 1;
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


// Return alignments of p in t using naive matching
vector<int> naive(string const& p, string const& t) {
    const int p_len = p.length();
    const int t_len = t.length();

    vector<int> alignments;
    for (int i = 0; i < t_len - p_len + 1; i++) {
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

// check if s only contains characters in ALPHABET
bool check_alphabet(string const& s) {
    bool check = true;
    for (auto& c : s) {
        bool in_alphabet = false;
        for (auto& a : ALPHABET) {
            if (c == a) {
                in_alphabet = true;
                break;
            }
        }
        if (!in_alphabet) {
            check = false;
            break;
        }
    }
    return check;
}

// print elements of input to stdout
void print(vector<int> const& input) {
    for (auto const& i : input) {
        cout << i << " ";
    }
    cout << endl;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        cerr << "Usage: [p] [t]" << endl;
        return -1;
    } else if (strlen(argv[1]) > strlen(argv[2])) {
        cerr << "[p] must be no longer than [t]" << endl;
        return -2;
    } else if (!(check_alphabet(argv[1]) && check_alphabet(argv[2]))) {
        cerr << "[p], [t] must consist only of characters in the alphabet" << endl;
        return -3;
    }

    vector<int> naive_alignments = naive(argv[1], argv[2]);
    vector<int> boyer_moore_alignments = boyer_moore(argv[1], argv[2]);
    print(naive_alignments);
    print(boyer_moore_alignments);

    return 0;
}
