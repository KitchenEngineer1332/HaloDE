project(
  'HaloDE',
  'c',
  version: '0.1.0',
  license: 'MIT',
  meson_version: '>=0.60.0',
  default_options: [
    'c_std=c17',
    'warning_level=3',
    'werror=true',
  ],
)

cc = meson.get_compiler('c')

# Dependencies
wayland_server = dependency('wayland-server')
wayland_client = dependency('wayland-client')
wayland_protocols = dependency('wayland-protocols', version: '>=1.24')
wlroots = dependency('wlroots', version: '>=0.17.0', fallback: ['wlroots', 'wlroots'])
xkbcommon = dependency('xkbcommon')
libinput = dependency('libinput')
pixman = dependency('pixman-1')
drm = dependency('libdrm')
gbm = dependency('gbm')
egl = dependency('egl')
glesv2 = dependency('glesv2')

# Subdirectories
subdir('protocols')
subdir('libs')
subdir('compositor')
subdir('shell')
subdir('services')
subdir('session')
subdir('settings')
subdir('plugins')
subdir('testing')
subdir('tools')

# Summary
summary({
  'Build Type': get_option('buildtype'),
  'C Standard': get_option('c_std'),
  'Prefix': get_option('prefix'),
}, bool_yn: true)
