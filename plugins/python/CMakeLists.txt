set(CMAKE_CXX_STANDARD 17)

find_package(Python3
	COMPONENTS Interpreter Development
	OPTIONAL_COMPONENTS NumPy
)

set(_LIBPYTHON_LOCATE_PROG "
import sys, sysconfig; from pathlib import Path
libpl, _lib = sysconfig.get_config_vars('LIBPL', 'LIBRARY')
libpl = Path(libpl)
if sys.platform == 'linux':
	# NOTE:20211222:jgerity:Debian changes the linkage of libpythonX.Ym.a and
	# introduces libpythonX.Ym-pic.a. We want the PIC version if it exists,
	# otherwise whatever sysconfig told us. More information: https://bugs.python.org/issue589429
	lib = next(libpl.glob('*-pic.a'), libpl.joinpath(_lib))
else:
	lib = libpl.joinpath(_lib)
if not lib.exists():
	raise SystemExit('not found')
print(str(lib),end='')
")

execute_process(COMMAND
	${Python3_EXECUTABLE} -c "${_LIBPYTHON_LOCATE_PROG}"
	OUTPUT_VARIABLE _PYTHON3_LIB
)

add_subdirectory(PythonCamera)
