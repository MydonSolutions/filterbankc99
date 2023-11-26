configure_file(
   input : 'filterbankc99/config.hh.in',
   output : 'filterbankc99_config.h',
   configuration : cfg_lst,
   install : true,
   install_dir : get_option('includedir')
)

install_subdir('filterbankc99', install_dir : get_option('includedir'))
install_headers('filterbankc99.h', install_dir : get_option('includedir'))