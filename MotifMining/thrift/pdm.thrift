namespace cpp motifPDM

service DataPartition {
  void startFirstPhase(1:i32 block),
  void setRange(1:i32 startBlock, 2:i32 endBlock)
} 
