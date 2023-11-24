all:
	# Compile the runner
	gcc ./run/run.c -o ./output/ssoup_run.out
	
	# Compile the compiler
	bison ./compile/parser.y -o ./output/parser.tab.c
	flex ./compile/lexer.l -o ./output/lexer.yy.c

	gcc ./output/lexer.yy.c ./output/parser.tab.c -o ./output/ssoup_compile.out
