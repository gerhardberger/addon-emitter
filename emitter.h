#ifndef EMITTER_H
#define EMITTER_H

#include <nan.h>
#include <map>
#include <vector>
#include <iostream>

namespace internal {

using CP = Nan::CopyablePersistentTraits<v8::Function>::CopyablePersistent;

} // namespace internal

class Emitter : public Nan::ObjectWrap {
public:
  Emitter  () {}
  ~Emitter () {}

  template<typename... Args>
  void Emit (v8::Local<v8::Value> info, const Args&... args) {
    v8::String::Utf8Value name(info->ToString());

    if (m[std::string(*name)].empty()) return;

    std::vector<v8::Local<v8::Value> > converted_args = { Nan::New(args)..., };

    for (std::vector<internal::CP>::iterator it = m[std::string(*name)].begin();
         it != m[std::string(*name)].end(); ++it) {
      Nan::MakeCallback(Nan::GetCurrentContext()->Global()
                        , Nan::New(*it)
                        , converted_args.size(), &converted_args.front());
    }
  }

  static void On (const Nan::FunctionCallbackInfo<v8::Value>& info) {
    if (!info[0]->IsString() || !info[1]->IsFunction()) return;

    Emitter *emitter = ObjectWrap::Unwrap<Emitter>(info.Holder());

    v8::String::Utf8Value name(info[0]->ToString());
    v8::Local<v8::Function> cb = info[1].As<v8::Function>();

    Nan::Persistent<v8::Function> persistent_cb;
    persistent_cb.Reset(cb);

    emitter->m[std::string(*name)].push_back(persistent_cb);
  }


private:
  std::map<std::string, std::vector<internal::CP> > m;
};

#endif
