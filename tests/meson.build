test('filterbank', executable(
  'filterbank', ['filterbank.c'],
	  dependencies: lib_filterbankc99_dep,
    c_args: '-fno-stack-protector'
  ),
  args: [],
  timeout: false,
  priority : 2
)

test('filterbank_h5', executable(
  'filterbank_h5', ['filterbank_h5.c'],
	  dependencies: lib_filterbankc99_dep,
    c_args: '-fno-stack-protector'
  ),
  args: [],
  timeout: false,
  is_parallel: false,
  priority : 1
)

test('filterbank_h5_read', executable(
  'filterbank_h5_read', ['filterbank_h5_read.c'],
	  dependencies: lib_filterbankc99_dep,
    c_args: '-fno-stack-protector'
  ),
  args: [],
  is_parallel: false,
  timeout: false,
  priority : 0
)
