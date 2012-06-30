## Aguri Ruby Extension

It's a fixed-size radix trie with an LRU bolted onto the side. When the
tree fills up, the LRU is consulted to find 2 nodes to merge by rolling
up prefixes (/32 to /31, for instance), freeing up a new node.

Nutshell:

	require 'aguritrie'
 	
	tree = Aguri::IPTree.new 1024
	
	1000.times {
	  tree.add( (10<<24) + rand(1000), 1)
	}

        tree.each do |ip, prefix, count, depth|
	  pp [ " " * depth, "#{ ip }/#{ prefix }", count ]
        end

	tree.dump # -> stderr

You can adapt this to things besides IP addresses. I've used it with memory 
address hits, for instance.

