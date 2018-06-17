COMP = g++-5.3.0
LIBS = -ldl

default:
	make the_tournament
	make the_lib

rps_tournament:
	make the_tournament
rps_lib:
	make the_lib
	
#put all your object files here
COMMONOBJS = BoardImpl.o Bomb.o FightInfoImpl.o Flag.o Joker.o Paper.o Piece.o PieceFactory.o Player.o PointImpl.o Rock.o RPSPiece.o Scissors.o StrategyPiece.o
LIBOBJS = $(COMMONOBJS) RSPPlayer_309962264.o
EXECOBJS = $(COMMONOBJS) Game.o AlgorithmRegistration.o TournamentManager.o Main.o
#The executable filename DON'T CHANGE
EXEC = ex3
CPP_COMP_FLAG = -std=c++14 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG
	
$(EXEC): $(EXECOBJS)
	$(COMP) -rdynamic -o $(EXEC) $(EXECOBJS) $(LIBS) -pthread

#a rule for building a simple c++ source file
#use g++ -MM Main.cpp to see dependencies

RSPPlayer_309962264.o: RSPPlayer_309962264.cpp RSPPlayer_309962264.h \
 PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h Move.h \
 JokerChange.h BoardImpl.h Piece.h PieceFactory.h Player.h PointImpl.h \
 FightInfoImpl.h PiecePositionImpl.h StrategyPiece.h Flag.h Paper.h \
 RPSPiece.h Rock.h Scissors.h Bomb.h Joker.h MoveImpl.h JokerChangeImpl.h \
 AlgorithmRegistration.h
	$(COMP) -fPIC $(CPP_COMP_FLAG) -c $*.cpp
RSPPlayer_309962264.so: RSPPlayer_309962264.o
	$(COMP) -shared -Wl,-soname,RSPPlayer_309962264.so -o RSPPlayer_309962264.so $(LIBOBJS)
	
BoardImpl.o: BoardImpl.cpp BoardImpl.h Piece.h PieceFactory.h Player.h \
 PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h Move.h \
 JokerChange.h PointImpl.h FightInfoImpl.h StrategyPiece.h
	$(COMP) -fPIC $(CPP_COMP_FLAG) -c $*.cpp
Bomb.o:   Bomb.cpp Bomb.h Piece.h PieceFactory.h Player.h PlayerAlgorithm.h \
 Point.h PiecePosition.h Board.h FightInfo.h Move.h JokerChange.h
	$(COMP) -fPIC $(CPP_COMP_FLAG) -c $*.cpp
FightInfoImpl.o: FightInfoImpl.cpp FightInfoImpl.h FightInfo.h \
 PointImpl.h Point.h
	$(COMP) -fPIC $(CPP_COMP_FLAG) -c $*.cpp
	
Flag.o: Flag.cpp Flag.h Piece.h PieceFactory.h Player.h PlayerAlgorithm.h \
 Point.h PiecePosition.h Board.h FightInfo.h Move.h JokerChange.h
	$(COMP) -fPIC $(CPP_COMP_FLAG) -c $*.cpp
Game.o: Game.cpp Game.h BoardImpl.h Piece.h PieceFactory.h Player.h \
 PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h Move.h \
 JokerChange.h PointImpl.h FightInfoImpl.h Joker.h PiecePositionImpl.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Joker.o: Joker.cpp Joker.h Piece.h PieceFactory.h Player.h \
 PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h Move.h \
 JokerChange.h
	$(COMP) -fPIC $(CPP_COMP_FLAG) -c $*.cpp
Main.o: Main.cpp Game.h BoardImpl.h Piece.h PieceFactory.h Player.h \
 PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h Move.h \
 JokerChange.h PointImpl.h FightInfoImpl.h TournamentManager.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Paper.o: Paper.cpp Paper.h RPSPiece.h Piece.h PieceFactory.h Player.h \
 PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h Move.h \
 JokerChange.h
	$(COMP) -fPIC $(CPP_COMP_FLAG) -c $*.cpp
Piece.o: Piece.cpp Piece.h PieceFactory.h Player.h PlayerAlgorithm.h \
 Point.h PiecePosition.h Board.h FightInfo.h Move.h JokerChange.h
	$(COMP) -fPIC $(CPP_COMP_FLAG) -c $*.cpp
PieceFactory.o:  PieceFactory.cpp PieceFactory.h Rock.h RPSPiece.h Piece.h \
 Player.h PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h \
 Move.h JokerChange.h Paper.h Scissors.h Bomb.h Joker.h Flag.h
	$(COMP) -fPIC $(CPP_COMP_FLAG) -c $*.cpp
Player.o: Player.cpp Player.h PieceFactory.h PlayerAlgorithm.h Point.h \
 PiecePosition.h Board.h FightInfo.h Move.h JokerChange.h
	$(COMP) -fPIC $(CPP_COMP_FLAG) -c $*.cpp
PointImpl.o: PointImpl.cpp PointImpl.h Point.h
	$(COMP) -fPIC $(CPP_COMP_FLAG) -c $*.cpp
Rock.o: Rock.cpp Rock.h RPSPiece.h Piece.h PieceFactory.h Player.h \
 PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h Move.h \
 JokerChange.h
	$(COMP) -fPIC $(CPP_COMP_FLAG) -c $*.cpp
RPSPiece.o: RPSPiece.cpp RPSPiece.h Piece.h PieceFactory.h Player.h \
 PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h Move.h \
 JokerChange.h Joker.h
	$(COMP) -fPIC $(CPP_COMP_FLAG) -c $*.cpp
Scissors.o: Scissors.cpp Scissors.h RPSPiece.h Piece.h PieceFactory.h \
 Player.h PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h \
 Move.h JokerChange.h
	$(COMP) -fPIC $(CPP_COMP_FLAG) -c $*.cpp
StrategyPiece.o: StrategyPiece.cpp StrategyPiece.h Piece.h PieceFactory.h \
 Player.h PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h \
 Move.h JokerChange.h Joker.h Rock.h RPSPiece.h Paper.h Scissors.h
	$(COMP) -fPIC $(CPP_COMP_FLAG) -c $*.cpp
AlgorithmRegistration.o: AlgorithmRegistration.cpp \
 AlgorithmRegistration.h PlayerAlgorithm.h Point.h PiecePosition.h \
 Board.h FightInfo.h Move.h JokerChange.h TournamentManager.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
TournamentManager.o: TournamentManager.cpp TournamentManager.h \
 PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h Move.h \
 JokerChange.h Game.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

the_tournament: $(EXEC) $(EXECOBJS)
the_lib: $(LIBOBJS) RSPPlayer_309962264.so

clean:
	rm -f *.so *.o $(EXEC)