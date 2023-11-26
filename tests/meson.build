test('filterbank', executable(
  'filterbank', ['filterbank.c'],
	  dependencies: lib_filterbankc99_dep,
    c_args: '-fno-stack-protector'
  ),
  args: [],
  timeout: false,
  priority : -1
)

test('filterbank_h5', executable(
  'filterbank_h5', ['filterbank_h5.c'],
	  dependencies: lib_filterbankc99_dep,
    c_args: '-fno-stack-protector'
  ),
  args: [],
  timeout: false,
  priority : -1
)
