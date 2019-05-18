#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cstring>

using namespace std;

// creates index of p for bad character rule
int* bad_char_index(string const& p, int const p_len);

// naively preprocess s in quadratic time
int* naive_preprocess(string const& s, int const s_len);

// "fundamental preprocessing" as described at https://web.cs.ucdavis.edu/~gusfield/cs224f09/znotes.pdf
int* preprocess(string const& s, int const s_len );

// naively creates index of p for good suffix rule
int** naive_good_suffix_index(string const& p, int const p_len);

// creates index of p for good suffix rule
int** good_suffix_index(string const& p, int const p_len);

// Return alignments of p in t using Boyer-Moore
vector<int> boyer_moore(string const& p, string const& t);

// Return alignments of p in t using naive matching
vector<int> naive(string const& p, string const& t);

// check if s only contains characters in ALPHABET
bool check_alphabet(string const& s);

// print elements of vector to stdout
void print_vec(vector<int> const& input);

// print elements of array to stdout
void print_arr(int* arr, int len);
