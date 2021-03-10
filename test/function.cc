#include "napi.h"

using namespace Napi;

namespace {

int testData = 1;

Boolean EmptyConstructor(const CallbackInfo& info) {
  auto env = info.Env();
  Function function = Function();
  return Boolean::New(env, function.IsEmpty());
}

Boolean NonEmptyConstructor(const CallbackInfo& info) {
  auto env = info.Env();
  Function function = Function(env, Object::New(env));
  return Boolean::New(env, !function.IsEmpty());
}

void VoidCallback(const CallbackInfo& info) {
  auto env = info.Env();
  Object obj = info[0].As<Object>();

  obj["foo"] = String::New(env, "bar");
}

Value ValueCallback(const CallbackInfo& info) {
  auto env = info.Env();
  Object obj = Object::New(env);

  obj["foo"] = String::New(env, "bar");

  return obj;
}

void VoidCallbackWithData(const CallbackInfo& info) {
  auto env = info.Env();
  Object obj = info[0].As<Object>();

  obj["foo"] = String::New(env, "bar");

  int* data = static_cast<int*>(info.Data());
  obj["data"] = Number::New(env, *data);
}

Value ValueCallbackWithData(const CallbackInfo& info) {
  auto env = info.Env();
  Object obj = Object::New(env);

  obj["foo"] = String::New(env, "bar");

  int* data = static_cast<int*>(info.Data());
  obj["data"] = Number::New(env, *data);

  return obj;
}

Value CallWithArgs(const CallbackInfo& info) {
  Function func = info[0].As<Function>();
  return func.Call(
      std::initializer_list<napi_value>{info[1], info[2], info[3]});
}

Value CallWithVector(const CallbackInfo& info) {
   Function func = info[0].As<Function>();
   std::vector<napi_value> args;
   args.reserve(3);
   args.push_back(info[1]);
   args.push_back(info[2]);
   args.push_back(info[3]);
   return func.Call(args);
}

Value CallWithCStyleArray(const CallbackInfo& info) {
  Function func = info[0].As<Function>();
  std::vector<napi_value> args;
  args.reserve(3);
  args.push_back(info[1]);
  args.push_back(info[2]);
  args.push_back(info[3]);
  return func.Call(args.size(), args.data());
}

Value CallWithReceiverAndCStyleArray(const CallbackInfo& info) {
  Function func = info[0].As<Function>();
  Value receiver = info[1];
  std::vector<napi_value> args;
  args.reserve(3);
  args.push_back(info[2]);
  args.push_back(info[3]);
  args.push_back(info[4]);
  return func.Call(receiver, args.size(), args.data());
}

Value CallWithReceiverAndArgs(const CallbackInfo& info) {
   Function func = info[0].As<Function>();
   Value receiver = info[1];
   return func.Call(receiver, std::initializer_list<napi_value>{ info[2], info[3], info[4] });
}

Value CallWithReceiverAndVector(const CallbackInfo& info) {
   Function func = info[0].As<Function>();
   Value receiver = info[1];
   std::vector<napi_value> args;
   args.reserve(3);
   args.push_back(info[2]);
   args.push_back(info[3]);
   args.push_back(info[4]);
   return func.Call(receiver, args);
}

Value CallWithInvalidReceiver(const CallbackInfo& info) {
   Function func = info[0].As<Function>();
   return func.Call(Value(), std::initializer_list<napi_value>{});
}

Value CallConstructorWithArgs(const CallbackInfo& info) {
   Function func = info[0].As<Function>();
   return func.New(std::initializer_list<napi_value>{ info[1], info[2], info[3] });
}

Value CallConstructorWithVector(const CallbackInfo& info) {
   Function func = info[0].As<Function>();
   std::vector<napi_value> args;
   args.reserve(3);
   args.push_back(info[1]);
   args.push_back(info[2]);
   args.push_back(info[3]);
   return func.New(args);
}

Value CallConstructorWithCStyleArray(const CallbackInfo& info) {
  Function func = info[0].As<Function>();
  std::vector<napi_value> args;
  args.reserve(3);
  args.push_back(info[1]);
  args.push_back(info[2]);
  args.push_back(info[3]);
  return func.New(args.size(), args.data());
}

void IsConstructCall(const CallbackInfo& info) {
   Function callback = info[0].As<Function>();
   bool isConstructCall = info.IsConstructCall();
   callback({Napi::Boolean::New(info.Env(), isConstructCall)});
}

Value CallWithFunctionOperator(const CallbackInfo& info) {
  Function func = info[0].As<Function>();
  return func({info[1], info[2], info[3]});
}

} // end anonymous namespace

