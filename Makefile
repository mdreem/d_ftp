all: server

clean: clean_server

.PHONY: server

server:
	$(MAKE) -C server

clean_server:
	$(MAKE) -C server clean
