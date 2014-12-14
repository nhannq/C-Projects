/**
 * Autogenerated by Thrift Compiler (0.9.1)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef DataPartition_H
#define DataPartition_H

#include <thrift/TDispatchProcessor.h>
#include "pdm_types.h"

namespace motifPDM {

class DataPartitionIf {
 public:
  virtual ~DataPartitionIf() {}
  virtual void startFirstPhase(const int32_t block) = 0;
  virtual void setRange(const int32_t startBlock, const int32_t endBlock) = 0;
};

class DataPartitionIfFactory {
 public:
  typedef DataPartitionIf Handler;

  virtual ~DataPartitionIfFactory() {}

  virtual DataPartitionIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(DataPartitionIf* /* handler */) = 0;
};

class DataPartitionIfSingletonFactory : virtual public DataPartitionIfFactory {
 public:
  DataPartitionIfSingletonFactory(const boost::shared_ptr<DataPartitionIf>& iface) : iface_(iface) {}
  virtual ~DataPartitionIfSingletonFactory() {}

  virtual DataPartitionIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(DataPartitionIf* /* handler */) {}

 protected:
  boost::shared_ptr<DataPartitionIf> iface_;
};

class DataPartitionNull : virtual public DataPartitionIf {
 public:
  virtual ~DataPartitionNull() {}
  void startFirstPhase(const int32_t /* block */) {
    return;
  }
  void setRange(const int32_t /* startBlock */, const int32_t /* endBlock */) {
    return;
  }
};

typedef struct _DataPartition_startFirstPhase_args__isset {
  _DataPartition_startFirstPhase_args__isset() : block(false) {}
  bool block;
} _DataPartition_startFirstPhase_args__isset;

class DataPartition_startFirstPhase_args {
 public:

  DataPartition_startFirstPhase_args() : block(0) {
  }

  virtual ~DataPartition_startFirstPhase_args() throw() {}

  int32_t block;

  _DataPartition_startFirstPhase_args__isset __isset;

  void __set_block(const int32_t val) {
    block = val;
  }

  bool operator == (const DataPartition_startFirstPhase_args & rhs) const
  {
    if (!(block == rhs.block))
      return false;
    return true;
  }
  bool operator != (const DataPartition_startFirstPhase_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DataPartition_startFirstPhase_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class DataPartition_startFirstPhase_pargs {
 public:


  virtual ~DataPartition_startFirstPhase_pargs() throw() {}

  const int32_t* block;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class DataPartition_startFirstPhase_result {
 public:

  DataPartition_startFirstPhase_result() {
  }

  virtual ~DataPartition_startFirstPhase_result() throw() {}


  bool operator == (const DataPartition_startFirstPhase_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const DataPartition_startFirstPhase_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DataPartition_startFirstPhase_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class DataPartition_startFirstPhase_presult {
 public:


  virtual ~DataPartition_startFirstPhase_presult() throw() {}


  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _DataPartition_setRange_args__isset {
  _DataPartition_setRange_args__isset() : startBlock(false), endBlock(false) {}
  bool startBlock;
  bool endBlock;
} _DataPartition_setRange_args__isset;

class DataPartition_setRange_args {
 public:

  DataPartition_setRange_args() : startBlock(0), endBlock(0) {
  }

  virtual ~DataPartition_setRange_args() throw() {}

  int32_t startBlock;
  int32_t endBlock;

  _DataPartition_setRange_args__isset __isset;

  void __set_startBlock(const int32_t val) {
    startBlock = val;
  }

  void __set_endBlock(const int32_t val) {
    endBlock = val;
  }

  bool operator == (const DataPartition_setRange_args & rhs) const
  {
    if (!(startBlock == rhs.startBlock))
      return false;
    if (!(endBlock == rhs.endBlock))
      return false;
    return true;
  }
  bool operator != (const DataPartition_setRange_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DataPartition_setRange_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class DataPartition_setRange_pargs {
 public:


  virtual ~DataPartition_setRange_pargs() throw() {}

  const int32_t* startBlock;
  const int32_t* endBlock;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class DataPartition_setRange_result {
 public:

  DataPartition_setRange_result() {
  }

  virtual ~DataPartition_setRange_result() throw() {}


  bool operator == (const DataPartition_setRange_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const DataPartition_setRange_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DataPartition_setRange_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class DataPartition_setRange_presult {
 public:


  virtual ~DataPartition_setRange_presult() throw() {}


  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class DataPartitionClient : virtual public DataPartitionIf {
 public:
  DataPartitionClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) :
    piprot_(prot),
    poprot_(prot) {
    iprot_ = prot.get();
    oprot_ = prot.get();
  }
  DataPartitionClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) :
    piprot_(iprot),
    poprot_(oprot) {
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void startFirstPhase(const int32_t block);
  void send_startFirstPhase(const int32_t block);
  void recv_startFirstPhase();
  void setRange(const int32_t startBlock, const int32_t endBlock);
  void send_setRange(const int32_t startBlock, const int32_t endBlock);
  void recv_setRange();
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class DataPartitionProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<DataPartitionIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (DataPartitionProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_startFirstPhase(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_setRange(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  DataPartitionProcessor(boost::shared_ptr<DataPartitionIf> iface) :
    iface_(iface) {
    processMap_["startFirstPhase"] = &DataPartitionProcessor::process_startFirstPhase;
    processMap_["setRange"] = &DataPartitionProcessor::process_setRange;
  }

  virtual ~DataPartitionProcessor() {}
};

class DataPartitionProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  DataPartitionProcessorFactory(const ::boost::shared_ptr< DataPartitionIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< DataPartitionIfFactory > handlerFactory_;
};

class DataPartitionMultiface : virtual public DataPartitionIf {
 public:
  DataPartitionMultiface(std::vector<boost::shared_ptr<DataPartitionIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~DataPartitionMultiface() {}
 protected:
  std::vector<boost::shared_ptr<DataPartitionIf> > ifaces_;
  DataPartitionMultiface() {}
  void add(boost::shared_ptr<DataPartitionIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void startFirstPhase(const int32_t block) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->startFirstPhase(block);
    }
    ifaces_[i]->startFirstPhase(block);
  }

  void setRange(const int32_t startBlock, const int32_t endBlock) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->setRange(startBlock, endBlock);
    }
    ifaces_[i]->setRange(startBlock, endBlock);
  }

};

} // namespace

#endif
