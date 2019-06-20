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

Test cases for performance must be downloaded. Subsets of Chromosome 1 and Chromosome 20 are used as reference genomes. They can be downloaded from the Human Reference Genome here; name the files `chr1.small.fa`, `chr1.fa`, and `chr20.fa` respectively, and place in the root directory. The query string is included in FASTA format in the file `p.fa` and is taken from Ben Langmead's slides.

The full test suite can then be run with the following command:
```
./run_test.sh
```
