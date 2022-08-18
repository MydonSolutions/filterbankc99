project('filterbankc99', 'c',
  version: '1.0.1',
  default_options: [
    'c_std=c99',
    'buildtype=release',
    'b_ndebug=if-release'
  ]
)

add_project_arguments('-D_DEFAULT_SOURCE', language: 'c')

cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required : true)

src_lst = []
inc_lst = [
	include_directories('include'),
]
dep_lst = [
  m_dep
]

subdir('include')
subdir('src')

lib_filterbankc99 = library(
  'filterbankc99',
  src_lst,
  include_directories: inc_lst,
  dependencies: dep_lst,
  install: true,
)

lib_filterbankc99_dep = declare_dependency(
  include_directories: inc_lst,
  dependencies: dep_lst,
  link_with: lib_filterbankc99,
)

pkg = import('pkgconfig')
pkg.generate(
  lib_filterbankc99,
  libraries: dep_lst,
)

subdir('tests')