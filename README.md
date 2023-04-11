# CSQV

This repository contains all code and data needed to reproduce the results presented in [1].

The repository includes a C++ implementation of the Merkle R-tree [2]
and the corresponding algorithms for spatial query verification.

The project uses LibreSSL (https://www.libressl.org) for SHA-256 hash computations,
libmorton (https://github.com/Forceflow/libmorton) for computing Z-orders in 2D
and Vince's CSV parser (https://github.com/vincentlaucsb/csv-parser) for reading CSV files.

## References

[1] Loporchio, Matteo, et al. "Authenticating spatial queries on blockchain systems." IEEE Access 9 (2021): 163363-163378.

[2] Yang, Yin, et al. "Authenticated indexing for outsourced spatial databases." The VLDB Journal 18 (2009): 631-648.
