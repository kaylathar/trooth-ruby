require 'mkmf'

LIBDIR = RbConfig::CONFIG['libdir']
INCLUDEDIR = RbConfig::CONFIG['includedir']

HEADER_DIRS = [INCLUDEDIR]
LIB_DIRS = [LIBDIR]

dir_config('trooth', HEADER_DIRS, LIB_DIRS)

target_archs = RbConfig::CONFIG['ARCH_FLAG'].scan(/-arch (\S+)/).flatten

if ENV['ARCHFLAGS']
  env_archs = ENV['ARCHFLAGS'].scan(/-arch\s+(\S+)/).flatten
  target_archs = (env_archs & target_archs)
  if target_archs.empty?
    abort 'Specified ARCHFLAGS and installed Ruby have no common '\
          'architectures - please modify flags'
  end
end

final_archs = []
$stderr.puts 'Polling for available architectures...'
target_archs.each do |arch|
  $ARCH_FLAG = " -arch #{arch}" # rubocop:disable all
  $stderr.puts "Checking if trooth is built for #{arch}... "
  final_archs << arch if find_library('trooth', 'TR_Environment_alloc')
end

if final_archs.empty?
  abort 'trooth is not installed or is compiled for an incompatible '\
        'architecture'
end

$ARCH_FLAG = " -arch #{final_archs.join(' -arch ')}" # rubocop:disable all

unless find_header('trooth/bigint.h')
  abort 'libtrooth is missing.  please install libtrooth'
end

unless find_library('trooth', 'TR_Environment_alloc')
  abort 'libtrooth is missing.  please install libtrooth'
end

create_makefile('trooth/trooth')
