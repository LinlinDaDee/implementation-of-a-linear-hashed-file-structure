# implementation-of-a-linear-hashed-file-structure
## Overview
This project is part of the DBMS Implementation course (COMP9315). The goal is to build a simple implementation of a linear-hashed file structure that uses multi-attribute hashing (MALH). This file structure allows the hashed files to grow as needed and enables all attributes to contribute to the hash value of each tuple.

## Features
1. Multi-attribute Hashing
   Computes the hash value of a tuple using the choice vector for multi-attribute hashing. (tupleHash(Reln r, Tuple t) in tuple.c)
2. Linear Hashing and Insertion
   This feature enhances the current implementation of hashing by introducing dynamic file expansion to ensure efficient storage and retrieval of data. (in reln.c)
   - Trigger Expansion: After every c (c = floor(B/R) ≈ 1024/(10*n)) insertions, a new page is added to the end of the file.
   - Redistribution: The tuples from the “buddy” page (located at index 2^d less than the current number of pages) are redistributed between the old and new pages.
   - Hashing: The redistribution is based on considering d+1 bits of the hash value to determine where each tuple goes.
3. Selection (Querying)
   This feature involves designing and implementing a comprehensive query scanning data structure to enhance the query processing capabilities of the system. (in query.c)
