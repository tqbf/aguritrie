# -*- encoding: utf-8 -*-
$:.push File.expand_path("../lib", __FILE__)
require "aguritrie/version"

Gem::Specification.new do |s|
  s.name        = "aguritrie"
  s.version     = Aguri::VERSION
  s.authors     = ["Thomas H. Ptacek"]
  s.email       = ["ThomasPtacek@gmail.com"]
  s.homepage    = ""
  s.summary     = %q{Trivial wrapper around WIDE Project's Aguri library}
  s.description = %q{Trivial wrapper around WIDE Project's Aguri library}

  s.rubyforge_project = "aguritrie"

  s.files         = `git ls-files`.split("\n")
  s.test_files    = `git ls-files -- {test,spec,features}/*`.split("\n")
  s.executables   = `git ls-files -- bin/*`.split("\n").map{ |f| File.basename(f) }
  s.require_paths = ["lib"]

  # specify any dependencies here; for example:
  # s.add_development_dependency "rspec"
  # s.add_runtime_dependency "rest-client"
  s.extensions = ["ext/aguritrie/extconf.rb"]
end
