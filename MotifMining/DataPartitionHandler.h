/* 
 * File:   DataPartitionHandler.h
 * Author: nhannguyen
 *
 * Created on April 25, 2013, 12:57 AM
 */

#ifndef DATAPARTITIONHANDLER_H
#define	DATAPARTITIONHANDLER_H

#include "DataPartition.h"
#include "MotifSearch.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace  ::motifPDM;

class DataPartitionHandler : virtual public DataPartitionIf {
 public:
  DataPartitionHandler();
  void startFirstPhase(const int32_t block); 
  void setRange(const int32_t startBlock, const int32_t endBlock); 
  MotifSearch mS;
};


#endif	/* DATAPARTITIONHANDLER_H */

