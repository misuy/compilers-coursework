OUT_NAME = main

build: src/main.c src/vector.c src/ir.c src/ast.c src/ast.h src/lang.tab.c src/lex.yy.c
	gcc -o $(OUT_NAME) src/main.c src/vector.c src/ir.c src/ast.c src/lang.tab.c src/lex.yy.c

bison: src/lang.tab.c

src/lang.tab.c: src/lang.y src/ast.h
	bison -o src/lang.tab.c -v -d src/lang.y

flex: src/lex.yy.c

src/lex.yy.c: src/lang.l src/lang.tab.c src/ast.h
	flex -o src/lex.yy.c src/lang.l

clean:
	rm src/lang.output src/lang.tab.c src/lang.tab.h src/lex.yy.c $(OUT_NAME)