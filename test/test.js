var Foo = require('./build/Release/foo').Foo
var foo = new Foo()

// subscribing to an event
foo.on('sus', function (a, b) {
  console.log('Received:', a, b);
})

setInterval(function () {
  // this method fires the `sus` event on the native side
  foo.bar()
}, 1000)
