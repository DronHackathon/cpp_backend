PROJNAME = cpp_backend

ifndef (PREFIX)
	PREFIX = /usr
endif

libforce:
	@echo Installing libforce with PREFIX=$(PREFIX)
	cp $(OUT_SO) $(PREFIX)/lib/
	cp $(API_INC) $(PREFIX)/include/
