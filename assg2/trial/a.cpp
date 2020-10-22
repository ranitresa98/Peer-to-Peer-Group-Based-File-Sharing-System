#include <openssl/sha.h>
//If you have all of your data at once, just use the SHA1 function:
char data[] = "Hello, world!";
size_t length = strlen(data);

unsigned char hash[SHA_DIGEST_LENGTH];
SHA1(data, length, hash);




//If, on the other hand, you only get your data one piece at a time and you want to compute the hash as you receive that data, then use the other functions:
// Error checking omitted for expository purposes

// Object to hold the current state of the hash
SHA_CTX ctx;
SHA1_Init(&ctx);

// Hash each piece of data as it comes in:
SHA1_Update(&ctx, "Hello, ", 7);
...
SHA1_Update(&ctx, "world!", 6);
// etc.
...
// When you're done with the data, finalize it:
unsigned char hash[SHA_DIGEST_LENGTH];
SHA1_Final(hash, &ctx);




