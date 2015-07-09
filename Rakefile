require 'rubygems'
require 'bundler'
require 'bundler/gem_tasks'
Bundler.setup(:default, :development)
require 'rspec/core'
require 'rspec/core/rake_task'
require 'rake/extensiontask'

spec = Gem::Specification.load('trooth.gemspec')
RSpec::Core::RakeTask.new(:spec)

Rake::ExtensionTask.new('trooth', spec)

desc 'Validate Package'
task validate: [:rubocop, :compile, :spec]
task default: :validate

desc 'Run Rubocop'
task :rubocop do
  require 'rubocop/rake_task'
  RuboCop::RakeTask.new
end

desc 'Build Documentation'
task :yard do
  require 'yard'
  YARD::Rake::YardocTask.new
end
