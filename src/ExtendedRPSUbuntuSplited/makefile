COMP = g++-5.3.0
LIBS = -ldl

rps_tournament:
	make the_tournament
rps_lib:
	make the_lib
	
#put all your object files here
COMMONOBJS = BoardImpl.o Bomb.o FightInfoImpl.o Flag.o Joker.o JokerChangeImpl.o MoveImpl.o Paper.o Piece.o PieceFactory.o PiecePositionImpl.o Player.o PointImpl.o Rock.o RPSPiece.o Scissors.o StrategyPiece.o
LIBOBJS = $(COMMONOBJS) RSPPlayer_309962264.o
LIBOBJS111111111 = $(COMMONOBJS) RSPPlayer_111111111.o
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
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
RSPPlayer_309962264.so: RSPPlayer_309962264.o
	$(COMP) -shared -Wl,-soname,RSPPlayer_309962264.so -o RSPPlayer_309962264.so $(LIBOBJS)
	
# RSPPlayer_111111111.o: RSPPlayer_309962264.cpp RSPPlayer_309962264.h \
 # PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h Move.h \
 # JokerChange.h BoardImpl.h Piece.h PieceFactory.h Player.h PointImpl.h \
 # FightInfoImpl.h PiecePositionImpl.h StrategyPiece.h Flag.h Paper.h \
 # RPSPiece.h Rock.h Scissors.h Bomb.h Joker.h MoveImpl.h JokerChangeImpl.h \
 # AlgorithmRegistration.h
	# $(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
# RSPPlayer_111111111.so: RSPPlayer_111111111.o
	# $(COMP) -shared -Wl,-soname,RSPPlayer_111111111.so -o RSPPlayer_111111111.so $(LIBOBJS111111111)

BoardImpl.o: BoardImpl.cpp BoardImpl.h Piece.h PieceFactory.h Player.h \
 PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h Move.h \
 JokerChange.h PointImpl.h FightInfoImpl.h StrategyPiece.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
Bomb.o:   Bomb.cpp Bomb.h Piece.h PieceFactory.h Player.h PlayerAlgorithm.h \
 Point.h PiecePosition.h Board.h FightInfo.h Move.h JokerChange.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
FightInfoImpl.o: FightInfoImpl.cpp FightInfoImpl.h FightInfo.h \
 PointImpl.h Point.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
# FilePlayerAlgorithm.o: FilePlayerAlgorithm.cpp FilePlayerAlgorithm.h \
 # PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h Move.h \
 # JokerChange.h ParserMoveFile.h Parser.h Player.h PieceFactory.h \
 # PointImpl.h ParserInitFile.h MoveImpl.h JokerChangeImpl.h
	# $(COMP) -fPIC $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
	
#Delete!!!!!!!!!!!!!!!!!!!!!!!!!
#FilePlayerAlgorithm.so: FilePlayerAlgorithm.o
#	$(COMP) -shared -fPIC -Wl,-soname,FilePlayerAlgorithm.so -o FilePlayerAlgorithm.so FilePlayerAlgorithm.o
	
Flag.o: Flag.cpp Flag.h Piece.h PieceFactory.h Player.h PlayerAlgorithm.h \
 Point.h PiecePosition.h Board.h FightInfo.h Move.h JokerChange.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
Game.o: Game.cpp Game.h BoardImpl.h Piece.h PieceFactory.h Player.h \
 PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h Move.h \
 JokerChange.h PointImpl.h FightInfoImpl.h Joker.h PiecePositionImpl.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Joker.o: Joker.cpp Joker.h Piece.h PieceFactory.h Player.h \
 PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h Move.h \
 JokerChange.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
JokerChangeImpl.o: JokerChangeImpl.cpp JokerChangeImpl.h PointImpl.h \
 Point.h JokerChange.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
Main.o: Main.cpp Game.h BoardImpl.h Piece.h PieceFactory.h Player.h \
 PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h Move.h \
 JokerChange.h PointImpl.h FightInfoImpl.h TournamentManager.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
