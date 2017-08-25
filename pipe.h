/*
 * pipe.h
 *
 *  Created on: Feb 23, 2017
 *      Author: epanda
 */

#ifndef PIPE_H_
#define PIPE_H_

typedef struct
{
  uint8_t *buf;
  uint32_t buf_size;
  uint8_t *start, *end; /* pipe start and pipe end */
  uint8_t *wp, *rp; /* where to write and where to read */
} pipe;

uint32_t pipe_init(pipe *dev, uint8_t *buf, uint32_t buf_size);
uint32_t pipe_read(pipe *dev, uint8_t *data, uint32_t len);
uint32_t pipe_write(pipe *dev, uint8_t *data, uint32_t len);

#endif /* PIPE_H_ */
