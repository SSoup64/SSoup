all:
	# Compile the runner
	gcc -o ./output/ssoup_run.out ./run/Run.c -D DEBUG
	
	# Compile the compiler
	bison -o ./ast/parser.tab.c ./ast/parser.y -d
	flex -o ./ast/lexer.yy.c ./ast/lexer.l

	gcc -o ./output/ssoup_compile.out ./ast/lexer.yy.c ./ast/parser.tab.c -Wall -D DEBUG

	rm ./ast/lexer.yy.c
	rm ./ast/parser.tab.c
	rm ./ast/parser.tab.h
