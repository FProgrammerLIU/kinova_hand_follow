#ifndef KINOVA_HAND_FOLLOW_H_
#define KINOVA_HAND_FOLLOW_H_

#include "kinova_driver/kinova_comm.h"
namespace iqr {

class HongfuBmsStatus {
public:
  uint8_t cmd_status_ = 0x03, cmd_voltage_ = 0x04, cmd_status_sum_ = 0xFD, cmd_voltage_sum_ = 0xFC;
  std::vector<uint8_t> buffer_all_, buffer_vol_;
  int looprate_;

  HongfuBmsStatus(ros::NodeHandle& nod);
  bool initPort();
  std::vector<uint8_t> dataRead(uint8_t date_type, uint8_t check_sum_write, uint16_t buffer_sum, 
      uint16_t check_sum_read, std::vector<uint8_t> buffer);
  void dataParsing(std::vector<uint8_t>& all, std::vector<uint8_t>& vol);

private:
  hongfu_bms_msg::HongfuStatus hongfu_status_;
  void hongfuCallback();
};
} // namespace iqr
#endif // KINOVA_HAND_FOLLOW_H_