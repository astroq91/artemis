
.PHONY: sandbox run tests run-tests

all: sandbox

sandbox:
	cmake --preset debug
	cmake --build --preset debug 

run: sandbox 
	build/debug/sandbox/sandbox

tests:
	cmake --preset artemis-tests
	cmake --build --preset artemis-tests

run-tests: tests
	build/tests/artemis_tests

clean:
	rm -rf build

