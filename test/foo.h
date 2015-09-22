#ifndef FOO_H
#define FOO_H

#include <nan.h>
#include <emitter.h>

class Foo : public Emitter {
public:
  static void Init(v8::Local<v8::Object> exports);

  static NAN_METHOD(New);
  static NAN_METHOD(Bar);

private:
  explicit Foo ();
  ~Foo ();

  static Nan::Persistent<v8::Function> constructor;
};

#endif
