SUBDIRS := contrib lib uplink/src

all: build dist-full

clean:
	rm -rf install; \
	for dir in $(SUBDIRS); do \
			$(MAKE) -C $$dir clean; \
    done

build:
	for dir in $(SUBDIRS); do \
			$(MAKE) -C $$dir; \
    done

dist-full:
	$(MAKE) -C uplink/src dist-full