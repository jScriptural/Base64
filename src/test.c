#include <stdio.h>
#include "base64.h"


int main(void)
{
  char *input = "Jscriptural";

  size_t encode_len = base64_encode_len(input);
  char output[encode_len+1];

  base64_encode(input,output,encode_len+1);

  printf("encode_len: %zu\n", encode_len);
  printf("input: %s\n", input);
  printf("output: %s\n", output);

  return 0;
}
