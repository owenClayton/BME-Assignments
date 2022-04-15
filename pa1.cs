#nullable enable
using System;
using static System.Console;

namespace Bme121
{
    record Player( string Colour, string Symbol, string Name );
    
    static partial class Program
    {
        // Display common text for the top of the screen.
        
        static void Welcome( )
        {
            WriteLine("Welcome to Othello!");
        }
        
        // Collect a player name or default to form the player record.
        
        static Player NewPlayer( string colour, string symbol, string defaultName )
        {
            Write( "Type the {0} disc ({1}) player name or <Enter> for '{2}': ", colour, symbol, defaultName);
            string response = ReadLine ( )!; 
            if (response.Length == 0 ) //<Enter> for default name (colour)
            {
                return new Player(colour, symbol, defaultName); 
            }
            else 
            {
                defaultName = response; //fill in Player Name with what they type
                return new Player( colour , symbol , defaultName );
            }
        }
        
        // Determine which player goes first or default.
        
        static int GetFirstTurn( Player[ ] players, int defaultFirst )
        {
            Write("Choose who will go first (white or black) or <Enter> for {0}: ", players[0]);
            string whatColour = ReadLine( )!;
            if (whatColour =="white") 
            {
                defaultFirst = 1;
            }
            return defaultFirst; 
        }
        
        // Get a board size (between 4 and 26 and even) or default, for one direction.
        
        static int GetBoardSize( string direction, int defaultSize )
        {
            Write( "Enter board {0} (4 to 26, even) or <Enter> for {1}: ", direction, defaultSize );
            string response = ReadLine( )!; 
            if (response.Length == 0)
            {
                return defaultSize;
            }
            else 
            {
                defaultSize = int.Parse(response); 
                return defaultSize; 
            }
        }
        
        // Get a move from a player.
        
        static string GetMove( Player player )
        {
            WriteLine("Turn is {0} disk, ({1}) player: {2}", player.Colour, player.Symbol, player.Name);
            WriteLine("Pick a cell by its row then column names (like 'bc') to play there.");
            WriteLine("Use 'skip' to end your turn, use 'quit' to end the game.");
            WriteLine("Enter your choice: "); 
            string response = ReadLine ( )!; 
            return response;
        }
        
        // Try to make a move. Return true if it worked.
        
        static bool TryMove( string[ , ] board, Player player, string move )
        {
            string r = move.Substring(0,1); 
            string c = move.Substring(1,1);
            int row = IndexAtLetter(r);
            int col = IndexAtLetter(c);
            if (move.Length == 2) 
            {
                if (row >= 0 && col >= 0 && row < board.GetLength( 0 )  && col < board.GetLength(1) ) 
                {
                    if (board [row,col] == " ")
                    {
                       // try all 8 directions
                        bool DirectionWorked1 = TryDirection (  board, player, row, 0, col, 1);
                        bool DirectionWorked2 = TryDirection (  board, player, row, 0, col, -1);
                        bool DirectionWorked3 = TryDirection (  board, player, row, 1, col, 0);
                        bool DirectionWorked4 = TryDirection (  board, player, row, -1, col, 0);
                        bool DirectionWorked5 = TryDirection (  board, player, row, 1, col, 1);
                        bool DirectionWorked6 = TryDirection (  board, player, row, 1, col, -1);
                        bool DirectionWorked7 = TryDirection (  board, player, row, -1, col, 1);
                        bool DirectionWorked8 = TryDirection (  board, player, row, -1, col, -1);
                        if (DirectionWorked1 ||DirectionWorked2 ||DirectionWorked3 
                        ||DirectionWorked4 ||DirectionWorked5 ||DirectionWorked6 ||DirectionWorked7 
                        ||DirectionWorked8 )
                        {
                            return true ;
                        }
                        else return false;
                    }
                    else return false;
                }
                else return false; 
            }
            else return false;
        }
        
        // Do the flips along a direction specified by the row and column delta for one step.
        
