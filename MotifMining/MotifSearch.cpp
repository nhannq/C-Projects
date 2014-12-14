/* 
 * File:   MotifSearch.cpp
 * Based on http://www.cs.ucr.edu/~mueen/DAME/index.html
*/

#include "MotifSearch.h"

double **Dist;
double **data;
long long *ind;
int iRef;

/*per block variables*/
double **data1;
double **data2;
long long *ind1;
long long *ind2;
double **Dist1;
double **Dist2;
long long *locations1, * locations2;
long long pS1, pS2;

MotifSearch::MotifSearch() {
    countLoads = 0;
    countDists = 0;
    blocks = 0;
    cumLoads = 0;
    cumDists = 0;
}

MotifSearch::MotifSearch(const MotifSearch& orig) {
}

MotifSearch::~MotifSearch() {
}

/* Calculates the distance between two time series x and y. If the distance is
larger than the best so far (bsf) it stops computing and returns the approximate
distance. To get exact distance the bsf argument should be omitted.*/

double MotifSearch::distance(double *x, double *y, int length, double best_so_far = INF) {
    int i;
    double sum = 0;
    double bsf2 = best_so_far*best_so_far;

    countDists++;

    for (i = 0; i < length && sum < bsf2; i++)
        sum += (x[i] - y[i])*(x[i] - y[i]);
    return sqrt(sum);
}

/*Comparison function for qsort function. Compares two time series by using their
distances from the reference time series. */
int comp1(const void *a, const void *b) {
    long long *x = (long long *) a;
    long long *y = (long long *) b;

    if (Dist[iRef][*x] > Dist[iRef][*y])
        return 1;
    else if (Dist[iRef][*x] < Dist[iRef][*y])
        return -1;
    else
        return 0;
}

void MotifSearch::error(int id) {
    if (id == 1)
        printf("ERROR : Memory can't be allocated!!!\n\n");
    else if (id == 2)
        printf("ERROR : File not Found!!!\n\n");
    else if (id == 3)
        printf("ERROR : Can't create Output File!!!\n\n");
    else if (id == 4)
        printf("ERROR : Invalid Number of Arguments!!!\n\n");
    else if (id == 6)
        printf("ERROR : Can't create page descriptors!!!\n\n");
    else if (id == 7)
        printf("ERROR : Page has been corrupted!!!\n\n");
    system("PAUSE");
    exit(1);

}

/*
 * load data from 1 block to main memory
 * read data from block p
 * store z-normalized TSs in data and theirs id in block in ind
 * return the number of read TSs
 */
long long MotifSearch::pageLoad(double **_data, long long *_ind, int p) {

    double d;
    long long i = 0, j = 0;
    double ex, ex2, mean, std;
    char tmp[5], fname[100];

    ex = ex2 = 0;

    countLoads++;

    std::sprintf(tmp, "%d", p + blocks);
    strcpy(fname, dirName);
    strcat(fname, tmp);
    strcat(fname, ".txt");
    printf("%s\n", fname);

    FILE * fp = fopen(fname, "r");
    if (fp == NULL)
        error(2);

    while (fscanf(fp, "%lf", &d) != EOF && i < maxPageSizeTS) {
        _data[i][j] = d;
        //        printf("%lf-",_data[i][j]);
        ex += d;
        ex2 += d*d;
        if (j == LENGTH - 1) {
            mean = ex = ex / LENGTH;
            ex2 = ex2 / LENGTH;
            std = sqrt(ex2 - ex * ex);
            for (int k = 0; k < LENGTH; k++)
                _data[i][k] = (_data[i][k] - mean) / std; //z-normalize
            ex = ex2 = 0;
            _ind[i] = i;
            i++;
            j = 0;
            char c;
            //    while(fscanf(fp,"%c",&c) != EOF)
            //    if( c == '\n')
            //      break;


        } else
            j++;
    }
    //    printf("\ni - j : %d - %d\n",i,j);

    fclose(fp);
    return i;

}

void MotifSearch::computeDist(double ** _data, double ** _Dist, long long pS, long long * _locations, int flag) {
    double d;
    long long i, j;
    int r;
    double ex, ex2, mean, std;


    i = 0;
    j = 0;
    ex = ex2 = 0;

    for (r = 0; r < MAXREF; r++) {
        if (flag == 0)
            r = iRef;

        for (i = 0; i < pS; i++) {


            if (_locations[i] == refs[r]) {
                _Dist[r][i] = INF;
                continue;
            }
            d = distance(_data[i], dref[r], LENGTH);
            _Dist[r][i] = d;
            ex += d;
            ex2 += d*d;
            if (labs(_locations[i] - refs[r]) < clear) continue;
            if (d < bsf) {
                bsf = d;
                loc1 = _locations[i];
                loc2 = refs[r];
                memcpy(ts1, _data[i], sizeof (double)*LENGTH);
                memcpy(ts2, dref[r], sizeof (double)*LENGTH);
            }

        }

        ex = ex / (pS - 1);
        ex2 = ex2 / (pS - 1);
        std = sqrt(ex2 - ex * ex);

        rInd[r] = r;
        stdRef[r] = std;
        for (; i < maxPageSizeTS; i++)
            _Dist[r][i] = INF;

        if (flag == 0)
            break;

    }
}

void MotifSearch::sortdata(double **_data, long long *_ind, double ** _Dist, long long pS, long long * locations) {

    Dist = _Dist;
    data = _data;
    ind = _ind;

    computeDist(data, Dist, pS, locations, 0);
    qsort(ind, pS, sizeof (long long), comp1);
}

