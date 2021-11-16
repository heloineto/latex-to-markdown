tex-to-md: tex-to-md.l tex-to-md.y tex-to-md.h
				bison -d tex-to-md.y
				flex -o tex-to-md.lex.c tex-to-md.l
				gcc -o $@ tex-to-md.tab.c tex-to-md.lex.c tex-to-md.c -lm -g
				@echo Parser da Linguagem estah pronto!