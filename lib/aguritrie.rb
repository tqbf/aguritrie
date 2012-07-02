require "aguritrie/version"
require 'aguritrie/aguritrie'

module Aguri
  class IPTree < Tree
    def initialize(sz)
      super(sz, 32)
    end

    def add(ip, count=1, opts={})
      if ip.kind_of? String
        ip = ip.split(".").map(&:to_i).reduce(0) {|acc, i| acc <<= 8; acc |= i}
      end

      if opts[:prefix]
        super(ip, count, opts[:prefix])
      else
        super(ip, count)
      end
    end

    def each 
      super do |ip, prefix, ticks, depth|
        ip = ip.unpack("N").first
        yield "#{ (ip>>24&0xff) }.#{ (ip>>16)&0xff }.#{ (ip>>8)&0xff }.#{ ip&0xff }",
   	      prefix, ticks, depth
      end
    end
  end
end
