#ifndef MOTIFSEARCH_H
#define	MOTIFSEARCH_H

#define EXCLENGTH 500
#define INF 999999999999.0    
#include <stdio.h>
#include <cstdio>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <signal.h>
#include <iostream>
#include <string.h>

class MotifSearch {
public:
    MotifSearch();
    MotifSearch(const MotifSearch& orig);
    virtual ~MotifSearch();
    double MotifSearch::distance(double *x, double *y, int length, double best_so_far = INF);
//    int comp1(const void *a, const void *b);
    void error(int id);
    long long pageLoad(double **_data, long long *_ind, int p);
    void computeDist(double ** _data, double ** _Dist, long long pS, long long * _locations, int flag);
    void sortdata(double **_data, long long *_ind, double ** _Dist, long long pS, long long * _locations);
    int searchInBlock(double **_data, double ** _Dist, long long pS, long long * _locations);
    void takeNsave(int pNo);
    void searchNsave(int pNo);
    long long load(double **_data, double ** _Dist, long long * _locations, int pNo, int flag);
    int searchPartitions(int start, int mid, int end);
    void mergeNsave(int start, int mid, int end);
    void Initialize(int32_t _block);
    void Search(int32_t _startBlock, int32_t _endBlock);
    void CleanBlocks(int32_t _startThisBlock, int32_t _endThisBlock, int32_t _startNextBlock, int32_t _endNextBlock);
    int motifCount;

    long long TIMESERIES;
    long long maxPageSizeTS;
    int LENGTH;
    int MAXREF;

        long long cumLoads;
    long long cumDists;

    /* Variables for Input and Output*/

    char dirName[100];

    /*Exclusion variables*/

    long long clear;
    int K;
    double X;
    int blocks;




    /*Global Pointers for sorting and searching */

    long long * locsP;

    long long *pageBoundaries;
    long long numOfPages;
    double * st, * en;

    /*Variables Related to Reference Points */
    double **dref;
    int *rInd;
    double *stdRef;
    long long *refs;

    /*Variables for Output*/
    long long loc1, loc2;
    double bsf;
    double *ts1, *ts2;

    /*Counters*/

    long long countLoads;
    long long countDists;
    double diskTime;
private:


};

#endif	/* MOTIFSEARCH_H */

