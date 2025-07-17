require 'mkmf'

LIBDIR = RbConfig::CONFIG['libdir']
INCLUDEDIR = RbConfig::CONFIG['includedir']

HEADER_DIRS = [INCLUDEDIR].freeze
LIB_DIRS = [LIBDIR].freeze

dir_config('trooth', HEADER_DIRS, LIB_DIRS)

target_archs = RbConfig::CONFIG['ARCH_FLAG'].scan(/-arch (\S+)/).flatten
unless target_archs.empty?
  if ENV['ARCHFLAGS']
    env_archs = ENV['ARCHFLAGS'].scan(/-arch\s+(\S+)/).flatten
    target_archs = (env_archs & target_archs)
    if target_archs.empty?
      abort 'Specified ARCHFLAGS and installed Ruby have no common ' \
            'architectures - please modify flags'
    end
  end

  final_archs = []
  warn 'Polling for available architectures...'
  target_archs.each do |arch|
    $ARCH_FLAG = " -arch #{arch}" # rubocop:disable all
    warn "Checking if trooth is built for #{arch}... "
    final_archs << arch if find_library('trooth', 'TR_Environment_alloc')
  end

  if final_archs.empty?
    abort 'trooth is not installed or is compiled for an incompatible ' \
          'architecture'
  end

  $ARCH_FLAG = " -arch #{final_archs.join(' -arch ')}" # rubocop:disable all
end

abort 'libtrooth is missing.  please install libtrooth' unless find_header('trooth/bigint.h')

abort 'libtrooth is missing.  please install libtrooth' unless find_library('trooth', 'TR_Environment_alloc')

create_makefile('trooth/trooth')
