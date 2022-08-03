run: AssemblerCrossing.o checkText.o Input.o macroList.o convertText.o main.o
	gcc -g -ansi -Wall -pedantic AssemblerCrossing.o checkText.o Input.o macroList.o convertText.o main.o -o run
AssemblerCrossing.o: AssemblerCrossing.h AssemblerCrossing.c
	gcc -c -ansi -Wall -pedantic AssemblerCrossing.c -o AssemblerCrossing.o
checkText.o: checkText.h checkText.c
	gcc -c -ansi -Wall -pedantic checkText.c -o checkText.o
Input.o: Input.h Input.c
	gcc -c -ansi -Wall -pedantic Input.c -o Input.o
convertText.o: convertText.h convertText.c
	gcc -c -ansi -Wall -pedantic convertText.c -o convertText.o
macroList.o: macroList.h macroList.c
	gcc -c -ansi -Wall -pedantic macroList.c -o macroList.o
main.o: main.c
	gcc -c -ansi -Wall -pedantic main.c -o main.o
