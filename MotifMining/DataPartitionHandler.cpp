#include "DataPartition.h"
#include "DataPartitionHandler.h"
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

DataPartitionHandler::DataPartitionHandler() {
    // Your initialization goes here
}
void DataPartitionHandler::startFirstPhase(const int32_t block) {
    // Your implementation goes here
//    mS.Search(block);
    printf("startFirstPhase\n");
  }
void DataPartitionHandler::setRange(const int32_t startBlock, const int32_t endBlock) {
    // Your implementation goes here
    printf("setRange%d-%d\n",startBlock,endBlock);
}
