#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include "base64.h"


static char charAt(size_t index)
{
  char digits[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  return digits[index];
}

char *base64_encode(const char *input, char *output, size_t output_len)
{
  size_t encode_len;
  char *temp = NULL;
  char buf[3] = {0};
  size_t count = 0;
  size_t current_index = 0;

  if(strlen(input) == 0)
  {
    errno = EINVAL;
    return NULL;
  }

  encode_len = base64_encode_len(input);
  if(output && output_len <= encode_len)
  {
    errno = ERANGE;
    return NULL;
  }
  
  if((temp = calloc(encode_len+1, sizeof(char))) == NULL)
    return NULL;

  for(size_t n = strlen(input), i = 0; i < n; ++i)
  {
    buf[count++] = input[i];
    if(count == 3)
    {
      temp[current_index++] = charAt(buf[0]>>2);
      temp[current_index++] = charAt(((buf[0] & 03) << 4) | (buf[1] >> 4));
      temp[current_index++] = charAt(((buf[1] & 017) << 2) | (buf[2] >> 6));
      temp[current_index++] = charAt(buf[2] & 077);

      count = 0;
    }
  }

    if(count == 1)
    {
      temp[current_index++] = charAt(buf[0] >> 2);
      temp[current_index++] = charAt(((buf[0] & 03) << 4));
      temp[current_index++] = '=';
      temp[current_index++] = '=';
    }

    if(count == 2)
    {
      temp[current_index++] = charAt(buf[0] >> 2);
      temp[current_index++] = charAt(((buf[0] & 03) << 4) | (buf[1] >> 4));
      temp[current_index++] = charAt((buf[1] & 017) << 2);
      temp[current_index++] = '=';
    }

    if(output)
    {
      memmove(output,temp,current_index);
      output[current_index] = '\0';
      free(temp);
      return output;
    }
    
    temp[current_index] = '\0';
    return temp;
}

size_t base64_encode_len(const char *input)
{
  return (size_t) ceil(strlen(input)/3.0) * 4;
}
