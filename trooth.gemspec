lib = File.expand_path('lib', __dir__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'trooth/version'

Gem::Specification.new do |spec|
  spec.name          = 'trooth'
  spec.version       = Trooth::VERSION
  spec.authors       = ['Kayla McArthur']
  spec.email         = ['kayla@kayla.is']
  spec.summary       = 'A general mathematics library'
  spec.homepage      = 'http://github.com/klmcarthur/trooth-ruby'
  spec.license       = 'MIT'

  spec.files         = `git ls-files -z`.split("\x0")
  spec.require_paths = ['lib']
  spec.required_ruby_version = '>= 2.0.0'

  spec.add_development_dependency 'bundler', '>= 1.6'
  spec.add_development_dependency 'faker'
  spec.add_development_dependency 'rake'
  spec.add_development_dependency 'rake-compiler'
  spec.add_development_dependency 'rspec'
  spec.add_development_dependency 'rubocop', '>= 0.49.0'

  spec.extensions = ['ext/trooth/extconf.rb']
  spec.metadata['rubygems_mfa_required'] = 'true'
end
