#include "align.h"

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
    cout << "Result: " << endl;
    print_vec(naive_alignments);
    print_vec(boyer_moore_alignments);

    return 0;
}
