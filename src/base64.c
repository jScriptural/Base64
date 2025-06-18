#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <ctype.h>
#include <stdbool.h>
#include "base64.h"


static char char_at(size_t index)
{
  char digits[] = {'A','B','C','D','E','F','G',
       'H','I','J','K','L','M','N','O','P','Q',
       'R','S','T','U','V','W','X','Y','Z','a',
       'b','c','d','e','f','g','h','i','j','k',
       'l','m','n','o','p','q','r','s','t','u',
       'v','w','x','y','z','0','1','2','3','4',
       '5','6','7','8','9','+','/'};

  return digits[index];

}

static int  char_index(char c)
{
  switch(c)
  {
    case '+': return 62;
    case '/': return 63;
    case '=': return 64;
  }

  if(isupper(c))
    return (c % 65);

  if(islower(c))
    return (c % 71);

  if(isdigit(c))
    return (c + 4);

  return -1;
}


static bool is_valid_base64_encoded_string(const char *input)
{
  size_t len = strlen(input);

  if((NULL == input) || (0 == len) || ((len % 4) != 0))
    return false;

  for(size_t j=0,i = 0; i < len; ++i)
  {
    if(input[i] == '=')
      ++j;

    if(j > 2)
      return false;
  }

  for(size_t i = 0; i < len; i++)
  {
    if(input[i] == '=')
    {
      if(i == len-2)
	if(input[len-1] != '=')
	  return false;

      if(i != len-1 && i != len-2)
	return  false;
    }
  }

  for(size_t i = 0; i < len; ++i)
    if(-1 == char_index(input[i]))
      return false;

  return true;
}

char *base64_encode(const char *input, char *output, size_t output_len)
{
  size_t current_index = 0;
  size_t encode_len;
  char *temp = NULL;
  char buf[3] = {0};
  short count = 0;

  if(input == NULL || strlen(input) == 0)
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
      temp[current_index] = char_at(buf[0]>>2);
      temp[current_index+1] = char_at(((buf[0] & 03) << 4) | (buf[1] >> 4));
      temp[current_index+2] = char_at(((buf[1] & 017) << 2) | (buf[2] >> 6));
      temp[current_index+3] = char_at(buf[2] & 077);

      current_index += 4;
      count = 0;
    }
  }

    if(count == 1)
    {
      temp[current_index] = char_at(buf[0] >> 2);
      temp[current_index+1] = char_at(((buf[0] & 03) << 4));
      temp[current_index+2] = '=';
      temp[current_index+3] = '=';

      current_index += 4;
    }

    if(count == 2)
    {
      temp[current_index] = char_at(buf[0] >> 2);
      temp[current_index+1] = char_at(((buf[0] & 03) << 4) | (buf[1] >> 4));
      temp[current_index+2] = char_at((buf[1] & 017) << 2);
      temp[current_index+3] = '=';

      current_index += 4;
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








char *base64_decode(const char *input, char *output, size_t output_len)
{
  size_t current_index = 0;
  size_t decode_len;
  char buf[4] = {0};
  char *temp = NULL;
  short count = 0; 

  if(!is_valid_base64_encoded_string(input))
  {
    errno = EINVAL;
    return NULL;
  }


  decode_len = base64_decode_len(input);
  if(output && output_len <= decode_len)
  {
    errno = ERANGE;
    return NULL;
  }

  if((temp = calloc(decode_len+1, sizeof(char))) == NULL)
    return NULL;

  for(size_t n = strlen(input), i = 0; i < n; ++i)
  {
    buf[count++] = char_index(input[i]);
    if(count == 4)
    {
      temp[current_index++] = ((buf[0] << 2) | (buf[1] >> 4));
      
      if(buf[2] != 64)
      {
	temp[current_index++] = (((buf[1] & 017) << 4) | (buf[2] >> 2));
      }
      else
      {
	temp[current_index++] = ((buf[1] & 017) << 4);

	break;
      }
      
      if(buf[3] != 64)
      {
	temp[current_index++] = (((buf[2] & 03) << 6) | buf[3]);
      }

      count = 0;
    }
  }


  if(output)
  {
    memmove(output,temp,decode_len);
    output[decode_len] = '\0';
    free(temp);
    return output;
  }
 
  temp[current_index] = '\0';
  return temp;
}




size_t base64_encode_len(const char *input)
{
  return ceil(strlen(input)/3.0) * 4;
}



size_t base64_decode_len(const char *input)
{
  size_t decode_len, len;

  len = strlen(input);
  decode_len = floor(len/4.0) * 3;

  for(size_t i = len-1; i > 0; --i)
    if(input[i] == '=')
      --decode_len;
    else
      break;

  return decode_len;
}
