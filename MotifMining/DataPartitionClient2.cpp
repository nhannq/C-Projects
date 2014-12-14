/* 
 * File:   DataPartitionClient2.cpp
 * Author: nhannguyen
 * 
 * Created on April 25, 2013, 1:19 AM
 */

#include "DataPartitionClient2.h"

DataPartitionClient2::DataPartitionClient2() {
}

DataPartitionClient2::DataPartitionClient2(const DataPartitionClient2& orig) {
}

DataPartitionClient2::~DataPartitionClient2() {
}

void DataPartitionClient2::Connect() {
  shared_ptr<TTransport> socket(new TSocket("localhost", 9898));
  shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  DataPartitionClient client(protocol);

  try {
    transport->open();

    client.setRange(1,1);
    transport->close();
  } catch (TException &tx) {
    printf("ERROR: %s\n");
  }
}

void DataPartitionClient2::callSearch(int32_t _block) {
  shared_ptr<TTransport> socket(new TSocket("localhost", 9898));
  shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  DataPartitionClient client(protocol);

  try {
    transport->open();

    client.startFirstPhase(_block);
    transport->close();
  } catch (TException &tx) {
    printf("ERROR: %s\n");
  }
}
