PROJNAME = cpp_backend

ifndef $(PREFIX)
	PREFIX = /usr
endif

ifndef $(LIBFORCE_SO)
	LIBFORCE_SO = libforce/lib/libforce.so
endif

ifndef $(LIBFORCE_H)
	LIBFORCE_H = libforce/include/force.h
endif

install_libforce: $(LIBFORCE_H) $(LIBFORCE_SO)
	@echo Installing libforce with PREFIX=$(PREFIX)
	cp $(LIBFORCE_SO) $(PREFIX)/lib/
	cp $(LIBFORCE_H) $(PREFIX)/include/
	ldconfig
