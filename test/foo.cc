#include "foo.h"

Nan::Persistent<v8::Function> Foo::constructor;

Foo::Foo  () { }
Foo::~Foo () { }

void Foo::Init(v8::Local<v8::Object> exports) {
  Nan::HandleScope scope;

  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Foo").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  Nan::SetPrototypeMethod(tpl, "bar", Bar);
  Nan::SetPrototypeMethod(tpl, "on", Emitter::On); // Emitter's prototype method 

  constructor.Reset(tpl->GetFunction());
  exports->Set(Nan::New("Foo").ToLocalChecked(), tpl->GetFunction());
}

void Foo::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  if (info.IsConstructCall()) {
    Foo *foo = new Foo();
    foo->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    const int argc = 1;
    v8::Local<v8::Value> argv[argc] = { info[0] };
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    info.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

void Foo::Bar(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Foo *foo = ObjectWrap::Unwrap<Foo>(info.Holder());

  foo->Emit(Nan::New("sus").ToLocalChecked()
            , Nan::New("pls").ToLocalChecked(), 5);
}
