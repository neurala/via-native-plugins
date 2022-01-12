# Neurala SDK Python Plugins

These sample plugins show how the Neurala SDK can make use of Python for providing
Camera data (`PythonCamera/`) or for processing outputs (`PythonOutputAction/`).

<h2 style="color:red;">WARNING: because these sample plugins link `libpython`
statically, running more than one of them simultaneously is dangerous!</h2>


## Notes

* You will need a relocatable `libpython` to build these plugins, i.e. one
  built with `--enable-shared` in CPython's configuration step.
  * This is already taken care of on Debian/Ubuntu systems with the `python3-dev`
    package installed, the build system will be able to locate the library.
* `libpython` is **statically linked** into each plugin, which makes running
  more than one of these plugins dangerous because they could clobber each other's
  symbols.
  * This can be improved, it should be possible to distribute `libpython`
    alongside the plugins so that there is only one set of symbols in use.
  * In the meantime, it is best to run **only one** of these plugins at the same
    time, although these samples were tested side-by-side without any problems
* The Global Interpreter Lock (GIL) and other interpreter state is shared
  across an entire process, so it is essential that plugins correctly acquire
  and release the GIL.
  * The provided `ACQUIRE_GIL, RELEASE_GIL` macros in `PythonPlugin.h` can and
    should be wrapped around any C-API usage. Don't forget to decrement
    object references with `Py_XDECREF(objptr)` when you are done with them, too!