MoveImpl.o: MoveImpl.cpp MoveImpl.h Point.h Move.h PointImpl.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
Paper.o: Paper.cpp Paper.h RPSPiece.h Piece.h PieceFactory.h Player.h \
 PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h Move.h \
 JokerChange.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
# Parser.o: Parser.cpp Parser.h Player.h PieceFactory.h PlayerAlgorithm.h \
 # Point.h PiecePosition.h Board.h FightInfo.h Move.h JokerChange.h \
 # PointImpl.h Game.h BoardImpl.h Piece.h FightInfoImpl.h Joker.h
	# $(COMP) $(CPP_COMP_FLAG) -c $*.cpp
# ParserInitFile.o: ParserInitFile.cpp ParserInitFile.h Parser.h Player.h \
 # PieceFactory.h PlayerAlgorithm.h Point.h PiecePosition.h Board.h \
 # FightInfo.h Move.h JokerChange.h PointImpl.h Game.h BoardImpl.h Piece.h \
 # FightInfoImpl.h Joker.h PiecePositionImpl.h
	# $(COMP) $(CPP_COMP_FLAG) -c $*.cpp
# ParserMoveFile.o: ParserMoveFile.cpp ParserMoveFile.h Parser.h Player.h \
 # PieceFactory.h PlayerAlgorithm.h Point.h PiecePosition.h Board.h \
 # FightInfo.h Move.h JokerChange.h PointImpl.h Game.h BoardImpl.h Piece.h \
 # FightInfoImpl.h Joker.h MoveImpl.h JokerChangeImpl.h
	# $(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Piece.o: Piece.cpp Piece.h PieceFactory.h Player.h PlayerAlgorithm.h \
 Point.h PiecePosition.h Board.h FightInfo.h Move.h JokerChange.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
PieceFactory.o:  PieceFactory.cpp PieceFactory.h Rock.h RPSPiece.h Piece.h \
 Player.h PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h \
 Move.h JokerChange.h Paper.h Scissors.h Bomb.h Joker.h Flag.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
PiecePositionImpl.o: PiecePositionImpl.cpp PiecePositionImpl.h \
 PointImpl.h Point.h PiecePosition.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
Player.o: Player.cpp Player.h PieceFactory.h PlayerAlgorithm.h Point.h \
 PiecePosition.h Board.h FightInfo.h Move.h JokerChange.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
PointImpl.o: PointImpl.cpp PointImpl.h Point.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
Rock.o: Rock.cpp Rock.h RPSPiece.h Piece.h PieceFactory.h Player.h \
 PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h Move.h \
 JokerChange.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
RPSPiece.o: RPSPiece.cpp RPSPiece.h Piece.h PieceFactory.h Player.h \
 PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h Move.h \
 JokerChange.h Joker.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
Scissors.o: Scissors.cpp Scissors.h RPSPiece.h Piece.h PieceFactory.h \
 Player.h PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h \
 Move.h JokerChange.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
StrategyPiece.o: StrategyPiece.cpp StrategyPiece.h Piece.h PieceFactory.h \
 Player.h PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h \
 Move.h JokerChange.h Joker.h Rock.h RPSPiece.h Paper.h Scissors.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -mcmodel=large
AlgorithmRegistration.o: AlgorithmRegistration.cpp \
 AlgorithmRegistration.h PlayerAlgorithm.h Point.h PiecePosition.h \
 Board.h FightInfo.h Move.h JokerChange.h TournamentManager.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
TournamentManager.o: TournamentManager.cpp TournamentManager.h \
 PlayerAlgorithm.h Point.h PiecePosition.h Board.h FightInfo.h Move.h \
 JokerChange.h Game.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

#all: $(EXEC) $(OBJS) RSPPlayer_309962264.so
the_tournament: $(EXEC) $(EXECOBJS)
the_lib: $(LIBOBJS) RSPPlayer_309962264.so

clean:
	rm -f *.so *.o $(EXEC)