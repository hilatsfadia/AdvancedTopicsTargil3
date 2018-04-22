COMP = g++-5.3.0
#put all your object files here
OBJS = Board.o Bomb.o Flag.o Game.o Joker.o Main.o Paper.o Parser.o ParserInitFile.o ParserMoveFile.o Piece.o PieceFactory.o Player.o Rock.o Scissors.o
#The executabel filename DON'T CHANGE
EXEC = ex1
CPP_COMP_FLAG = -std=c++14 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) -o $@
#a rule for building a simple c++ source file
#use g++ -MM Main.cpp to see dependencies
Board.o: Board.cpp Board.h Piece.h PieceFactory.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Bomb.o: Bomb.cpp Bomb.h Piece.h PieceFactory.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Flag.o: Flag.cpp Flag.h Piece.h PieceFactory.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Game.o: Game.cpp Game.h Player.h PieceFactory.h Board.h Piece.h \
 ParserInitFile.h Parser.h ParserMoveFile.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Joker.o: Joker.cpp Joker.h Piece.h PieceFactory.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Main.o: Main.cpp
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Paper.o: Paper.cpp Paper.h Piece.h PieceFactory.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Parser.o: Parser.cpp Parser.h Player.h PieceFactory.h Board.h Piece.h \
 Game.h Joker.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
ParserInitFile.o: ParserInitFile.cpp ParserInitFile.h Parser.h Player.h \
 PieceFactory.h Board.h Piece.h Game.h Joker.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
ParserMoveFile.o: ParserMoveFile.cpp ParserMoveFile.h Parser.h Player.h \
 PieceFactory.h Board.h Piece.h Game.h Joker.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Piece.o: Piece.cpp Piece.h PieceFactory.h Player.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
PieceFactory.o: PieceFactory.cpp PieceFactory.h Rock.h Piece.h Paper.h \
 Scissors.h Bomb.h Joker.h Flag.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Player.o: Player.cpp Player.h PieceFactory.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Rock.o: Rock.cpp Rock.h Piece.h PieceFactory.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Scissors.o: Scissors.cpp Scissors.h Piece.h PieceFactory.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
clean:
	rm -f $(OBJS) $(EXEC)