/*
 * search the motif in a block
 */
int MotifSearch::searchInBlock(double **_data, double ** _Dist, long long pS, long long * _locations) {
    int motifCount2 = 0;
    double d;
    long long i = 0, j = 0;
    int r = 0;
    long long m = 1;
    long long mid, end;
    long long p;


    while (m < pS) {
        p = 0;

        while (p <= pS) {
            if (p + m > pS)
                mid = pS + 1;
            else
                mid = p + m;

            if (p + 2 * m > pS)
                end = pS + 1;
            else
                end = p + 2 * m;

            if (mid == end)
                break;

            for (i = p; i < mid; i++)
                for (j = mid; j < end; j++) {

                    double lower_bound = 0;

                    if (labs(_locations[i] - _locations[j]) < clear)
                        continue;

                    lower_bound = fabs(_Dist[iRef][j] - _Dist[iRef][i]);
                    if (lower_bound <= bsf) {

                        r = 1;
                        do {
                            lower_bound = fabs(_Dist[r][j] - _Dist[r][i]);
                            r++;
                        } while (r < MAXREF && lower_bound <= bsf);


                        if (r >= MAXREF && lower_bound <= bsf) {

                            d = distance(_data[i], _data[j], LENGTH, bsf);
                            if (d == 0)
                                motifCount2++;
                            if (d < bsf) {
                                bsf = d;
                                loc1 = _locations[i];
                                loc2 = _locations[j];
                                memcpy(ts1, _data[i], sizeof (double)*LENGTH);
                                memcpy(ts2, _data[j], sizeof (double)*LENGTH);
                            }
                        }
                    } else
                        break;

                }

            p = p + 2 * m;
        }

        m = 2 * m;
    }
    return motifCount2;
}

void MotifSearch::takeNsave(int pNo) {
    FILE *fsave, *fdesc;
    long long i = 0, j = 0, k = 0;
    double *ts;
    long long p;
    char tmp[5];
    int lind = 0, rind = 0;



    char fname[100] = "temp/p_";
    std::sprintf(tmp, "%d", pNo);
    strcat(fname, tmp);
    fsave = fopen(fname, "wb");

    i = 0;
    while (i < pS1) {
        fwrite(data1[ind1[i]], sizeof (double), LENGTH, fsave);
        p = ind1[i] + pageBoundaries[pNo - 1];
        locsP[i] = p;
        i++;
    }

    st[pNo] = Dist1[iRef][ind1[0]];
    en[pNo] = Dist1[iRef][ind1[pS1 - 1]];

    fclose(fsave);

    char dname[100] = "temp/p_desc_";
    std::sprintf(tmp, "%d", pNo);
    strcat(dname, tmp);
    fdesc = fopen(dname, "wb");

    fwrite(locsP, sizeof (long long), pS1, fdesc);

    fclose(fdesc);
}

void MotifSearch::searchNsave(int pNo) {
    FILE *fsave, *fdesc;
    long long i = 0, j = 0, k = 0;
    double *ts, lD = 0, sD = 0;
    char tmp[5];
    long long p;

    char fname[100] = "temp/p_";
    std::sprintf(tmp, "%d", pNo);
    strcat(fname, tmp);
    fsave = fopen(fname, "wb");


    i = 0;
    j = 0;
    k = 0;
    while (k < pS1) { //compare the distance of 2 blocks and save the smaller one to the disk

        if (j == pS2 || Dist1[iRef][ind1[i]] < Dist2[iRef][ind2[j]]) {
            lD = Dist1[iRef][ind1[i]]; //smaller distance
            ts = data1[ind1[i]];
            p = ind1[i] + pageBoundaries[pNo - 1];
            i++;
        } else {
            lD = Dist2[iRef][ind2[j]]; //smaller distance
            ts = data2[ind2[j]];
            p = ind2[j] + pageBoundaries[pNo];
            j++;
        }


        fwrite(ts, sizeof (double), LENGTH, fsave); //write to the file

        locsP[k] = p; //the location of this TS in the whole DB

        if (k == 0)
            sD = lD;

        k++;
    }

    fclose(fsave);

    char dname[100] = "temp/p_desc_";
    std::sprintf(tmp, "%d", pNo);
    strcat(dname, tmp);
    fdesc = fopen(dname, "wb");

    fwrite(locsP, sizeof (long long), pS1, fdesc); //save ids of TSs in the 1st block to the disk


    fclose(fdesc);

    st[pNo] = sD; //set the distance interval for the 1st block
    en[pNo] = lD;

    strcpy(fname, "temp/p_");
    std::sprintf(tmp, "%d", pNo + 1);
    strcat(fname, tmp);
    fsave = fopen(fname, "wb");


    k = 0;
    while (k < pS2) { //save the 2nd block to the disk
        if (i == pS1) {
            lD = Dist2[iRef][ind2[j]];
            ts = data2[ind2[j]];
            p = ind2[j] + pageBoundaries[pNo];
            j++;
        } else if (j == pS2 || Dist1[iRef][ind1[i]] < Dist2[iRef][ind2[j]]) {
            lD = Dist1[iRef][ind1[i]];
            ts = data1[ind1[i]];
            p = ind1[i] + pageBoundaries[pNo - 1];
            i++;
        } else {
            lD = Dist2[iRef][ind2[j]];
            ts = data2[ind2[j]];
            p = ind2[j] + pageBoundaries[pNo];
            j++;
        }

        fwrite(ts, sizeof (double), LENGTH, fsave);
        locsP[k] = p;

        if (k == 0)
            sD = lD;

        k++;
    }
    fclose(fsave);


    strcpy(dname, "temp/p_desc_");
    std::sprintf(tmp, "%d", pNo + 1);
    strcat(dname, tmp);
    fdesc = fopen(dname, "wb");

    fwrite(locsP, sizeof (long long), pS2, fdesc); //save ids of TSs in the 2nd block to the disk

    fclose(fdesc);

    st[pNo + 1] = sD; //set the distance interval for the 2nd block
    en[pNo + 1] = lD;
}

