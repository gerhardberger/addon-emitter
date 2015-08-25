# addon-emitter
event emitter class for native addons.

## usage
using a native addon (here `foo`) with event emitter:
``` js
var Foo = require('foo')
var foo = new Foo()

foo.on('sus', function (a, b) {
  console.log('Received:', a, b);
})
```
the `Foo` class inherits the public interface of the `Emitter` class:
``` cpp
// foo.h

#include <nan.h>
#include <emitter.h>

// inherit from the Emitter class
class Foo : public Emitter {
public:
  static void Init(v8::Local<v8::Object> exports);

private:
  explicit Foo ();
  ~Foo ();

  void Bar ();
};
```
set the method for the event listening (usually the `on` method):
``` cpp
// foo.cc

void Foo::Init(v8::Local<v8::Object> exports) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);

  // setting the prototype method for the event listening
  Nan::SetPrototypeMethod(tpl, "on", Emitter::On);
}
```
to emit an event from `Foo`:
``` cpp
void Foo::Bar(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  // the first parameter is the name of the event,
  // after that you can write as many parameters as needed,
  // but they have to convertible to v8's `Local<Value>` type
  Emit(Nan::New("sus").ToLocalChecked()
      , Nan::New("a string").ToLocalChecked(), 123);
}
```
the example above is just to show what you have to do to use the module. it's not a fully functioning program. for a working example check the `test` folder.

## install
``` bash
npm i addon-emitter
```
in `binding.gyp`:
``` gyp
{
  'include_dirs': [ "<!(node -e \"require('addon-emitter')\")" ]
}
```
