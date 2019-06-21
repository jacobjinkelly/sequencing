# sequencing [![Build Status](https://travis-ci.org/jacobjinkelly/sequencing.svg?branch=master)](https://travis-ci.org/jacobjinkelly/sequencing)

A repository of algorithms for genomic sequencing.

# Algorithms

## Boyer-Moore

The repository currently consists of an implementation of the [Boyer-Moore](https://en.wikipedia.org/wiki/Boyer%E2%80%93Moore_string-search_algorithm) algorithm for exact alignment of a sequencing read to a reference genome. The implementation uses a linear-time algorithm as described by [Gusfield](https://web.cs.ucdavis.edu/~gusfield/cs224f09/) (lecture notes [4](https://web.cs.ucdavis.edu/~gusfield/cs224f09/znotes.pdf) & [5](https://web.cs.ucdavis.edu/~gusfield/cs224f09/bnotes.pdf])) to construct indexes of the query string used for the good suffix and bad character rules.

# Tests

The full test suite consists of tests for correctness and performance of the implementation of the Boyer-Moore string matching algorithm, using the [Naïve](https://en.wikipedia.org/wiki/String-searching_algorithm#Na%C3%AFve_string_search) string matching algorithm as a baseline comparison.

Test cases for correctness are available in `tests`. The following command will convert these test cases into the FASTA format required to run the full test suite:
```
./format_test.sh
```

Test cases for performance must be downloaded. The reference genomes are from the Dec. 2013 (GRCh38/hg38) assembly of the human genome; chromosomes 1 and 20 were used and can be downloaded [here](http://hgdownload.cse.ucsc.edu/goldenPath/hg38/chromosomes/chr1.fa.gz) and [here](http://hgdownload.cse.ucsc.edu/goldenPath/hg38/chromosomes/chr20.fa.gz) respectively; place the (decompressed) downloaded files in the root directory of the repository. The query string is included in FASTA format in the file `p.fa` and is taken from Ben Langmead's slides.

The full test suite can then be run with the following command:
```
./run_test.sh
```

# Acknowledgements

[Ben Langmead's course](http://www.langmead-lab.org/teaching-materials/) was immensely helpful, and was a guide throughout developing this repository and learning about genomic sequencing.