Object InitFunction(Env env) {
  Object result = Object::New(env);
  Object exports = Object::New(env);
  exports["emptyConstructor"] = Function::New(env, EmptyConstructor);
  exports["nonEmptyConstructor"] = Function::New(env, NonEmptyConstructor);
  exports["voidCallback"] = Function::New(env, VoidCallback, "voidCallback");
  exports["valueCallback"] = Function::New(env, ValueCallback, std::string("valueCallback"));
  exports["voidCallbackWithData"] =
    Function::New(env, VoidCallbackWithData, nullptr, &testData);
  exports["valueCallbackWithData"] =
    Function::New(env, ValueCallbackWithData, nullptr, &testData);
  exports["callWithArgs"] = Function::New(env, CallWithArgs);
  exports["callWithVector"] = Function::New(env, CallWithVector);
  exports["callWithCStyleArray"] = Function::New(env, CallWithCStyleArray);
  exports["callWithReceiverAndCStyleArray"] =
      Function::New(env, CallWithReceiverAndCStyleArray);
  exports["callWithReceiverAndArgs"] = Function::New(env, CallWithReceiverAndArgs);
  exports["callWithReceiverAndVector"] = Function::New(env, CallWithReceiverAndVector);
  exports["callWithInvalidReceiver"] = Function::New(env, CallWithInvalidReceiver);
  exports["callConstructorWithArgs"] = Function::New(env, CallConstructorWithArgs);
  exports["callConstructorWithVector"] = Function::New(env, CallConstructorWithVector);
  exports["callConstructorWithCStyleArray"] =
      Function::New(env, CallConstructorWithCStyleArray);
  exports["isConstructCall"] = Function::New(env, IsConstructCall);
  exports["callWithFunctionOperator"] =
      Function::New(env, CallWithFunctionOperator);
  result["plain"] = exports;

  exports = Object::New(env);
  exports["emptyConstructor"] = Function::New(env, EmptyConstructor);
  exports["nonEmptyConstructor"] = Function::New(env, NonEmptyConstructor);
  exports["voidCallback"] = Function::New<VoidCallback>(env, "voidCallback");
  exports["valueCallback"] =
      Function::New<ValueCallback>(env, std::string("valueCallback"));
  exports["voidCallbackWithData"] =
      Function::New<VoidCallbackWithData>(env, nullptr, &testData);
  exports["valueCallbackWithData"] =
      Function::New<ValueCallbackWithData>(env, nullptr, &testData);
  exports["callWithArgs"] = Function::New<CallWithArgs>(env);
  exports["callWithVector"] = Function::New<CallWithVector>(env);
  exports["callWithCStyleArray"] = Function::New<CallWithCStyleArray>(env);
  exports["callWithReceiverAndCStyleArray"] =
      Function::New<CallWithReceiverAndCStyleArray>(env);
  exports["callWithReceiverAndArgs"] =
      Function::New<CallWithReceiverAndArgs>(env);
  exports["callWithReceiverAndVector"] =
      Function::New<CallWithReceiverAndVector>(env);
  exports["callWithInvalidReceiver"] =
      Function::New<CallWithInvalidReceiver>(env);
  exports["callConstructorWithArgs"] =
      Function::New<CallConstructorWithArgs>(env);
  exports["callConstructorWithVector"] =
      Function::New<CallConstructorWithVector>(env);
  exports["callConstructorWithCStyleArray"] =
      Function::New<CallConstructorWithCStyleArray>(env);
  exports["isConstructCall"] = Function::New<IsConstructCall>(env);
  exports["callWithFunctionOperator"] =
      Function::New<CallWithFunctionOperator>(env);
  result["templated"] = exports;
  return result;
}
