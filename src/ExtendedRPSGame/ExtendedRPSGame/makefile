COMP = g++-5.3.0
#put all your object files here
OBJS = RSPPlayer_309962264.o BoardImpl.o Bomb.o FightInfoImpl.o FilePlayerAlgorithm.o Flag.o Game.o Joker.o JokerChangeImpl.o Main.o MoveImpl.o Paper.o Parser.o ParserInitFile.o ParserMoveFile.o Piece.o PieceFactory.o PiecePositionImpl.o Player.o PointImpl.o Rock.o RPSPiece.o Scissors.o StrategyPiece.o TournamentManager.o
#The executable filename DON'T CHANGE
EXEC = ex3
CPP_COMP_FLAG = -std=c++14 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) -o $@
#a rule for building a simple c++ source file
#use g++ -MM Main.cpp to see dependencies

RSPPlayer_309962264.o: RSPPlayer_309962264.cpp RSPPlayer_309962264.h \
 PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h Move.h \
 JokerChange.h BoardImpl.h Piece.h PieceFactory.h Player.h PointImpl.h \
 FightInfoImpl.h PiecePositionImpl.h StrategyPiece.h Flag.h Paper.h \
 RPSPiece.h Rock.h Scissors.h Bomb.h Joker.h MoveImpl.h JokerChangeImpl.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
BoardImpl.o: BoardImpl.cpp BoardImpl.h Piece.h PieceFactory.h Player.h \
 PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h Move.h \
 JokerChange.h PointImpl.h FightInfoImpl.h StrategyPiece.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Bomb.o:   Bomb.cpp Bomb.h Piece.h PieceFactory.h Player.h PlayerAlgorithm.h \
 Point.h PiecePosition.h Board.h FightInfo.h Move.h JokerChange.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
FightInfoImpl.o: FightInfoImpl.cpp FightInfoImpl.h FightInfo.h \
 PointImpl.h Point.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
FilePlayerAlgorithm.o: FilePlayerAlgorithm.cpp FilePlayerAlgorithm.h \
 PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h Move.h \
 JokerChange.h ParserMoveFile.h Parser.h Player.h PieceFactory.h \
 PointImpl.h ParserInitFile.h MoveImpl.h JokerChangeImpl.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Flag.o: Flag.cpp Flag.h Piece.h PieceFactory.h Player.h PlayerAlgorithm.h \
 Point.h PiecePosition.h Board.h FightInfo.h Move.h JokerChange.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Game.o: Game.cpp Game.h BoardImpl.h Piece.h PieceFactory.h Player.h \
 PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h Move.h \
 JokerChange.h PointImpl.h FightInfoImpl.h ParserInitFile.h Parser.h \
 ParserMoveFile.h Joker.h PiecePositionImpl.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Joker.o: Joker.cpp Joker.h Piece.h PieceFactory.h Player.h \
 PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h Move.h \
 JokerChange.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
JokerChangeImpl.o: JokerChangeImpl.cpp JokerChangeImpl.h PointImpl.h \
 Point.h JokerChange.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Main.o: Main.cpp Game.h BoardImpl.h Piece.h PieceFactory.h Player.h \
 PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h Move.h \
 JokerChange.h PointImpl.h FightInfoImpl.h FilePlayerAlgorithm.h \
 ParserMoveFile.h Parser.h RSPPlayer_309962264.h PiecePositionImpl.h \
 StrategyPiece.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
MoveImpl.o: MoveImpl.cpp MoveImpl.h Point.h Move.h PointImpl.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Paper.o: Paper.cpp Paper.h RPSPiece.h Piece.h PieceFactory.h Player.h \
 PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h Move.h \
 JokerChange.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Parser.o: Parser.cpp Parser.h Player.h PieceFactory.h PlayerAlgorithm.h \
 Point.h PiecePosition.h Board.h FightInfo.h Move.h JokerChange.h \
 PointImpl.h Game.h BoardImpl.h Piece.h FightInfoImpl.h Joker.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
ParserInitFile.o: ParserInitFile.cpp ParserInitFile.h Parser.h Player.h \
 PieceFactory.h PlayerAlgorithm.h Point.h PiecePosition.h Board.h \
 FightInfo.h Move.h JokerChange.h PointImpl.h Game.h BoardImpl.h Piece.h \
 FightInfoImpl.h Joker.h PiecePositionImpl.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
ParserMoveFile.o: ParserMoveFile.cpp ParserMoveFile.h Parser.h Player.h \
 PieceFactory.h PlayerAlgorithm.h Point.h PiecePosition.h Board.h \
 FightInfo.h Move.h JokerChange.h PointImpl.h Game.h BoardImpl.h Piece.h \
 FightInfoImpl.h Joker.h MoveImpl.h JokerChangeImpl.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Piece.o: Piece.cpp Piece.h PieceFactory.h Player.h PlayerAlgorithm.h \
 Point.h PiecePosition.h Board.h FightInfo.h Move.h JokerChange.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
PieceFactory.o:  PieceFactory.cpp PieceFactory.h Rock.h RPSPiece.h Piece.h \
 Player.h PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h \
 Move.h JokerChange.h Paper.h Scissors.h Bomb.h Joker.h Flag.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
PiecePositionImpl.o: PiecePositionImpl.cpp PiecePositionImpl.h \
 PointImpl.h Point.h PiecePosition.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Player.o: Player.cpp Player.h PieceFactory.h PlayerAlgorithm.h Point.h \
 PiecePosition.h Board.h FightInfo.h Move.h JokerChange.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
PointImpl.o: PointImpl.cpp PointImpl.h Point.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Rock.o: Rock.cpp Rock.h RPSPiece.h Piece.h PieceFactory.h Player.h \
 PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h Move.h \
 JokerChange.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RPSPiece.o: RPSPiece.cpp RPSPiece.h Piece.h PieceFactory.h Player.h \
 PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h Move.h \
 JokerChange.h Joker.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Scissors.o: Scissors.cpp Scissors.h RPSPiece.h Piece.h PieceFactory.h \
 Player.h PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h \
 Move.h JokerChange.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
StrategyPiece.o: StrategyPiece.cpp StrategyPiece.h Piece.h PieceFactory.h \
 Player.h PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h \
 Move.h JokerChange.h Joker.h Rock.h RPSPiece.h Paper.h Scissors.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
TournamentManager.o: TournamentManager.cpp TournamentManager.h \
 PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h Move.h \
 JokerChange.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

clean:
	rm -f $(OBJS) $(EXEC)