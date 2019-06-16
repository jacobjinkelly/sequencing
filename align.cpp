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
    // p[i..p_len - 1] matches a suffix of p[0..j]
    // the character preceding the suffix is not equal to p[i - 1]
    // otherwise -1
    L[0] = new int[p_len];
    // L[1][i] length of the largest suffix of p[i..p_len - 1]
    // that is also a prefix of p
    // otherwise 0
    L[1] = new int[p_len];

    for (int i = 0; i < p_len; i++) {
        L[0][i] = -1;
        // p[0..j] has length j + 1 and p[i..p_len - 1] has length p_len - i
        // so if p[i..p_len - 1] matches a suffix of p[0..j],
        // we only need to consider j s.t. p[0..j] is at least
        // as long as p[i..p_len - 1]
        for (int j = p_len - i - 1; j < p_len - 1; j++) {

            // check if p[i..p_len - 1] matches suffix of p[0..j]
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
                (p[k] == p[p_len - 1 - j + k]);
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
        L[1][i] = 0;
    }

    L[1][0] = p_len;
    int max_j = 0;
    for (int j = 0; j < p_len - 1; j++) {
        L[0][p_len - n[j]] = j;
        if (n[j] == j + 1) {
            max_j = j + 1;
        }
        L[1][p_len - 1 - j] = max_j;
    }

    // clean up
    delete[] reversed;

    return L;
}

// Return alignments of p in t using Boyer-Moore
vector<int> boyer_moore(string const& p, string const& t,
                        int* char_comps_ptr = NULL,
                        int* num_aligns_ptr = NULL,
                        chrono::duration<double>* elapsed_ptr = NULL) {
    const int p_len = p.length();
    const int t_len = t.length();

    auto start = chrono::high_resolution_clock::now();
    int* r = bad_char_index(p, p_len);
    int** L = good_suffix_index(p, p_len);
    auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = finish - start;
    if (elapsed_ptr != NULL) *elapsed_ptr = elapsed;

    vector<int> aligns;
    int bad_char_shift;
    int good_suffix_shift;
    bool match;
    int char_comps = 0;
    int num_aligns = 0;
    for (int i = 0; i < t_len - p_len + 1; i++) {
        match = true;
        num_aligns++;
        for (int j = p_len - 1; j >= 0; j--) {
            char_comps++;
            if (p[j] != t[j + i]) {
                bad_char_shift = j - r[ALPHABET_MAP[t[j + i]]];
                if (j == p_len - 1) {
                    // special case
                    good_suffix_shift = 1;
                } else {
                    if (L[0][j + 1] != -1) {
                        good_suffix_shift = p_len - 1 - L[0][j + 1];
                    } else {
                        good_suffix_shift = p_len - L[1][j + 1];
                    }
                }
                // subtract 1 since for loop increments i
                i += max(bad_char_shift, good_suffix_shift) - 1;
                match = false;
                break;
            }
        }
        if (match) {
            aligns.push_back(i);
            i += p_len - L[1][1] - 1;
        }
    }

    // free indices
    delete[] r;
    delete[] L[0];
    delete[] L[1];
    delete[] L;

    if (char_comps_ptr != NULL) *char_comps_ptr = char_comps;
    if (num_aligns_ptr != NULL) *num_aligns_ptr = num_aligns;

    return aligns;
}


// Return alignments of p in t using naive matching
vector<int> naive(string const& p, string const& t,
                  int* char_comps_ptr = NULL,
                  int* num_aligns_ptr = NULL) {
    const int p_len = p.length();
    const int t_len = t.length();

    vector<int> aligns;
    bool match;
    int char_comps = 0;
    int num_aligns = 0;
    for (int i = 0; i < t_len - p_len + 1; i++) {
        match = true;
        num_aligns++;
        for (int j = 0; j < p_len; j++) {
            char_comps++;
            if (p[j] != t[j + i]) {
                match = false;
                break;
            }
        }
        if (match) {
            aligns.push_back(i);
        }
    }

    if (char_comps_ptr != NULL) *char_comps_ptr = char_comps;
    if (num_aligns_ptr != NULL) *num_aligns_ptr = num_aligns;

    return aligns;
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

// parse file in FASTA format
// returns the last read in the file
// adapted from: http://rosettacode.org/wiki/FASTA_format#C.2B.2B
string read_fasta(string file) {
    ifstream input(file);

    string line, name, content;

    while(getline(input, line).good()) {
        if (line.empty() || line[0] == '>') { // Identifier marker
            if (!name.empty()) {
                name.clear();
            }
            if (!line.empty()) {
                name = line.substr(1);
            }
            content.clear();
        } else if (!name.empty()) {
            if (line.find(' ') != string::npos) { // Invalid sequence--no spaces allowed
                name.clear();
                content.clear();
            } else {
                content += line;
            }
        }
    }

    // return dynamically allocated string
    char *to_return = (char*)malloc(sizeof(char) * (content.length() + 1));
    strncpy(to_return, content.c_str(), content.length());
    to_return[content.length()] = '\0';

    return to_return;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " [p file] [t file]" << endl;
        return -1;
    }

    string p = read_fasta(argv[1]);
    string t = read_fasta(argv[2]);

    if (p.length() > t.length()) {
        cerr << "[p] must be no longer than [t]" << endl;
        return -2;
    }

    // construct alphabet
    add_alphabet(p);
    add_alphabet(t);

    int bm_char_comps, naive_char_comps, bm_num_aligns, naive_num_aligns;
    chrono::duration<double> bm_index_elapsed;

    auto bm_start = chrono::high_resolution_clock::now();
    vector<int> bm_aligns = boyer_moore(p, t,
                                        &bm_char_comps,
                                        &bm_num_aligns,
                                        &bm_index_elapsed);
    auto bm_finish = chrono::high_resolution_clock::now();
    chrono::duration<double> bm_elapsed = bm_finish - bm_start;

    auto naive_start = chrono::high_resolution_clock::now();
    vector<int> naive_aligns = naive(p, t,
                                     &naive_char_comps,
                                     &naive_num_aligns);
    auto naive_finish = chrono::high_resolution_clock::now();
    chrono::duration<double> naive_elapsed = naive_finish - naive_start;

    if (bm_aligns != naive_aligns) {
        cout << "Correctness Failed." << endl;
        return -1;
    }

    cout << "Boyer-Moore Character Comparisons: " << bm_char_comps << endl;
    cout << "Naive Character Comparisons: " << naive_char_comps << endl;
    cout << "Boyer-Moore Number of Checked Alignmnets: " << bm_num_aligns << endl;
    cout << "Naive Number of Checked Alignments: " << naive_num_aligns << endl;
    cout << "Boyer-Moore Indexing Time: " << bm_index_elapsed.count() << endl;
    cout << "Boyer-Moore Total Time: " << bm_elapsed.count() << endl;
    cout << "Naive Total Time: " << naive_elapsed.count() << endl;

    return 0;
}
