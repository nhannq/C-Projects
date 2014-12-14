#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <signal.h>
#include <iostream>
#include <string.h>
#include <pthread.h>

#include "MotifSearch.h"                                                  
#include "libdb.h"
#include "DataPartition.h"
#include "DataPartitionHandler.h"
#include "DataPartitionClient2.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace ::motifPDM;

int motifCount;

void startThriftServer(void *threadid) {
    //start thrift server    
    int port = 9898;
    DataPartitionHandler* dPH = (DataPartitionHandler*) threadid;
    printf("Start thrift server at port%d", port);
    shared_ptr<DataPartitionHandler> handler(dPH);
    shared_ptr<TProcessor> processor(new DataPartitionProcessor(handler));
    shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

    TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
    server.serve();
}

int main(int argc, char * argv[]) {
    double t1, t2, t3;
    t1 = clock();
    MotifSearch mS;
    //    printf("testtest\n");
    DataPartitionHandler* dPH = new DataPartitionHandler();
    dPH->mS.LENGTH = atoi(argv[3]); //length of motif
    //    printf("length%d", dPH->mS.LENGTH);
    dPH->mS.MAXREF = 8; //number of reference TS
    dPH->mS.numOfPages = atol(argv[2]); //number of blocks
    dPH->mS.maxPageSizeTS = atol(argv[4]); //number of TS in a block
    strcpy(dPH->mS.dirName, argv[1]);
    printf("Start\n");
    FILE * fp = fopen("config.properties", "r");
    int _isCluster = 0;
    int _startBlock = 1;
    int _endBlock = 100;
    int _startThisBlock = 76;
    int _endThisBlock = 100;
    int _startNextBlock = 76;
    int _endNextBlock = 100;
    fscanf(fp, "%d", &_isCluster);
    fscanf(fp, "%d", &_startBlock);
    fscanf(fp, "%d", &_endBlock);
    fscanf(fp, "%d", &_startThisBlock);
    fscanf(fp, "%d", &_endThisBlock);
    fscanf(fp, "%d", &_startNextBlock);
    fscanf(fp, "%d", &_endNextBlock);

    dPH->mS.Initialize(0);
    t2 = clock();
    if (_isCluster == 0) {
        printf("Start motif searching in one node with %d-%d\n",_startBlock, _endBlock);
        dPH->mS.Search(_startBlock, _endBlock);
    } else {
        printf("Start motif searching in a cluster with %d-%d-%d-%d\n", _startThisBlock, _endThisBlock, _startNextBlock, _endNextBlock);
        dPH->mS.Search(_startThisBlock, _endThisBlock);
        dPH->mS.CleanBlocks(_startThisBlock, _endThisBlock, _startNextBlock, _endNextBlock);
    }

    t3 = clock();
    printf("Time taken by the initialization step is %lf\n\n", (t2 - t1) / CLOCKS_PER_SEC);
    printf("Time taken by the search step is %lf\n\n", (t3 - t2) / CLOCKS_PER_SEC);
    printf("Time taken is %lf\n\n", (t3 - t1) / CLOCKS_PER_SEC);
    //    printf("\ntest\n");
    DataPartitionClient2 dParClient;
    //    dParClient.callSearch(0);

    //        dParClient.Connect();
    //    pthread_t thread1, thread2;
    //    pthread_create(&thread1, NULL, startThriftServer, (void *) dPH);
    //    dPH.mS.Search();
    //    pthread_create(&thread2, NULL, motifSearch, (void *) NULL);
    //    pthread_join(thread1, NULL);
    //    pthread_exit(NULL);
    //
    //
    //    //    pthread_join( thread, NULL);
    //    //   system("PAUSE");
    //
}