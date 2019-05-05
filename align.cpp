#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

constexpr int ALPHABET_SIZE = 4;
constexpr char ALPHABET[] = {'A', 'C', 'T', 'G'};
const map<char, int> ALPHABET_MAP = {{'A', 0}, {'C', 1}, {'T', 2}, {'G', 3}};


// creates index of p for bad character rule
int* bad_char_index(string const& p, int const p_len) {
    int* r = new int[ALPHABET_SIZE];
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        r[i] = -1;
    }
    for (int i = p_len - 1; i >= 0; i--) {
        if (r[ALPHABET_MAP.at(p[i])] == -1) {
            r[ALPHABET_MAP.at(p[i])] = i;
        }
    }

    return r;
}

// naively preprocess s in quadratic time
int* naive_preprocess(string const& s, int const s_len) {
    int* z = new int[s_len - 1];

    for (int k = 0; k < s_len - 1; k++) {
        for (z[k] = 0; ((z[k] + k + 1 < s_len) && (s[z[k]] == s[z[k] + k + 1])); z[k]++) {}
    }

    return z;
}


// "fundamental preprocessing" as described at https://web.cs.ucdavis.edu/~gusfield/cs224f09/znotes.pdf
int* preprocess(string const& s, int const s_len ) {
    // TODO: rewrite this to do on the reverse of s, and return the reverse of z

    int* z = new int[s_len - 1];
    int r = 0;
    int l = 0;

    // compute the base case
    for (z[0] = 0; ((z[0] + 1 < s_len) && (s[z[0]] == s[z[0] + 1])); z[0]++) {}
    if (z[0] > 0) {
        r = z[0];
        l = 1;
    }

    // inductively compute the rest
    for (int k = 1; k < s_len - 1; k++) {
        if (k > r) {
            for (z[k] = 0; ((z[k] + k + 1 < s_len) && (s[z[k]] == s[z[k] + k + 1])); z[k]++) {}
            if (z[k] > 0) {
                r = k + z[k] - 1;
                l = k + 1;
            }
        } else {
            if (z[k - l] < r - k + 1) {
                z[k] = z[k - l];
            } else {
                for (z[k] = r + 1; ((z[k] + r - k + 2 < s_len) && (s[z[k]] == s[z[k] + r - k + 2])); z[k]++) {}
                z[k] -= k + 1;
                r = z[k] + k;
                l = k + 1;
            }
        }
    }

    return z;
}


// creates index of p for good suffix rule
int** good_suffix_index(string const& p, int const p_len) {
    int* n = preprocess(p, p_len);

    int** L = new int*[2];
    L[0] = new int[p_len]; // L
    L[1] = new int[p_len]; // l

    // TODO: accumulate L[1]
    for (int i = 0; i < p_len; i++) {
        L[0][i] = 0;
    }

    for (int i = 0; i < p_len - 1; i++) {
        L[0][p_len - n[i] + 1] = i;
    }

    return L;
}

// Return alignments of p in t using Boyer-Moore
vector<int> boyer_moore(string const& p, string const& t) {
    const int p_len = p.length();
    const int t_len = t.length();

    int* r = bad_char_index(p, p_len);

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

    // free indices
    delete r;

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

// print elements of vector to stdout
void print_vec(vector<int> const& input) {
    for (auto const& i : input) {
        cout << i << " ";
    }
    cout << endl;
}

// print elements of array to stdout
void print_arr(int* arr, int len) {
    for (int i = 0; i < len; i++) {
        cout << arr[i] << " ";
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
    int s_len = strlen(argv[1]);
    int* naive_preprocessed = naive_preprocess(argv[1], s_len);
    int* preprocessed = preprocess(argv[1], s_len);
    cout << "naive ";
    print_arr(naive_preprocessed, s_len - 1);
    cout << "fast ";
    print_arr(preprocessed, s_len - 1);

    vector<int> naive_alignments = naive(argv[1], argv[2]);
    vector<int> boyer_moore_alignments = boyer_moore(argv[1], argv[2]);
    print_vec(naive_alignments);
    print_vec(boyer_moore_alignments);

    return 0;
}
