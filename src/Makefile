all:
	# Compile the runner
	gcc -o ./output/ssoup_run.out ./run/run.c 
	
	# Compile the compiler
	bison -o ./compile/parser.tab.c ./compile/parser.y -d
	flex -o ./compile/lexer.yy.c ./compile/lexer.l

	gcc -o ./output/ssoup_compile.out ./compile/lexer.yy.c ./compile/parser.tab.c

	rm ./compile/lexer.yy.c
	rm ./compile/parser.tab.c
	rm ./compile/parser.tab.h
