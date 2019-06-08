// functions for sequence alignment

#include "align.h"

using namespace std;

int ALPHABET_SIZE = 0;
vector<char> ALPHABET;
map<char, int> ALPHABET_MAP;


// creates index of p for bad character rule
int* bad_char_index(string const& p, int const p_len) {
    // r[i] is the rightmost position of the ith char
    // in the alphabet in p
    int* r = new int[ALPHABET_SIZE];

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        r[i] = -1;
    }
    for (int i = p_len - 1; i >= 0; i--) {
        if (r[ALPHABET_MAP[p[i]]] == -1) {
            r[ALPHABET_MAP[p[i]]] = i;
        }
    }

    return r;
}

// naively "preprocess" s in quadratic time
int* naive_preprocess(string const& s, int const s_len) {
    // n[j] is the length of the longest suffix of the
    // substring s[0..j] that is also a suffix of s
    int* n = new int[s_len - 1];

    for (int j = 0; j < s_len - 1; j++) {
        for (n[j] = 0; ((n[j] <= j) && (s[j - n[j]] == s[s_len - 1 - n[j]])); n[j]++) {}
    }

    return n;
}

// "preprocess" s efficiently
int* preprocess(string const& s, int const s_len ) {
    // z[i] is the length of the longest substring of s
    // that starts at i + 1 and matches a prefix of s
    int* z = new int[s_len - 1];
    int r = -1;
    int l = -1;

    // the Z algorithm as described in Section 1.4 of
    // https://web.cs.ucdavis.edu/~gusfield/cs224f09/znotes.pdf
    for (int k = 0; k < s_len - 1; k++) {
        if (k + 1 > r) {
            for (z[k] = 0; ((z[k] + k + 1 < s_len) && (s[z[k]] == s[z[k] + k + 1])); z[k]++) {}
            if (z[k] > 0) {
                r = k + z[k];
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


// naively creates index of p for good suffix rule
int** naive_good_suffix_index(string const& p, int const p_len) {
    // construct indices as described in
    // https://web.cs.ucdavis.edu/~gusfield/cs224f09/bnotes.pdf
    // L[0] is "L`" in the notes, L[1] is "l`" in the notes
    int** L = new int*[2];
    // L[0][i] = max j such that:
    // j < p_len - 1
    // p[i..p_len - 1] matches a suffix of p[0..j - 1]
    // the character preceding the suffix is not equal to p[i - 1]
    // otherwise -1
    L[0] = new int[p_len];
    // L[1][i] length of the largest suffix of p[i..p_len - 1]
    // that is also a prefix of p
    // otherwise 0
    L[1] = new int[p_len];

    for (int i = 0; i < p_len; i++) {
        L[0][i] = -1;
        // p[0..j - 1] has length j and p[i..p_len - 1] has length p_len - i
        // so if p[i..p_len - 1] matches a suffix of p[0..j - 1],
        // we only need to consider j s.t. p[0..j - 1] is at least
        // as long as p[i..p_len - 1]
        for (int j = p_len - i; j < p_len; j++) {

            // check if p[i..p_len - 1] matches suffix of p[0..j - 1]
            int k;
            for (k = i;
                (k <= p_len - 1) &&
                (p[k] == p[k + j - p_len + 1]);
                k++) {}

            // matches suffix
            if (k == p_len) {
                // check if character preceding suffix unequal to p[i - 1]
                if ((i >= 1) && (i + j - p_len >= 0)) {
                    if (p[i - 1] != p[i + j - p_len]) L[0][i] = j;
                } else {
                    L[0][i] = j;
                }
            }
        }

        L[1][i] = 0;
        // longest suffix of p[i..p_len - 1] is itself,
        // which has length p_len - i
        for (int j = 0; j < p_len - i; j++) {
            // see if suffix of length j + 1 works
            int k;
            for (k = 0;
                (k < j + 1) &&
                (p[k] == p[p_len - j + k]);
                k++) {}

            // suffix matched
            if (k == j + 1) L[1][i] = j + 1;
        }
    }

    return L;
}


// creates index of p for good suffix rule
int** good_suffix_index(string const& p, int const p_len) {
    // reverse p
    char* reversed = new char[p_len + 1];
    for (int i = 0; i < p_len; i++) {
        reversed[i] = p[p_len - 1 - i];
    }
    reversed[p_len] = '\0';

    // do fundamental preprocessing on reversed p
    int* n = preprocess(reversed, p_len);

    // reverse the result
    reverse(n, n + p_len - 1);

    // construct indices as described in
    // https://web.cs.ucdavis.edu/~gusfield/cs224f09/bnotes.pdf
    // L[0] is "L`" in the notes, L[1] is "l`" in the notes
    int** L = new int*[2];
    L[0] = new int[p_len];
    L[1] = new int[p_len];

    for (int i = 0; i < p_len; i++) {
        L[0][i] = -1;
        L[1][i] = -1;
    }

    int i;
    for (int j = 0; j < p_len - 1; j++) {
        i = p_len - n[j];
        L[0][i] = j;

        if (n[j] == j) {
            i = p_len - j;
            L[1][i] = j;
        }
    }

    // clean up
    delete[] reversed;

    return L;
}

// Return alignments of p in t using Boyer-Moore
vector<int> boyer_moore(string const& p, string const& t) {
    const int p_len = p.length();
    const int t_len = t.length();

    int* r = bad_char_index(p, p_len);
    int** L = naive_good_suffix_index(p, p_len);

    vector<int> alignments;
    int bad_char_shift;
    int good_suffix_shift;
    int shift;
    for (int i = 0; i < t_len - p_len + 1; i++) {
        bool match = true;
        for (int j = p_len - 1; j >= 0; j--) {
            if (p[j] != t[j + i]) {
                bad_char_shift = j - r[ALPHABET_MAP[t[j + i]]];
                if (j == p_len - 1) {
                    good_suffix_shift = 1;
                } else {
                    if (L[0][j] != -1) {
                        good_suffix_shift = p_len - 1 - L[0][j];
                    } else {
                        good_suffix_shift = p_len - 1 - L[1][j];
                    }
                }
                shift = max(bad_char_shift, good_suffix_shift);
                i += max(0, shift - 1);
                match = false;
                break;
            }
        }
        if (match) {
            alignments.push_back(i);
            i += p_len - 1 - L[1][0] - 1;
        }
    }

    // free indices
    delete[] r;
    delete[] L[0];
    delete[] L[1];
    delete[] L;

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

// add the characters of s to the alphabet
void add_alphabet(string const& s) {
    for (auto& c : s) {
        bool in_alphabet = false;
        for (auto& a: ALPHABET) {
            if (c == a) {
                in_alphabet = true;
                break;
            }
        }
        if (!in_alphabet) {
            ALPHABET.push_back(c);
            ALPHABET_MAP.insert(pair<char, int>(c, ALPHABET_SIZE));
            ALPHABET_SIZE++;
        }
    }
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
    }
    // construct alphabet
    add_alphabet(argv[1]);
    add_alphabet(argv[2]);

    int p_len = strlen(argv[1]);
    int** naive_good_suffix = naive_good_suffix_index(argv[1], p_len);
    int** good_suffix = good_suffix_index(argv[1], p_len);

    print_arr(naive_good_suffix[0], p_len);
    print_arr(good_suffix[0], p_len);
    print_arr(naive_good_suffix[1], p_len);
    print_arr(good_suffix[1], p_len);
    return 0;
}
