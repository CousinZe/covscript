var str="\t\"hello\"\n\tworld"
for ch iterate str
  if(ch=='\t')
    system.println("Boom!")
  end
  system.print(ch)
end
system.println()
