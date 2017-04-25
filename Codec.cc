// Copyright 2017, Kelvin Yin. All rights reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author:  Kelvin Yin (yinakngxi at gmail dot com)

#include "Codec.h"

#include <stdlib.h>
#include <string>
#include <stdint.h>

void LengthHeaderCodec::onMessage(const muduo::net::TcpConnectionPtr& conn,
               muduo::net::Buffer* buf,
               muduo::Timestamp receiveTime)
{
  while (buf->readableBytes() >= kHeaderLength) // kHeaderLength == 4
  {
    const int32_t len = buf->peekInt32();
    if (len > 65536 || len < 0)
    {
      LOG_ERROR << "Invalid length " << len;
      conn->shutdown();  // FIXME: disable reading
      break;
    }
    else if (buf->readableBytes() >= static_cast<size_t>(len))
    {
      string message(buf->peek(), len);
      messageCallback_(conn, message, receiveTime);
      buf->retrieve(len);
    }
    else
    {
      break;
    }
  }
}

void LengthHeaderCodec::send(const muduo::net::TcpConnectionPtr& conn,
          const muduo::StringPiece& message)
{
  muduo::net::Buffer buf;
  buf.append(message.data(), message.size());
  conn->send(&buf);
}

uint16_t checksum(const uint16_t *buffer, int size)
{
    uint16_t cksum = 0;
    while(size > 1)
    {
        cksum += *buffer++;
        size -= sizeof(uint16_t);
    }
    if(size)
    {
        cksum += *(unsigned char*)(buffer);
    }
    //将32位数转换成16
    while (cksum >> 16)
        cksum = (cksum >> 16) + (cksum & 0xffff);
    return static_cast<uint16_t>(~cksum);
}

bool compress_(uint32_t i, char *buf, size_t &length)
{
    length = 0;
    for (int a=4; a>=0; a--)
    {
        char c;
        c = i >> (a*7) & 0x7f;
        if (c == 0x00 && length == 0)
            continue;

        if (a == 0)
            c &= 0x7f;
        else
            c |= 0x80;
        buf[length] = c;
        length++;
    }

    if (length == 0) {
        length++;
        buf[0] = 0;
    }

    return true;
}

bool uncompress_(char *buf, size_t length, uint32_t &i)
{
    i = 0;
    for (int index = 0; index < (int)length; index++)
    {
        char c = *(buf + index);
        i = i << 7;

        c &= 0x7f;
        i |= c;
    }
    return true;
}

