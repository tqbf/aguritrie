require "bundler/gem_tasks"
require 'rubygems'
require 'rake'
require 'rake/extensiontask'
require 'bundler'

Rake::ExtensionTask.new("aguritrie") do |extension|
  extension.lib_dir = "lib/aguritrie"
end

task :build => [:clean, :compile]

Bundler::GemHelper.install_tasks
