.PHONY: all check cmdline

all:
	$(MAKE) -C .. $(MAKECMDGOALS)

check:
	$(MAKE) -C .. $(MAKECMDGOALS)

cmdline:
	$(MAKE) -C .. $(MAKECMDGOALS)

hash_benchmark:
	$(MAKE) -C .. $(MAKECMDGOALS)
