#include <stdio.h>
#include "base64.h"


int main(void)
{
  char input[] = "Jhon";
  size_t decode_len = base64_decode_len(input);
   char output[decode_len + 1];
  if(base64_decode(input,output, decode_len+1) == NULL)
  {
    perror("base64");
    return -1;
  }

  printf("decod_len: %zu\n", decode_len);
  printf("input: %s\n", input);
  printf("decode output: %s\n", output);


  return 0;
}
