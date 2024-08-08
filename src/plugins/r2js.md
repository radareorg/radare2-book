## R2JS Plugins

The javascript runtime embedded in radare2 provides a way to implement different types of plugins.

Check out the `r2skel` project for more examples, but we will cover the basics now.

```js
(function () {
  r2.unload("core", "mycore");
  r2.plugin("core", function () {
    console.log("==> The 'mycore' plugin has been instantiated. Type 'mycore' to test it");
    function coreCall(cmd) {
      if (cmd.startsWith("mycore")) {
        console.log("Hello From My Core!");
        return true;
      }
      return false;
    }
    return {
      "name": "mycore",
      "license": "MIT",
      "desc": "simple core plugin in typescript for radare2",
      "call": coreCall,
    };
  });
})();
```

Some notes on this code:

* The whole code is wrapped inside an anonymous function call, this way we don't polute the global scope.
* Use r2.unload() and r2.plugin() to unload and register new plugins
* Register a plugin by passing the plugin type and a function
* The initialization function returns a object describing it

This code runs inside radare2, this means that it will be _fast_, and by fast I mean faster than Python, r2pipe and will be closer to the `C` plugins. Not just for running, but also for loading, because the js runtime is already there, use r2js plugins if possible if you care about performance.

You can find other plugin examples in the examples directory in radare2.
