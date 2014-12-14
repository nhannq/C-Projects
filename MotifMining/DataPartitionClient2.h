/* 
 * File:   DataPartitionClient2.h
 * Author: nhannguyen
 *
 * Created on April 25, 2013, 1:19 AM
 */

#ifndef DATAPARTITIONCLIENT2_H
#define	DATAPARTITIONCLIENT2_H

#include "DataPartition.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

using boost::shared_ptr;

using namespace  ::motifPDM;

class DataPartitionClient2 {
public:
    DataPartitionClient2();
    DataPartitionClient2(const DataPartitionClient2& orig);
    virtual ~DataPartitionClient2();
    void Connect();
    void callSearch(int32_t _block);
private:

};

#endif	/* DATAPARTITIONCLIENT2_H */

