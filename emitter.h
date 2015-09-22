#ifndef EMITTER_H
#define EMITTER_H

#include <nan.h>
#include <map>
#include <vector>
#include <iostream>

using CopyablePersistentType =
  Nan::CopyablePersistentTraits<v8::Function>::CopyablePersistent;


class Emitter : public Nan::ObjectWrap {
 public:
  Emitter  () {
    uv_async_init(uv_default_loop()
      , &async
      , Emitter::AsyncEmit);
  }
  ~Emitter () { }

  struct AsyncData {
  	Emitter *self;
  	std::string event_name;
  };

  template<typename... Args>
  void Emit (v8::Local<v8::Value> eventName, const Args&... args) {
    v8::String::Utf8Value name_(eventName->ToString());
    std::string name = std::string(*name_);

    if (m[name].empty()) return;

    std::vector<v8::Local<v8::Value> > converted_args = { args..., };

    for (std::vector<CopyablePersistentType>::iterator
      it = m[name].begin(); it != m[name].end(); ++it) {
      Nan::MakeCallback(Nan::GetCurrentContext()->Global()
        , Nan::New(*it)
        , converted_args.size(), &converted_args.front());
    }
  }

  void Emit2 (std::string eventName) {
    AsyncData *data = new AsyncData();
		data->self = this;
    data->event_name = eventName;

		async.data = (void*)data;
		uv_async_send(&async);
  }

  static void On (const Nan::FunctionCallbackInfo<v8::Value>& info) {
    if (!info[0]->IsString() || !info[1]->IsFunction()) return;

    Emitter *emitter = ObjectWrap::Unwrap<Emitter>(info.Holder());

    v8::String::Utf8Value name(info[0]->ToString());
    v8::Local<v8::Function> cb = info[1].As<v8::Function>();
    std::string eventName = std::string(*name);

    Nan::Persistent<v8::Function> persistent_cb;
    persistent_cb.Reset(cb);

    emitter->m[eventName].push_back(persistent_cb);
  }

  static void AsyncEmit(uv_async_t *handle) {
    Nan::HandleScope scope;

    AsyncData data = *((AsyncData*) handle->data);

    data.self->Emit(Nan::New(data.event_name).ToLocalChecked(), Nan::New(5));
  }

 private:
  std::map<std::string, std::vector<CopyablePersistentType> > m;
  //std::map<std::string, uv_async_t> asyncObjects;

  uv_async_t async;
};

#endif