long long MotifSearch::load(double **_data, double ** _Dist, long long * _locations, int pNo, int flag) {

    countLoads++;

    double d;
    long long i = 0, j = 0;
    char tmp[5];

    long long pS = pageBoundaries[pNo] - pageBoundaries[pNo - 1];
    FILE *fp;

    char dname[100] = "temp/p_desc_";
    std::sprintf(tmp, "%d", pNo);
    strcat(dname, tmp);
    fp = fopen(dname, "rb");


    for (i = 0; i < pS; i++) {
        fread(&_locations[i], sizeof (long long), 1, fp);
    }

    fclose(fp);

    char fname[100] = "temp/p_";
    std::sprintf(tmp, "%d", pNo);
    strcat(fname, tmp);
    fp = fopen(fname, "rb");

    i = 0;

    while (!feof(fp) && i < maxPageSizeTS) {
        fread(_data[i], sizeof (double), LENGTH, fp);
        i++;
    }

    if (feof(fp))
        i--;
    if (ferror(fp))
        error(1);
    if (i != pS) {
        printf("i is %lld and pS is %lld  and pNo is %d\n\n", i, pS, pNo);
        error(7);
    }
    fclose(fp);


    Dist = _Dist;
    data = _data;
    computeDist(data, Dist, pS, _locations, flag);

    return pS;
}

int MotifSearch::searchPartitions(int start, int mid, int end) {
    long long i, j, l1, l2, iStart = 0;
    int r;
    double d;
    int k, p1, p2;
    int runPg = 0, run = 0;
    double t1, t2;

    if (end - start < 1)
        return 0;

    for (p1 = start; p1 < mid; p1++) {

        if ((st[mid] - en[p1]) > bsf && (mid - start) > 1) //if bsf == 0, we cannot find all motifs, deleted mark "="
            continue;

        t1 = clock();
        pS1 = load(data1, Dist1, locations1, p1, 1);
        t2 = clock();
        diskTime += (t2 - t1) / CLOCKS_PER_SEC;


        if (mid - start == 1) {
            motifCount += searchInBlock(data1, Dist1, pS1, locations1);
            printf("Motif Count %d\n", motifCount);
        }

        iStart = 0;

        for (p2 = mid; p2 < end; p2++) {
            if ((st[p2] - en[p1]) > bsf && (end - mid) > 1)
                break;

            t1 = clock();
            pS2 = load(data2, Dist2, locations2, p2, 1);
            t2 = clock();
            diskTime += (t2 - t1) / CLOCKS_PER_SEC;

            if (end - mid == 1) {
                motifCount += searchInBlock(data2, Dist2, pS2, locations2);
                printf("Motif Count %d\n", motifCount);
            }
            for (i = iStart; i < pS1; i++) {
                l1 = locations1[i];

                for (j = 0; j < pS2; j++) {

                    l2 = locations2[j];

                    double lower_bound = 0;
                    lower_bound = fabs(Dist1[iRef][i] - Dist2[iRef][j]);

                    if (lower_bound <= bsf) {

                        if (labs(l1 - l2) >= clear) {
                            r = 0;
                            do {
                                lower_bound = fabs(Dist1[rInd[r]][i] - Dist2[rInd[r]][j]);
                                r++;
                            } while (r < MAXREF && lower_bound <= bsf);


                            if (r >= MAXREF && lower_bound <= bsf) {
                                d = distance(data1[i], data2[j], LENGTH, bsf);
                                if (d == 0)
                                    motifCount++;
                                if (d < bsf) {
                                    bsf = d;
                                    if (d == 0)
                                        motifCount++;
                                    loc1 = locations1[i];
                                    loc2 = locations2[j];
                                    memcpy(ts1, data1[i], sizeof (double)*LENGTH);
                                    memcpy(ts2, data2[j], sizeof (double)*LENGTH);
                                }
                            }
                        }//end if l1 - l2 >= clear
                    } else {
                        iStart = i;
                        break;
                    }
                }//for j
            }//for i
        }//for p2

    }//for p1

    return 1;
}

