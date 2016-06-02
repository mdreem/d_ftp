all: server

clean: clean_server

.PHONY: tests_server

tests_server:
	$(MAKE) -C server tests

clean_server:
	$(MAKE) -C server clean
