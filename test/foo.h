#ifndef FOO_H
#define FOO_H

#include <nan.h>
#include <emitter.h>

class Foo : public Emitter {
public:
  static void Init(v8::Local<v8::Object> exports);

private:
  explicit Foo ();
  ~Foo ();

  static void New (const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void Bar (const Nan::FunctionCallbackInfo<v8::Value>& info);

  static Nan::Persistent<v8::Function> constructor;
};

#endif