void MotifSearch::mergeNsave(int start, int mid, int end) {
    FILE *fsave, *fdesc;
    long long i = 0, j = 0, k = 0, PSz;
    double *ts;
    long long p;
    char tmp[5];
    int lind = start, rind = mid, pind = 0;
    double lD = 0, sD = 0;
    double t1, t2;


    k = 0;

    t1 = clock();

    pS1 = load(data1, Dist1, locations1, lind, 0);
    pS2 = load(data2, Dist2, locations2, rind, 0);

    t2 = clock();
    diskTime += (t2 - t1) / CLOCKS_PER_SEC;


    lind++;
    rind++;

    while (1) {

        char fname[100] = "temp/p_";
        std::sprintf(tmp, "%d", end - start);
        strcat(fname, tmp);
        strcat(fname, "_");
        std::sprintf(tmp, "%d", start + pind);
        strcat(fname, tmp);
        fsave = fopen(fname, "wb");

        k = 0;
        PSz = (pageBoundaries[start + pind] - pageBoundaries[start + pind - 1]);
        while (k < PSz) {
            if (i == pS1 && lind < mid) {
                t1 = clock();
                pS1 = load(data1, Dist1, locations1, lind, 0);
                t2 = clock();
                diskTime += (t2 - t1) / CLOCKS_PER_SEC;

                lind++;
                i = 0;
            }
            if (j == pS2 && rind < end) {
                t1 = clock();
                pS2 = load(data2, Dist2, locations2, rind, 0);
                t2 = clock();
                diskTime += (t2 - t1) / CLOCKS_PER_SEC;

                rind++;
                j = 0;
            }

            if (i == pS1 && j == pS2 && lind == mid && rind == end)
                break;

            if (i == pS1 && lind == mid && j < pS2) {
                lD = Dist2[iRef][j];
                ts = data2[j];
                p = locations2[j];
                //                printf("p-%d", p);
                j++;
            } else if (j == pS2 && rind == end && i < pS1) {
                lD = Dist1[iRef][i];
                ts = data1[i];
                p = locations1[i];
                //                printf("p-%d", p);
                i++;
            } else if (Dist1[iRef][i] < Dist2[iRef][j]) {
                lD = Dist1[iRef][i];
                ts = data1[i];
                p = locations1[i];
                //                printf("p-%d", p);
                i++;
            } else {
                lD = Dist2[iRef][j];
                ts = data2[j];
                p = locations2[j];
                //                printf("p-%d", p);
                j++;
            }


            fwrite(ts, sizeof (double), LENGTH, fsave);
            locsP[k] = p;


            if (k == 0)
                sD = lD;
            k++;
        }

        fclose(fsave);

        char dname[100] = "temp/p_desc_";
        std::sprintf(tmp, "%d", end - start);
        strcat(dname, tmp);
        strcat(dname, "_");
        std::sprintf(tmp, "%d", start + pind);
        strcat(dname, tmp);
        fdesc = fopen(dname, "wb");

        fwrite(locsP, sizeof (long long), PSz, fdesc);

        fclose(fdesc);

        st[start + pind] = sD;
        en[start + pind] = lD;

        pind++;

        if (i == pS1 && j == pS2 && lind == mid && rind == end)
            break;

    }

    if (pind != (end - start)) {
        printf("%ld %ld\n\n", pind, end - start);
        error(7);
    }

    for (k = 0; k < pind; k++) {
        char dname[100] = "temp/p_desc_";
        std::sprintf(tmp, "%d", start + k);
        strcat(dname, tmp);
        remove(dname);

        char fname[100] = "temp/p_";
        std::sprintf(tmp, "%d", start + k);
        strcat(fname, tmp);
        remove(fname);

        char d2name[100] = "temp/p_desc_";
        std::sprintf(tmp, "%d", end - start);
        strcat(d2name, tmp);
        strcat(d2name, "_");
        std::sprintf(tmp, "%d", start + k);
        strcat(d2name, tmp);
        rename(d2name, dname);

        char f2name[100] = "temp/p_";
        std::sprintf(tmp, "%d", end - start);
        strcat(f2name, tmp);
        strcat(f2name, "_");
        std::sprintf(tmp, "%d", start + k);
        strcat(f2name, tmp);
        rename(f2name, fname);
    }
}

//using namespace std;
//#define NUM_THREADS     1
//
//void *PrintHello(void *threadid) {
//    long tid;
//    tid = (long) threadid;
//    cout << "Hello World! Thread ID, " << tid << endl;
////    pthread_exit(NULL);
////    while (1) {
////        printf("ffff");
////    }
//}

//int main() {
//    //        DataPartitionClient2 dParClient;
//    //    dParClient.Connect();
//    pthread_t threads[NUM_THREADS];
//    int rc;
//    int i;
//    for (i = 0; i < NUM_THREADS; i++) {
//        cout << "main() : creating thread, " << i << endl;
//        rc = pthread_create(&threads[i], NULL,
//                startThriftServer, (void *) i);
//        if (rc) {
//            cout << "Error:unable to create thread," << rc << endl;
//            exit(-1);
//        }
//    }
//    pthread_exit(NULL);
//    cout << "test test";
//}

