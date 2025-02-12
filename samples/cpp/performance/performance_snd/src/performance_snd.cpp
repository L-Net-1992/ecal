/* ========================= eCAL LICENSE =================================
 *
 * Copyright (C) 2016 - 2019 Continental Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * ========================= eCAL LICENSE =================================
*/

#include <ecal/ecal.h>
#include <ecal/ecal_publisher.h>

#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>

// main entry
int main(int argc, char **argv)
{
  size_t payload_size(1024*1024);
  if(argc > 1) payload_size = atoi(argv[1]);
  if(payload_size < 1) payload_size = 1;

  // initialize eCAL API
  eCAL::Initialize(argc, argv, "performance_snd");

  // new publisher
  eCAL::CPublisher pub("Performance");

  int                 clock(0);
  int                 msgs (0);
  unsigned long long  bytes(0);
  size_t              slen (0);
  int                 trigger(1);
  bool                zero_copy(true);
  int                 buffer_count(1);
  bool                handshake(true);

  // default send string
  std::string send_s = "Hello World ";
  while(send_s.size() < payload_size)
  {
    send_s += send_s;
  }
  send_s.resize(payload_size);

  std::cout << "Message size  =  " << int(send_s.size()) << " Byte = " << int(send_s.size()/1024) << " kByte = " << int(send_s.size()/1024/1024) << " MByte" << std::endl << std::endl;
  slen = send_s.size();

  // set zero copy
  if (zero_copy)
  {
    std::cout << "Switch zero copy mode on" << std::endl << std::endl;
  }
  pub.ShmEnableZeroCopy(zero_copy);

  // set write buffer count
  if (buffer_count > 1)
  {
    std::cout << "Set number of write buffer to " << buffer_count << std::endl << std::endl;
  }
  pub.ShmSetBufferCount(buffer_count);
  
  // set handshake
  if (handshake)
  {
    std::cout << "Switch handshake on" << std::endl << std::endl;
    pub.ShmSetAcknowledgeTimeout(std::chrono::milliseconds(1000));
  }
  else
  {
    pub.ShmSetAcknowledgeTimeout(0);
  }

  // safe the start time
  auto start_time = std::chrono::steady_clock::now();

  // send updates
  while(eCAL::Ok())
  {
    eCAL::Logging::StartCoreTimer();

    // send content
    size_t snd_len = pub.Send(send_s);
    if((snd_len > 0) && (snd_len != slen))
    {
      std::cerr <<  std::endl << "Send failed !" << " sent : " << slen << " returned : " << snd_len <<  std::endl;
    }

    // collect data
    clock++;
    msgs++;
    bytes += slen;

    // check timer and print results every second
    if(clock%10011 == 0)
    {
      std::chrono::duration<double> diff_time = std::chrono::steady_clock::now() - start_time;
      if(diff_time >= std::chrono::seconds(1))
      {
        trigger++;

        start_time = std::chrono::steady_clock::now();
        std::stringstream out;
        out << "Message size (kByte):  " << (unsigned int)(slen  / 1024)                            << std::endl;
        out << "kByte/s:               " << (unsigned int)(bytes / 1024 /        diff_time.count()) << std::endl;
        out << "MByte/s:               " << (unsigned int)(bytes / 1024 / 1024 / diff_time.count()) << std::endl;
        out << "Messages/s:            " << (unsigned int)(msgs  /               diff_time.count()) << std::endl;
        out << "Latency (us):          " << (diff_time.count() / msgs) * 1000 * 1000                << std::endl;
        std::cout << out.str() << std::endl;
        msgs  = 0;
        bytes = 0;

        eCAL::Logging::Log(out.str());
      }
    }

    eCAL::Logging::StopCoreTimer();
  }

  // destroy publisher
  pub.Destroy();

  // finalize eCAL API
  eCAL::Finalize();

  return(0);
}
