#include <assert.h>
#include <node_api.h>
#include <stdio.h>
#include <stdlib.h>

// La función napi_get_cb_info está definida en el archivo napi.cpp en bun/src/bun.js/bindings
// napivalue es un JSC::JSValue

// Detects if we are running under Bun by checking if the global object has the Bun property
bool running_under_Bun(napi_env env)
{
  bool result;
  napi_value global;
  napi_status status = napi_get_global(env, &global);
  assert (status == napi_ok);
  status = napi_has_named_property(env, global, "Bun", &result);
  assert (status == napi_ok);
  return result;
}


char* get_string(napi_env env,napi_value v)
{
  char * buf;
  size_t str_size;
  size_t str_size_read;
  napi_status status;

  napi_get_value_string_utf8(env, v, NULL, 0, &str_size);
  //printf("str_size=%zu\n",str_size);
  buf = (char*)calloc(str_size + 1, sizeof(char));
  str_size = str_size + 1;
  status= napi_get_value_string_utf8(env, v, buf, str_size, &str_size_read);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Invalid string was passed as argument");
  }
  // This is different under bun or under node
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

  // napi_value global, print;
  // status = napi_get_global(env, &global);
  // assert (status == napi_ok); 
  // assert(global);

  // status = napi_get_named_property(env, global, "print", &print);
  // if (status != napi_ok)
  // {
  //   napi_throw_error(env, NULL, "Error in napi_get_named_property");
  // }
  // assert(print);
  // If the function is not found, it return undefined!


  // napi_value return_val;

  // napi_value print_argv[2];
  // print_argv[0]=result;
  // size_t print_argc = 1;

  // status = napi_call_function(env, global, print, print_argc, print_argv, &return_val);
  // if (status != napi_ok)
  // {
  //   napi_throw_error(env, NULL, "Error in napi_call_function");
  // }
  return result;
}

static napi_value hello2_napi(napi_env env, napi_callback_info info) {
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

  napi_value global, hello_function;
  status = napi_get_global(env, &global);
  assert (status == napi_ok); 
  assert(global);

  status = napi_get_named_property(env, global, "hello", &hello_function);
  if (status != napi_ok)
  {
    napi_throw_error(env, NULL, "Error in napi_get_named_property");
  }
  assert(hello_function);
  // If the function is not found, it return undefined!


  napi_value return_val;

  napi_value print_argv[2];
  print_argv[0]=result;
  size_t print_argc = 1;

  status = napi_call_function(env, global, hello_function, print_argc, print_argv, &return_val);
  if (status != napi_ok)
  {
    napi_throw_error(env, NULL, "Error in napi_call_function");
  }
  return result;
}


#define DECLARE_NAPI_METHOD(name, func)                                        \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

static napi_value Init(napi_env env, napi_value exports) {
  if(running_under_Bun(env))
     printf("Running under Bun\n");
  else
     printf("Not running under Bun\n");  

  napi_status status;
  const napi_property_descriptor  desc[]  ={ DECLARE_NAPI_METHOD("hello", Method),
                                             DECLARE_NAPI_METHOD("hello2", hello2_napi) };
  status = napi_define_properties(env, exports, 2, &desc);
  assert(status == napi_ok);
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