void MotifSearch::Initialize(int32_t _block) {
    long long i, j;
    int r = 0, p = 0;
    double ex, ex2, mean, std;
    double tExecTime, tDiskTime;
    double t1, t2, t11, t22, tt1, tt2;
    blocks = _block;

    //    printf("%d-%d-%d", numOfPages, LENGTH, maxPageSizeTS);

    motifCount = 0;
    bsf = INF;
    i = 0;
    j = 0;
    ex = ex2 = 0;

    t11 = clock();

    clear = 1; //to eliminate the trivial motif

    printf("*********************   ALLOCATING MEMEORY   ********************** \n\n");

    dref = (double **) malloc(MAXREF * sizeof (double *)); //stores the reference TS
    stdRef = (double *) malloc(MAXREF * sizeof (double)); //stores the std of reference TS
    rInd = (int *) malloc(MAXREF * sizeof (int)); //stores the id of a reference TS (1 - MAXREF)
    refs = (long long *) malloc(MAXREF * sizeof (long long)); //stores the id of a reference TS in the whole DB

    if (dref == NULL || stdRef == NULL || rInd == NULL || refs == NULL)
        error(1);

    data1 = (double **) malloc(sizeof (double *)*maxPageSizeTS); //data for block 1 (number of TSs in a block * length of a TS)
    ind1 = (long long *) malloc(sizeof (long long)*maxPageSizeTS); //index of TSs in block 1
    data2 = (double **) malloc(sizeof (double *)*maxPageSizeTS); //data for block 2
    ind2 = (long long *) malloc(sizeof (long long)*maxPageSizeTS); //index of TSs in block 2


    if (data1 == NULL || ind1 == NULL || data2 == NULL || ind2 == NULL)
        error(1);

    for (i = 0; i < maxPageSizeTS; i++) {
        data1[i] = (double *) malloc(sizeof (double)*LENGTH); //each TS has length of LENGTH
        data2[i] = (double *) malloc(sizeof (double)*LENGTH); //each TS has length of LENGTH
        if (data1[i] == NULL || data2[i] == NULL)
            error(1);
    }


    Dist1 = (double **) malloc(MAXREF * sizeof (double *)); //stores the reference distance of TSs in block 1 (MAXREF * number of TSs in a block)
    Dist2 = (double **) malloc(MAXREF * sizeof (double *)); //stores the reference distance of TSs in block 2
    if (Dist1 == NULL || Dist2 == NULL)
        error(1);

    for (r = 0; r < MAXREF; r++) {
        Dist1[r] = (double *) malloc(sizeof (double)*maxPageSizeTS);
        Dist2[r] = (double *) malloc(sizeof (double)*maxPageSizeTS);
        if (Dist1[r] == NULL || Dist2[r] == NULL)
            error(1);

    }

    locations1 = (long long *) malloc(sizeof (long long)*maxPageSizeTS); //stores the location of 1 TS in a block, this is the relative position of this TS in the whole DB
    locations2 = (long long *) malloc(sizeof (long long)*maxPageSizeTS);
    locsP = (long long *) malloc(sizeof (long long)*maxPageSizeTS);

    if (locsP == NULL || locations1 == NULL || locations2 == NULL)
        error(1);


    pageBoundaries = (long long *) malloc(sizeof (long long)*numOfPages * 2); //we assume the main memory can stores 2 blocks, we use this to find the absolute position of a TS in the whole DB
    pageBoundaries[0] = 0; //by sum pageBoundaries[block p] and locations[of a TS in block p]


    st = (double *) malloc(sizeof (double)*numOfPages + 1); //[st,en]: distance interval of a block
    en = (double *) malloc(sizeof (double)*numOfPages + 1);
    if (st == NULL || en == NULL)
        error(1);

    ts1 = (double *) malloc(sizeof (double)*LENGTH); //stores the motif
    ts2 = (double *) malloc(sizeof (double)*LENGTH); //stores the motif

    if (ts1 == NULL || ts2 == NULL)
        error(1);

    pS1 = pageLoad(data1, ind1, 1); //number of TSs in the 1st block
    //    for (int ii = 0; ii < maxPageSizeTS; ii++)
    //        for (int jj = 0; jj < LENGTH; jj++)
    //            printf("%lf-", data1[ii][jj]);
    pageBoundaries[1] = pS1;
    for (i = 0; i < pS1; i++)
        locations1[i] = i + pageBoundaries[0];

    ////////////////////   REFERENCE GENERATION BLOCK       ///////////////////

    printf("*********************   GENERATING REFERENCES   ********************** \n\n");

    printf("Reference time series are: ");
    srand(time(NULL));
    FILE * fref = fopen("temp/ref.txt", "w"); //stores the reference TSs to the disk
    for (r = 0; r < MAXREF; r++) {

        dref[r] = (double *) malloc(sizeof (double)*LENGTH);
        if (dref[r] == NULL)
            error(1);

//        long long random_pick = rand() % pS1; //for the time being
        refs[r] = r*3;//random_pick; //randomly pick a TS
        printf("%ld-", r);
        for (i = 0; i < LENGTH; i++) {
            dref[r][i] = data1[refs[r]][i];
            fprintf(fref, "%lf ", dref[r][i]);
        }
        fprintf(fref, "\n");

    }
    printf("\n");
    fclose(fref);

    iRef = 0; //id of the reference TS which we use to sort the reference distances

    printf("*********************  SORTING : FIRST PASS    ********************** \n\n");

    t1 = clock();

    diskTime = 0;
    p = 1; //start from block 1
    while (p <= numOfPages) {
        if (p != 1) {
            tt1 = clock();
            pS1 = pageLoad(data1, ind1, p);
            tt2 = clock();
            diskTime += (tt2 - tt1) / CLOCKS_PER_SEC;

            pageBoundaries[p] = pageBoundaries[p - 1] + pS1;
            for (i = 0; i < pS1; i++)
                locations1[i] = i + pageBoundaries[p - 1];
        }
        //        for (int ii = 0; ii < maxPageSizeTS; ii++)
        //            for (int jj = 0; jj < LENGTH; jj++)
        //                printf("%lf-", data1[ii][jj]);
        sortdata(data1, ind1, Dist1, pS1, locations1); //sort the 1st block
        //        for (int ii = 0; ii < maxPageSizeTS; ii++)
        //            for (int jj = 0; jj < LENGTH; jj++)
        //                printf("%lf-", data1[ii][jj]);

        if (p + 1 > numOfPages) { //if the number of pages is odd

            takeNsave(p);
            //            printf("In Pass 1: After %d pages : %lf %lld %lld \n", p, bsf, loc1, loc2);
            break;
        }

        tt1 = clock();
        pS2 = pageLoad(data2, ind2, p + 1);
        //                for (int ii = 0; ii < maxPageSizeTS; ii++)
        //            for (int jj = 0; jj < LENGTH; jj++)
        //                printf("%d-", data2[ii][jj]);
        tt2 = clock();
        diskTime += (tt2 - tt1) / CLOCKS_PER_SEC;

        pageBoundaries[p + 1] = pageBoundaries[p] + pS2;
        for (i = 0; i < pS2; i++)
            locations2[i] = i + pageBoundaries[p];
        sortdata(data2, ind2, Dist2, pS2, locations2); //sort the 2nd block

        //        printf("In Pass 1: After %d pages : %lf %lld %lld \n", p + 1, bsf, loc1, loc2);

        searchNsave(p); //save 2 sorted blocks to the disk
        p += 2;
    }

    //    printf("Number of Page Loads : %d\n", countLoads);
    //    printf("Number of Distance Computations : %lld\n", countDists);
    cumLoads += countLoads;
    cumDists += countDists;
    countLoads = 0;
    countDists = 0;
    t2 = clock();
    //    printf("Time taken in the Pass 1 is %lf\n\n", (t2 - t1) / CLOCKS_PER_SEC);
    //    printf("Time taken in disk loading %lf\n\n\n", diskTime);

//    double t221 = clock();
    printf("*********************   SORTING : MERGING PHASE    ********************** \n\n");
    int m = 2;
    int end;
    int mid;
    int stop = 0;

    tExecTime = 0;
    tDiskTime = 0;

    while (m < numOfPages && !stop) {
        t1 = clock();
        p = 1;
        diskTime = 0;
        while (p <= numOfPages) {
            if (p + m > numOfPages)
                mid = numOfPages + 1;
            else
                mid = p + m;

            if (p + 2 * m > numOfPages)
                end = numOfPages + 1;
            else
                end = p + 2 * m;

            if (mid == end)
                break;

            printf("Merging %d , %d and %d , %d \n", p, mid - 1, mid, end - 1);
            mergeNsave(p, mid, end);
            p = p + 2 * m;
        }

        //        printf("In %d Pass : %lf %lld %lld \n", m, bsf, loc1, loc2);
        //        printf("Number of Page Loads : %d\n", countLoads);
        //        printf("Number of Distance Computations : %lld\n", countDists);
        cumLoads += countLoads;
        cumDists += countDists;
        countLoads = 0;
        countDists = 0;
        t2 = clock();
        //        printf("Time taken in the Pass %d is %lf\n\n", m, (t2 - t1) / CLOCKS_PER_SEC);
        //        printf("Time taken in disk loading %lf\n\n\n", diskTime);
        tDiskTime += diskTime;
        tExecTime += (t2 - t1) / CLOCKS_PER_SEC;

        m = 2 * m;
    }

    printf("Time taken in the merging phase is %lf\n\n", tExecTime);
    printf("Time taken in disk loading while Merging is %lf\n\n\n", tDiskTime);

    printf("Motif Distance : %lf\nLocation 1 : %lld\nLocation 2 : %lld\n\n", bsf, loc1, loc2);
    printf("Number of motifs %d\n", motifCount);

    //    printf("Total Page Loads %lld\n", cumLoads);
    //    printf("Total Distance Computations %lld\n", cumDists);

    t22 = clock();
    //    printf("Execution time was %lf\n\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n\n", (t22 - t11) / CLOCKS_PER_SEC);
    //    pthread_exit(NULL);
}

