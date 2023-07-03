#include <assert.h>
#include <node_api.h>
#include <stdio.h>
#include <stdlib.h>

static napi_value Method(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value result;
 
  status = napi_create_string_utf8(env,"Confusión",NAPI_AUTO_LENGTH,&result);
  assert(status == napi_ok);

  size_t str_size=11;
  size_t str_size_read;

  char* buf = (char*)calloc(str_size, sizeof(char));
  status= napi_get_value_string_utf8(env, result, buf, str_size, &str_size_read);
  assert(status == napi_ok);
  printf("string returned by napi_get_value_string_utf8=%s \n",buf);
  printf("str_size_read=%zu\n",str_size_read);
  return result;
}

#define DECLARE_NAPI_METHOD(name, func)                                        \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

static napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor desc = DECLARE_NAPI_METHOD("hello", Method);
  status = napi_define_properties(env, exports, 1, &desc);
  assert(status == napi_ok);
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
