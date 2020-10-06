all:
	mkdir -p ui/build
	cd ui && $(MAKE)

.PHONY: clean

clean:
	cd ui && $(MAKE) clean