void MotifSearch::Search(int32_t _startBlock, int32_t _endBlock) {

    printf("*********************   SEARCHING PHASE    ********************** \n");
    printf("Searching from block %d to block %d\n", _startBlock, _endBlock);

    int m = 1;
    int stop = 0;

    double tExecTime = 0;
    double tDiskTime = 0;
    double t1, t2;
    int p;
    int end;
    int mid;
    int of;

    //    while (m < numOfPages && !stop) {
    while (m < _endBlock && !stop) {
        t1 = clock();
        diskTime = 0;

        //        p = 1;
        p = _startBlock;

        while (p <= _endBlock) {
            //            while (p <= numOfPages) {
            if (p + m > _endBlock)
                mid = _endBlock + 1;
            else
                mid = p + m;

            if (p + 2 * m > _endBlock)
                end = _endBlock + 1;
            else
                end = p + 2 * m;

            if (mid == end)
                break;

            printf("Searching %d , %d and %d , %d \n", p, mid - 1, mid, end - 1);

            //if( mid == 5 && end == 9 )
            //  of = 21;

            of = searchPartitions(p, mid, end);
            p = p + 2 * m;
        }

        //        printf("\nNumber of motifs %d", motifCount);

        //        printf("\n\n\nIn %d Pass : %lf %lld %lld \n", m, bsf, loc1, loc2);
        //        printf("Number of Page Loads : %d\n", countLoads);
        //        printf("Number of Distance Computations : %lld\n\n\n", countDists);

        t2 = clock();
        //        printf("Time taken in the Pass %d is %lf\n\n", m, (t2 - t1) / CLOCKS_PER_SEC);
        //        printf("Time taken in disk loading %lf\n\n\n", diskTime);

        tDiskTime += diskTime;
        tExecTime += (t2 - t1) / CLOCKS_PER_SEC;

        cumLoads += countLoads;
        cumDists += countDists;
        countLoads = 0;
        countDists = 0;
        m = 2 * m;
    }

    printf("Number of motifs after search step %d\n", motifCount);
    printf("Motif Distance : %lf\nLocation 1 : %lld\nLocation 2 : %lld\n\n", bsf, loc1, loc2);
    //    printf("Time taken in searching is %lf\n\n", tExecTime);
    //    printf("Time taken in disk loading while searching is %lf\n\n\n", tDiskTime);

    //    printf("\n\n\n********************   FINISHED SEARCHING    ********************** \n\n");
}

