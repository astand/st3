#pragma once


#include <stdint.h>

class ITrekList {

 public:
  /// @RefreshTrekList - refresh live list of availible treks
  /// @return count of availible treks in new list
  virtual int32_t RefreshTrekList() = 0;

  /// @UploadList - loads to buf descriptions of treks from startindex
  /// to @startindex + @indexcount
  /// @return size of loaded data (in bytes)
  virtual int32_t UploadList(uint16_t startindex, uint16_t indexcount, uint8_t* buf) = 0;


  /// UploadTrek - load to buf peace of trek
  /// @req_id - requested file index in list
  /// @tn_offset - number in track_notes from what uploading is start
  /// @maxsize -  max packet size (max bytes value that can be uploaded to buff)
  /// @buf - buffer for raw data
  ///
  /// @return
  virtual int32_t UploadTrek(uint16_t req_id, int32_t tn_offset,
                             uint32_t maxsize, uint8_t* buf) = 0;


  /// GetTrekSize - return size of dedicated trek
  /// @req_id - index of requested trek in list
  virtual int32_t GetTrekSize(uint16_t req_id) = 0;

  /// GetListIndex - return index in list of requested trek
  /// @store_id - ID that was assigned to file when saving
  virtual int32_t GetListIndex(uint16_t store_id) = 0;
};