        static bool TryDirection( string[ , ] board, Player player,
            int moveRow, int deltaRow, int moveCol, int deltaCol )
        {
            //checking that the first move will not cause it to be outside of the bounds of array
            if ((moveRow == 0 && deltaRow == -1 )|| (moveCol == 0 && deltaCol == -1) || 
                 (moveRow == board.GetLength(0)-1 && deltaRow == 1) || (moveCol == board.GetLength(1)-1 && deltaCol == 1))
                {
                    return false;
                }
            //ensure that the move was made next to an opposing colour
            if (board [moveRow + deltaRow , moveCol + deltaCol] != player.Symbol && 
                board [moveRow + deltaRow, moveCol + deltaCol] != " ") 
            {
                int r = moveRow;
                int c = moveCol;
                bool done = false; 
                bool written = false; 
                //move along specified axis to check if you run into the player's symbol, 
                //if they run out of board or hit a blank space, will return false. 
                while (!done)
                {
                    r = r + deltaRow; 
                    c = c + deltaCol; 
                    if (c > board.GetLength(1) -1 || r > board.GetLength(0) - 1 || c < 0 || r < 0 || board [r,c] == player.Symbol || board [r,c] == " ") 
                    {
                        done = true;
                    }
                }
                if (r > board.GetLength(0) -1 || c > board.GetLength(1) -1 || c <0 || r<0|| board [r,c] == " ")
                {
                    return false; 
                }
                else
                {
                    int j = moveRow;
                    int k = moveCol;
                    board[j,k] = player.Symbol; 
                    //If true, go back through and turn all the symbols into the player's
                    while (!written)
                    {
                        j = j + deltaRow; 
                        k = k + deltaCol; 
                        board [j, k] = player.Symbol; 
                        if (j == r && k == c)
                        {
                            written = true; 
                        }
                    } 
                    return true;
                }
            }
            else return false; 
        }

        
        // Count the discs to find the score for a player.
        
        static int GetScore( string[ , ] board, Player player )
        {
            int whiteScore = 0;
            int blackScore = 0;
            for (int i =0; i<board.GetLength(0); i++)
            {
                for (int j = 0; j<board.GetLength(1); j++) 
                {
                    if (board[i,j] == "X")
                    {
                        whiteScore ++; 
                    }
                    else if (board [i,j] == "O")
                    {
                        blackScore ++; 
                    }
                }
            }
            if (player.Symbol == "O")
            { 
                return blackScore; 
            }
            else 
            {
                return whiteScore; 
            }
        }
        
        // Display a line of scores for all players.
        
        static void DisplayScores( string[ , ] board, Player [ ] players )
        {
            Write("Scores: ");
            for (int x = 0; x<players.Length; x++)
            {
                Write( "{0} {1} ", players[x].Name, GetScore(board, players[x]));
            }
            WriteLine();
        }
        
        // Display winner(s) and categorize their win over the defeated player(s)
        
        static void DisplayWinners( string[ , ] board, Player[ ] players )
        {
            int Black = GetScore(board, players[0]);
            int White = GetScore(board, players[1]);
            if (Black==White)
            {
                WriteLine();
                WriteLine("The game ended in a tie!");
            }
            else 
            {
                if (Black>White)
                {
                    WriteLine();
                    WriteLine( "{0} wins!", players[0].Name);
                    if ((Black - White) <= 10  && (Black - White) >= 2) WriteLine( "A CLOSE GAME!");
                    if ((Black - White) <= 24  && (Black - White) >= 12) WriteLine( "A HOT GAME!");
                    if ((Black - White) <= 38  && (Black - White) >= 26) WriteLine( "A FIGHT GAME!");
                    if ((Black - White) <= 52  && (Black - White) >= 40) WriteLine( "A WALKAWAY GAME!");
                    if ((Black - White) <= 64  && (Black - White) >= 54) WriteLine( "A PERFECT GAME!");
                }
                else 
                {
                    WriteLine();
                    WriteLine( "{0} wins!", players[1].Name); 
                     if ((White - Black) <= 10  && (White - Black) >= 2) WriteLine( "A CLOSE GAME!");
                    if ((White - Black) <= 24  && (White - Black) >= 12) WriteLine( "A HOT GAME!");
                    if ((White - Black) <= 38  && (White - Black) >= 26) WriteLine( "A FIGHT GAME!");
                    if ((White - Black) <= 52  && (White - Black) >= 40) WriteLine( "A WALKAWAY GAME!");
                    if ((White - Black) <= 64  && (White - Black) >= 54) WriteLine( "A PERFECT GAME!");
                }
            }
        }
        