/*
 * _startThisBlock: the start block in the current machine
 * _endThisBlock: the end block in the current machine
 * _startNextBlock: the start block of other machines
 * _endNextBlock: the end block of other machines
 */
void MotifSearch::CleanBlocks(int32_t _startThisBlock, int32_t _endThisBlock, int32_t _startNextBlock, int32_t _endNextBlock) {
    printf("*********************   SEARCHING PHASE    ********************** \n");
    printf("Cleaning Blocks %d-%d-%d-%d with bsf is", _startThisBlock, _endThisBlock, _startNextBlock, _endNextBlock, bsf);
    long long i, j, l1, l2, iStart = 0;
    int r;
    double d;
    int k, p1, p2;
    int runPg = 0, run = 0;
    double t1, t2;

    int isBreak;
    //1st case: when all blocks from _startNextBlock to _endNextBlock are larger than all blocks from _startThisBlock to _endThisBlock
    if ((_startNextBlock < _endNextBlock) && (_startThisBlock < _endNextBlock)) {
        for (p1 = _endThisBlock; p1 >= _startThisBlock; p1--) {

            if (fabs(st[_startNextBlock] - en[p1]) > bsf) //if the distance between two blocks larger than bsf then we can stop 
                break;

            t1 = clock();
            pS1 = load(data1, Dist1, locations1, p1, 1);
            t2 = clock();
            diskTime += (t2 - t1) / CLOCKS_PER_SEC;

            isBreak = 0;

            for (p2 = _startNextBlock; p2 <= _endNextBlock; p2++) {
                if (fabs(st[p2] - en[p1]) > bsf)
                    break;

                t1 = clock();
                pS2 = load(data2, Dist2, locations2, p2, 1);
                t2 = clock();
                diskTime += (t2 - t1) / CLOCKS_PER_SEC;

                for (i = pS1 - 1; i >= 0; i--) {
                    l1 = locations1[i];

                    for (j = 0; j < pS2; j++) {
                        l2 = locations2[j];

                        double lower_bound = 0;
                        lower_bound = fabs(Dist1[iRef][i] - Dist2[iRef][j]);

                        if (lower_bound <= bsf) {

                            if (labs(l1 - l2) >= clear) {
                                r = 0;
                                do {
                                    lower_bound = fabs(Dist1[rInd[r]][i] - Dist2[rInd[r]][j]);
                                    r++;
                                } while (r < MAXREF && lower_bound <= bsf);


                                if (r >= MAXREF && lower_bound <= bsf) {
                                    d = distance(data1[i], data2[j], LENGTH, bsf);
                                    //                                    if (d == 0)
                                    //                                        motifCount++;
                                    if (d <= bsf) {
                                        bsf = d;
                                        if (d == 0)
                                            motifCount++;
                                        loc1 = locations1[i];
                                        loc2 = locations2[j];
                                        memcpy(ts1, data1[i], sizeof (double)*LENGTH);
                                        memcpy(ts2, data2[j], sizeof (double)*LENGTH);
                                    }
                                }
                            }//end if l1 - l2 >= clear
                        } else {
                            isBreak = 1;
                            break;
                        }
                    }//for j
                    if (isBreak == 1)
                        break;
                }//for i
                if (isBreak == 1)
                    break;
            }//for p2

        }//for p1
        //2nd case: when all blocks from _startNextBlock to _endNextBlock are smaller than all blocks from _startThisBlock to _endThisBlock
    } else if ((_startNextBlock < _endNextBlock) && (_startNextBlock < _endThisBlock)) {
        for (p1 = _startThisBlock; p1 <= _endThisBlock; p1++) {

            if (fabs(st[p1] - en[_endNextBlock]) > bsf) //if the distance between two blocks larger than bsf then we can stop 
                break;

            t1 = clock();
            pS1 = load(data1, Dist1, locations1, p1, 1);
            t2 = clock();
            diskTime += (t2 - t1) / CLOCKS_PER_SEC;

            isBreak = 0;

            for (p2 = _endNextBlock; p2 >= _startNextBlock; p2--) {
                if (fabs(st[p2] - en[p1]) > bsf)
                    break;

                t1 = clock();
                pS2 = load(data2, Dist2, locations2, p2, 1);
                t2 = clock();
                diskTime += (t2 - t1) / CLOCKS_PER_SEC;

                for (i = 0; i < pS1; i++) {
                    l1 = locations1[i];

                    for (j = pS2 - 1; j >= 0; j--) {
                        l2 = locations2[j];

                        double lower_bound = 0;
                        lower_bound = fabs(Dist1[iRef][i] - Dist2[iRef][j]);

                        if (lower_bound <= bsf) {

                            if (labs(l1 - l2) >= clear) {
                                r = 0;
                                do {
                                    lower_bound = fabs(Dist1[rInd[r]][i] - Dist2[rInd[r]][j]);
                                    r++;
                                } while (r < MAXREF && lower_bound <= bsf);


                                if (r >= MAXREF && lower_bound <= bsf) {
                                    d = distance(data1[i], data2[j], LENGTH, bsf);
                                    //                                    if (d == 0)
                                    //                                        motifCount++;
                                    if (d < bsf) {
                                        bsf = d;
                                        if (d == 0)
                                            motifCount++;
                                        loc1 = locations1[i];
                                        loc2 = locations2[j];
                                        memcpy(ts1, data1[i], sizeof (double)*LENGTH);
                                        memcpy(ts2, data2[j], sizeof (double)*LENGTH);
                                    }
                                }
                            }//end if l1 - l2 >= clear
                        } else {
                            isBreak = 1;
                            break;
                        }
                    }//for j
                    if (isBreak == 1)
                        break;
                }//for i
                if (isBreak == 1)
                    break;
            }//for p2

        }//for p1
        //3nd case: when all blocks from _startThisBlock to _endThisBlock belong all blocks from _endThisBlock to _startThisBlock 
    } else if ((_startNextBlock > _endNextBlock)) {
        for (p1 = _endThisBlock; p1 >= _startThisBlock; p1--) {
            //
            //            if (fabs(st[p1] - en[_endNextBlock]) > bsf) //if the distance between two blocks larger than bsf then we can stop 
            //                break;

            t1 = clock();
            pS1 = load(data1, Dist1, locations1, p1, 1);
            t2 = clock();
            diskTime += (t2 - t1) / CLOCKS_PER_SEC;

            isBreak = 0;

            for (p2 = _startNextBlock; p2 <= numOfPages; p2++) {
                if (fabs(st[p2] - en[p1]) > bsf)
                    break;

                t1 = clock();
                pS2 = load(data2, Dist2, locations2, p2, 1);
                t2 = clock();
                diskTime += (t2 - t1) / CLOCKS_PER_SEC;

                for (i = pS1 - 1; i >= 0; i--) {
                    l1 = locations1[i];

                    for (j = 0; j < pS2; j++) {
                        l2 = locations2[j];

                        double lower_bound = 0;
                        lower_bound = fabs(Dist1[iRef][i] - Dist2[iRef][j]);

                        if (lower_bound <= bsf) {

                            if (labs(l1 - l2) >= clear) {
                                r = 0;
                                do {
                                    lower_bound = fabs(Dist1[rInd[r]][i] - Dist2[rInd[r]][j]);
                                    r++;
                                } while (r < MAXREF && lower_bound <= bsf);


                                if (r >= MAXREF && lower_bound <= bsf) {
                                    d = distance(data1[i], data2[j], LENGTH, bsf);
                                    //                                    if (d == 0)
                                    //                                        motifCount++;
                                    if (d < bsf) {
                                        bsf = d;
                                        if (d == 0)
                                            motifCount++;
                                        loc1 = locations1[i];
                                        loc2 = locations2[j];
                                        memcpy(ts1, data1[i], sizeof (double)*LENGTH);
                                        memcpy(ts2, data2[j], sizeof (double)*LENGTH);
                                    }
                                }
                            }//end if l1 - l2 >= clear
                        } else {
                            isBreak = 1;
                            break;
                        }
                    }//for j
                    if (isBreak == 1)
                        break;
                }//for i
                if (isBreak == 1)
                    break;
            }//for p2

            for (p2 = _endNextBlock; p2 > 0; p2--) {
                if (fabs(st[p2] - en[p1]) > bsf)
                    break;

                t1 = clock();
                pS2 = load(data2, Dist2, locations2, p2, 1);
                t2 = clock();
                diskTime += (t2 - t1) / CLOCKS_PER_SEC;

                for (i = 0; i < pS1; i++) {
                    l1 = locations1[i];

                    for (j = pS2 - 1; j >= 0; j--) {
                        l2 = locations2[j];

                        double lower_bound = 0;
                        lower_bound = fabs(Dist1[iRef][i] - Dist2[iRef][j]);

                        if (lower_bound <= bsf) {

                            if (labs(l1 - l2) >= clear) {
                                r = 0;
                                do {
                                    lower_bound = fabs(Dist1[rInd[r]][i] - Dist2[rInd[r]][j]);
                                    r++;
                                } while (r < MAXREF && lower_bound <= bsf);


                                if (r >= MAXREF && lower_bound <= bsf) {
                                    d = distance(data1[i], data2[j], LENGTH, bsf);
                                    //                                    if (d == 0)
                                    //                                        motifCount++;
                                    if (d < bsf) {
                                        bsf = d;
                                        if (d == 0)
                                            motifCount++;
                                        loc1 = locations1[i];
                                        loc2 = locations2[j];
                                        memcpy(ts1, data1[i], sizeof (double)*LENGTH);
                                        memcpy(ts2, data2[j], sizeof (double)*LENGTH);
                                    }
                                }
                            }//end if l1 - l2 >= clear
                        } else {
                            isBreak = 1;
                            break;
                        }
                    }//for j
                    if (isBreak == 1)
                        break;
                }//for i
                if (isBreak == 1)
                    break;
            }//for p2

        }//for p1
    }
    printf("\nNumber of motifs after CleanBlocks step %d\n", motifCount);
    printf("Motif Distance : %lf\nLocation 1 : %lld\nLocation 2 : %lld\n\n", bsf, loc1, loc2);
    return 1;
}
