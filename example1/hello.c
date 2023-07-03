#include <assert.h>
#include <node_api.h>
#include <stdio.h>
#include <stdlib.h>

char* get_string(napi_env env,napi_value v)
{
  char * buf;
  size_t str_size;
  size_t str_size_read;
  napi_status status;

  napi_get_value_string_utf8(env, v, NULL, 0, &str_size);
  // This is different under bun or under node
  buf = (char*)calloc(str_size + 1, sizeof(char));
  str_size = str_size + 1;
  status= napi_get_value_string_utf8(env, v, buf, str_size, &str_size_read);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Invalid string was passed as argument");
  }
  printf("string read by napi_get_value_string_utf8 =%s\n", buf);
  printf("str_size_read=%zu\n",str_size_read);
 
  return buf;
}

static napi_value Method(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value result;
  size_t argc = 1;
  napi_value argv[1];

  status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Failed to parse arguments");
  }
  char* result_string =get_string(env,argv[0]);

  status = napi_create_string_utf8(env,result_string,NAPI_AUTO_LENGTH,&result);
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
