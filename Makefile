TEST_OBJS=g++m32 g++

.PHONY: test
test: $(TEST_OBJS)

.PHONY: clean_test
clean_test:
	#@cd test && make clean_test

.PHONY: g++m32
g++m32: clean_test
	#@cd test && make CXX_="g++ -m32" test

%: clean_test
	#@cd test && make CXX_="$@" test

#valgrind
#bcov
