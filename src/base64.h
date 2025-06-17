/*
 *
 */

#ifndef BASE64_H 
#define BASE64_H



/*[base64_encode]-
 *
 *
 */

char *base64_encode(const char *input, char *output, size_t output_len);


/*[base64_decode]-
 *
 *
 */
char *base64_decode(const char *base64_encode_str, char *output, size_t output_len);

/*[base64_encode_len]-
 *
 *
 */
size_t base64_encode_len(const char *input);

/*[base64_decode_len]-
 *
 *
 */
size_t base64_decode_len(const char *input);

#endif
