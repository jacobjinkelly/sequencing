#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char **argv) {
    // source: http://rosettacode.org/wiki/FASTA_format#C.2B.2B

    if (argc <= 1) {
        cerr << "Usage: " << argv[0] << " [infile]" << endl;
        return -1;
    }

    ifstream input(argv[1]);
    if (!input.good()) {
        cerr << "Error opening '" << argv[1] << "'. Bailing out." << endl;
        return -1;
    }

    string line, name, content;
    int count = 0;

    while(getline(input, line).good()) {
        if (line.empty() || line[0] == '>') { // Identifier marker
            if (!name.empty()) { // Print out what we read from the last entry
                cout << name << " : " << content << endl;
                count++;
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
    if(!name.empty()) { // Print out what we read from the last entry
        cout << name << " : " << content << endl;
        count++;
    }

    cout << "Number of reads: " << count << endl;

    return 0;
}
