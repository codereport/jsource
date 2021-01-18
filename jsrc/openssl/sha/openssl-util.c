
#include <string.h>
#include "openssl.h"

void OPENSSL_cleanse(void *ptr, size_t len) {

  memset(ptr, 0, len);
}

