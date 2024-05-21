// query.c ... query scan functions
// part of Multi-attribute Linear-hashed Files
// Manage creating and using Query objects
// Last modified by John Shepherd, July 2019

#include "defs.h"
#include "query.h"
#include "reln.h"
#include "tuple.h"
#include "hash.h"

// A suggestion ... you can change however you like

struct QueryRep {
    Reln rel;       // need to remember Relation info
    Bits known;     // the hash value from MAH
    Bits unknown;   // the unknown bits from MAH
    PageID curpage;   // current page in scan
    int is_ovflow; // are we in the overflow pages?
    Offset curtup;    // offset of current tuple within page
    //TODO
    int page_ids_size;
    int page_ids_index;
    int page_query_index;
    Bits *pages_ids;
    // 绑定当前的位置
    char *cur_t;
    char *query;
};


void recursion(Query q, Bits known, Bits unknown, int pos) {
//    int totalPagenum = npages(q->rel);
    if (unknown == 0 && (known | q->known) >= q->known && (known | q->known) <= (q->known | q->unknown)) {
        if (q->page_ids_size == q->page_ids_index) {
            q->page_ids_size *= 2;
            q->pages_ids = realloc(q->pages_ids, sizeof(Bits) * q->page_ids_size);
        }
        // printf("page: ");
        // showBits(known);
        // printf("\n");
        q->pages_ids[q->page_ids_index] = known;
        q->page_ids_index++;
    } else {
        for (int i = pos - 1; i >= 0; i--) {
            if (bitIsSet(unknown, i)) {
                unknown = unsetBit(unknown, i);
                recursion(q, known, unknown, i);
                known = setBit(known, i);
                recursion(q, known, unknown, i);
                break;
            }
        }
    }
}





// take a query string (e.g. "1234,?,abc,?")
// set up a QueryRep object for the scan

Query startQuery(Reln r, char *q) {
    Query new = malloc(sizeof(struct QueryRep));
    assert(new != NULL);
    // TODO
    // Partial algorithm:
    // form known bits from known attributes
    // form unknown bits from '?' attributes
    // compute PageID of first page
    //   using known bits and first "unknown" value
    // set all values in QueryRep object
    new->rel = r;
    new->known = 0;
    new->unknown = 0;
    new->is_ovflow = 0;
    new->curtup = 0;
    new->query = q;
    new->cur_t = NULL;

    Count nvals = nattrs(r);
    char **vals = malloc(nvals * sizeof(char *));
    assert(vals != NULL);

    ChVecItem *choice_vector = chvec(r);

    tupleVals(q, vals);

    int i, attribute, bit;
    Bits h[nvals];
    for (i = 0; i < nvals; i++) {
        if (strcmp(vals[i], "?") == 0) {
            h[i] = 0;
        } else {
            h[i] = hash_any((unsigned char *) vals[i], (int) strlen(vals[i]));
        }
    }

    for (i = 0; i < MAXCHVEC; i++) {
        attribute = choice_vector[i].att;
        bit = choice_vector[i].bit;
        if (strcmp(vals[attribute], "?") == 0) {
            new->unknown = setBit(new->unknown, i);
        } else if (bitIsSet(h[attribute], bit)) {
            new->known = setBit(new->known, i);
        }
    }

    new->page_ids_size = 2;
    new->page_ids_index = 0;
    new->page_query_index = -1;
    new->pages_ids = malloc(sizeof(Bits) * new->page_ids_size);
    recursion(new, getLower(new->known, depth(r) + 1), getLower(new->unknown, depth(r) + 1), depth(r) + 1);

    return new;
}


Tuple getNextTuplefrompage(Query q) {

    Page current_page = NULL;
    Tuple result = NULL;
    Tuple t = NULL;
    if (q->is_ovflow == 0) {
        current_page = getPage(dataFile(q->rel), q->curpage);
    } else {
        current_page = getPage(ovflowFile(q->rel), q->curpage);
    }
    if (q->curtup == 0) {
        t = pageData(current_page);
    } else {
        t = q->cur_t;
    }

    while (q->curtup < pageNTuples(current_page)) {
        if (tupleMatch(q->rel, t, q->query) == TRUE) {
            q->cur_t = t;
            result = malloc(strlen(t) + 1);
            strcpy(result, t);
            q->curtup++;
            q->cur_t = t + tupLength(t) + 1;
            break;
        }
        q->curtup++;
        t += tupLength(t) + 1;
    }


    if (result == NULL && pageOvflow(current_page) != NO_PAGE) {
        q->is_ovflow = 1;
        q->curtup = 0;
        q->curpage = pageOvflow(current_page);
        free(current_page);
        result = getNextTuplefrompage(q);
        if (result != NULL) {
            return result;
        }
    } else {
        free(current_page);
    }


    return result;
}

// get next tuple during a scan

Tuple getNextTuple(Query q) {

    if (q->page_query_index == -1) {
        q->page_query_index = 0;
        q->curpage = q->pages_ids[q->page_query_index];
    }
    Tuple result = getNextTuplefrompage(q);
    if (result == NULL) {
        q->page_query_index++;
        while (q->page_query_index < q->page_ids_index) {
            q->curpage = q->pages_ids[q->page_query_index];
            q->is_ovflow = 0;
            q->curtup = 0;
            // 太大就跳出了
            if (q->curpage >= npages(q->rel)) {
                break;
            }
            result = getNextTuplefrompage(q);
            if (result != NULL) {
                return result;
            }
            q->page_query_index++;
        }
    }
    return result; // No more matching tuples found.
}

// clean up a QueryRep object and associated data

void closeQuery(Query q) {
    // TODO
    free(q->pages_ids);
    free(q);
}
