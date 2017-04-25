// Copyright 2017, Kelvin Yin.  All rights reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author:  Kelvin Yin (yinakngxi at gmail dot com)
//
// This is a public header file, it must only include public header files.

#ifndef SIMPLE_CODEC_H
#define SIMPLE_CODEC_H

#include <muduo/base/Logging.h>
#include <muduo/net/Buffer.h>
#include <muduo/net/Endian.h>
#include <muduo/net/TcpConnection.h>

using namespace std;

class LengthHeaderCodec : muduo::noncopyable
{
 public:
  typedef std::function<void (const muduo::net::TcpConnectionPtr&,
                                const string& message,
                                muduo::Timestamp)> StringMessageCallback;

  explicit LengthHeaderCodec(const StringMessageCallback& cb)
    : messageCallback_(cb)
  {
  }

  void onMessage(const muduo::net::TcpConnectionPtr& conn,
                 muduo::net::Buffer* buf,
                 muduo::Timestamp receiveTime);

  void send(const muduo::net::TcpConnectionPtr& conn,
            const muduo::StringPiece& message);

 private:
  StringMessageCallback messageCallback_;
};

const int32_t kHeaderLength = 4;
const int32_t kPackageMaxLength = 0x100000; // 64M
const int32_t kCheckSumLength = 2;

uint16_t checksum(const uint16_t *buffer, int size);
bool compress_(uint32_t i, char *buf, size_t &length);
bool uncompress_(char *buf, size_t length, uint32_t &i);

#endif  // SIMPLE_CODEC_H