    /*  I was attempting to get the game to auto-end if no available moves. 
    
        static bool GameOver (string [ , ] board, Player player)
        {
            for( int i = 0; i < board.GetLength(0); i++)
            {
                for (int j = 0; j < board.GetLength(1); j++)
                {
                    if (board[i,j] == " ")
                    {
                        string location = i.ToString()+j.ToString(); 
                        bool result = TryMove2(board, player, location);
                        if (result == false)
                        {
                            return result; 
                            WriteLine(result);
                        }
                        else
                        {
                            return true;
                        }
                    }
                }
            }
            return false;
        }
        
         static bool TryMove2( string[ , ] board, Player player, string move )
        {
            string r = move.Substring(0,1); 
            string c = move.Substring(1,1);
            int row = IndexAtLetter(r);
            int col = IndexAtLetter(c);
             if (move.Length == 2) 
            {
                if (row >= 0 && col >= 0 && row < board.GetLength( 0 )  && col < board.GetLength(1) ) 
                {
                    if (board [row,col] == " ")
                    {
                        // try all 8 directions
                        bool DirectionWorked1 = TryDirection2 (  board, player, row, 0, col, 1);
                        bool DirectionWorked2 = TryDirection2 (  board, player, row, 0, col, -1);
                        bool DirectionWorked3 = TryDirection2 (  board, player, row, 1, col, 0);
                        bool DirectionWorked4 = TryDirection2 (  board, player, row, -1, col, 0);
                        bool DirectionWorked5 = TryDirection2 (  board, player, row, 1, col, 1);
                        bool DirectionWorked6 = TryDirection2 (  board, player, row, 1, col, -1);
                        bool DirectionWorked7 = TryDirection2 (  board, player, row, -1, col, 1);
                        bool DirectionWorked8 = TryDirection2 (  board, player, row, -1, col, -1);
                        if (DirectionWorked1 ||DirectionWorked2 ||DirectionWorked3 
                        ||DirectionWorked4 ||DirectionWorked5 ||DirectionWorked6 ||DirectionWorked7 
                        ||DirectionWorked8 )
                        {
                            return false ;
                        }
                        else return true;  
                    }
                    else return true;
                }
                else return true; 
            }
            else return true;         
        }
        
        static bool TryDirection2( string[ , ] board, Player player,
        int moveRow, int deltaRow, int moveCol, int deltaCol )
        {
            //checking that the first move will not cause it to be outside of the bounds of array
            if ((moveRow == 0 && deltaRow == -1 )|| (moveCol == 0 && deltaCol == -1) || 
                 (moveRow == board.GetLength(0)-1 && deltaRow == 1) || (moveCol == board.GetLength(1)-1 && deltaCol == 1))
                {
                    return false;
                }
            //ensure that the move was made next to an opposing colour
            if (board [moveRow + deltaRow , moveCol + deltaCol] != player.Symbol && 
                board [moveRow + deltaRow, moveCol + deltaCol] != " ") 
            {
                int r = moveRow;
                int c = moveCol;
                bool done = false; 
                //move along specified axis to check if you run into the player's symbol, 
                //if they run out of board or hit a blank space, will return false. 
                while (!done)
                {
                    r = r + deltaRow; 
                    c = c + deltaCol; 
                    if (c > board.GetLength(1) -1 || r > board.GetLength(0) - 1 || c < 0 || r < 0 || board [r,c] == player.Symbol || board [r,c] == " ") 
                    {
                        done = true;
                    }
                }
                if (r > board.GetLength(0) -1 || c > board.GetLength(1) -1 || c <0 || r<0|| board [r,c] == " ")
                {
                    return false; 
                }
                else
                {
                    return true;
                }
            }
            else return false; 
        }
   */     
        
        static void Main( )
        {
            Welcome( );
            
            Player[ ] players = new Player[ ] 
            {
                NewPlayer( colour: "black", symbol: "X", defaultName: "Black" ),
                NewPlayer( colour: "white", symbol: "O", defaultName: "White" ),
            };
            
            int turn = GetFirstTurn( players, defaultFirst: 0 );
            
            int rows = GetBoardSize( direction: "rows", defaultSize: 8 );
            int cols = GetBoardSize( direction: "columns", defaultSize: 8 );
            
            string[ , ] game = NewBoard( rows, cols );
            
            // Play the game.
            
            bool gameOver = false;
            while( ! gameOver )
            {
                DisplayBoard( game ); 
                DisplayScores( game, players );
                string move = GetMove( players[ turn ] );
                if( move == "quit" ) gameOver = true;
                else
                {
                    if (move == "skip")
                    {
                        turn = (turn + 1)%players.Length;
                    }
                    else
                    {
                        bool madeMove = TryMove( game, players[ turn ], move );
                        if( madeMove ) turn = ( turn + 1 ) % players.Length;
                        else 
                        {
                            Write( " Your choice didn't work!" );
                            Write( " Press <Enter> to try again." );
                            ReadLine( ); 
                        }
                    }
                }
                
            }
            
            // Show fhe final results.
            
            DisplayWinners( game, players );
            WriteLine( );
        }
    }
}
