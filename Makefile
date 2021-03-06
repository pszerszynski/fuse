IDIR =include
CC=g++
CFLAGS=-I$(IDIR) -std=c++1z -Os -fno-exceptions

ODIR=src/obj
SDIR=src
LDIR =../lib

LIBS=-lm -lpthread

_DEPS = data/Fuse_Object.hpp Operations.hpp data/Fuse_Table.hpp data/Fuse_Number.hpp data/Fuse_String.hpp data/Fuse_Bool.hpp data/Fuse_Function.hpp \
Fuse_Core.hpp Lexer.hpp Parser.hpp AST.hpp ast/Base.hpp ast/Literals.hpp ast/Block.hpp ast/FuncDef.hpp ast/Object.hpp ast/Variable.hpp ast/BinaryExpr.hpp \
ast/Assign.hpp ast/FuncCall.hpp ast/IfElse.hpp ast/TableConstructor.hpp ast/TableAccess.hpp ast/While.hpp ast/For.hpp ast/Break.hpp \
std/io.hpp std/math.hpp std/table.hpp Fuse.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o Fuse_Core.o Fuse_Funcs.o Lexer.o Parser.o Operations.o ast/Base.o ast/Literals.o ast/Block.o ast/FuncDef.o ast/Object.o ast/Variable.o ast/BinaryExpr.o \
ast/Assign.o ast/FuncCall.o ast/IfElse.o data/Fuse_Object.o data/Fuse_Table.o data/Fuse_Number.o data/Fuse_String.o data/Fuse_Bool.o data/Fuse_Function.o \
ast/TableConstructor.o ast/TableAccess.o ast/While.o ast/For.o ast/Break.o std/io.o std/math.o std/table.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

OUTPUT = fuse-test

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
make: $(OBJ)
	$(CC) -o $(OUTPUT) $^ $(CFLAGS) $(LIBS)
		
debug: $(OBJ)
	$(CC) -g -o $(OUTPUT) $^ $(CFLAGS) $(LIBS) 

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
