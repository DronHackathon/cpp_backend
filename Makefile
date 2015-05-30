PROJNAME = cpp_backend

DEPS = force_api.h force_api.c
QUAT = quat_test.c quaternion.c
RECOGNIZER = recognizer.c jedi_recognizer.h
EXEC = setup.py

VIRT_ENV_NAME = libjedi
VIRT_ENV_EXISTS = $(ls | grep libjedi)

ifndef $(PREFIX)
	PREFIX = /usr/
endif

ifndef $(LIBFORCE_SO)
	LIBFORCE_SO = libforce/lib/libforce.so
endif

ifndef $(LIBFORCE_H)
	LIBFORCE_H = libforce/include/force.h
endif

all: $(SRC) $(EXEC) $(QUAT) $(LIBFORCE_H) $(LIBFORCE_SO)
	python $(EXEC) build

install_libforce: $(LIBFORCE_H) $(LIBFORCE_SO)
	@echo Installing libforce with PREFIX=$(PREFIX)
	cp $(LIBFORCE_SO) $(PREFIX)/lib/
	cp $(LIBFORCE_H) $(PREFIX)/include/
	ldconfig

# virtualenv:

install: all
	sudo python $(EXEC) install

test_quat: $(QUAT)
	$(CC) $(QUAT) -o$@ -lm --debug
	./$@

test_recognition: $(RECOGNIZER) $(QUAT)
	$
