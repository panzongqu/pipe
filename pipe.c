/*
 * pipe.c
 *
 *  Created on: Feb 23, 2017
 *      Author: epanda
 */
#include "stdint.h"
#include "string.h"
#include "pipe.h"

#define min(a,b) ((a) < (b) ? (a) : (b))

static uint32_t pipe_space_remain(pipe *dev);

uint32_t pipe_init(pipe *dev, uint8_t *buf, uint32_t buf_size)
{
  dev->buf = buf;
  dev->buf_size = buf_size;
  dev->start = dev->buf;
  dev->end = dev->buf + dev->buf_size;
  dev->wp = dev->buf;
  dev->rp = dev->buf;
  return 0;
}

uint32_t pipe_read(pipe *dev, uint8_t *data, uint32_t len)
{
  uint32_t ret_len;

  /* no data to read */
  if(dev->rp == dev->wp)
  {
    ret_len = 0;
  }
  else if(dev->rp < dev->wp)
  {
    ret_len = min(len, (dev->wp - dev->rp));
    memcpy(data, dev->rp, ret_len);
    dev->rp += ret_len;
  }
  else
  {
    if((dev->rp + len) > dev->end)
    {
      memcpy(data, dev->rp, dev->end - dev->rp);
      memcpy(data + (dev->end - dev->rp), dev->start, min(dev->wp - dev->start, len - (dev->end - dev->rp)));
      dev->rp = dev->start + min(dev->wp - dev->start, len - (dev->end - dev->rp));
      ret_len = dev->end - dev->rp + min(dev->wp - dev->start, len - (dev->end - dev->rp));
    }
    else
    {
      memcpy(data, dev->rp, len);
      dev->rp += len;
      ret_len = len;
    }
  }
  return ret_len;
}

uint32_t pipe_write(pipe *dev, uint8_t *data, uint32_t len)
{
  uint32_t ret_len;

  ret_len = min(len, pipe_space_remain(dev));
  if((dev->wp + ret_len) > dev->end)
  {
    memcpy(dev->wp, data, dev->end - dev->wp);
    memcpy(dev->start, data + (dev->end - dev->wp), ret_len -(dev->end - dev->wp));
    dev->wp = dev->start + (ret_len -(dev->end - dev->wp));
  }
  else
  {
    memcpy(dev->wp, data, ret_len);
    dev->wp += ret_len;
  }
  return ret_len;
}

static uint32_t pipe_space_remain(pipe *dev)
{
  if(dev->wp == dev->rp)
    return dev->buf_size - 1;

  return (dev->rp + dev->buf_size - dev->wp)%dev->buf_size - 1;
}
