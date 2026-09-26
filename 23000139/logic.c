#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "types.h"

  void initializePlayer(Player* player, const char* color);
  void move(Player* player, Player players[], int numPlayers);
  void moveRed(Player* player, Player players[], int numPlayers);
  void moveYellow(Player* player, Player players[], int numPlayers);
  void moveGreen(Player* player, Player players[], int numPlayers);
  void moveBlue(Player* player, Player players[], int numPlayers);
  int rollDice();
  int toss();
  void handleMysteryCell(Player *player, int pieceIndex, int mysteryEffect);
  int findPlayerIndexByColor(Player players[], const char* color);
  void capturePiece(Player* player, Player players[], int numPlayers);
  int findstartingplayer(int rolls[]);
  void displayStatus(Player players[]);
  int calculateDistancetoHome(Player* opponent, int index);
  void gamestart();
     
int toss(){
	return rand() % 2;
	}
void gamestart(){
srand(5);
    int rolls[NUM_PLAYERS];
    int roundCount = 1;
    int startingplayer;
    Player players[NUM_PLAYERS];
    int numPlayers = NUM_PLAYERS;
    int winnerFound = 0;
    const char* fpo[NUM_PLAYERS];
    const char* playerColors[NUM_PLAYERS] = {"Red", "Yellow", "Green", "Blue"};

    initializePlayer(&players[0], "Red");
    initializePlayer(&players[1], "Yellow");
    initializePlayer(&players[2], "Green");
    initializePlayer(&players[3], "Blue");

    // Determine the order of players based on dice rolls
    do {
        printf("Rolling the dice ...\n");
        for (int i = 0; i < NUM_PLAYERS; i++) {
            rolls[i] = rollDice();
            printf("%s rolls %d\n", playerColors[i], rolls[i]);
        }
        startingplayer = findstartingplayer(rolls);

        if (startingplayer == -1) {
            printf("There's a tie! Re-rolling the dice ...\n\n");
        }
    } while (startingplayer == -1);

    printf("\n%s player has the highest roll and will begin the game.\n", playerColors[startingplayer]);
    printf("The order of a single round is:\n");

    for (int i = 0; i < NUM_PLAYERS; i++) {
        fpo[i] = playerColors[(startingplayer + i) % NUM_PLAYERS];
        printf("%s\n", fpo[i]);
    }

    // Game loop
    while (!winnerFound) {
        for (int i = 0; i < NUM_PLAYERS; i++) {
            int playerIndex = findPlayerIndexByColor(players, fpo[i]);
            move(&players[playerIndex], players, numPlayers);

            // Check if the player has won
            if (players[playerIndex].piecesInHome == NUM_PIECES) {
                printf("%s player wins the game!\n", players[playerIndex].color);
                winnerFound = 1;
                break;
            }
        }
	printf("\n\tRound %d summary \n",roundCount);
	roundCount++;
        displayStatus(players);
    }
}
//finding player mechanism
int findstartingplayer(int rolls[]) {
    int maxRoll = 0;
    int maxCount = 1;
    int startingplayer = -1;

    for (int i = 0; i < NUM_PLAYERS; i++) {
        if (rolls[i] > maxRoll) {
            maxRoll = rolls[i];
            startingplayer = i;
        } else if (rolls[i] == maxRoll) {
            maxCount++;
        }
    }
    if (maxCount > 1) {
        return -1;
    }
    return startingplayer;
}

//find the player
int findPlayerIndexByColor(Player players[], const char* color) {
    for (int i = 0; i < NUM_PLAYERS; i++) {
        if (strcmp(players[i].color, color) == 0) {
            return i;
        }
    }
    return -1;  // Color not found
}
/*
 * need some work for this function
void moveTostartingpoint(Player *player,int pieceIndex){
        int direction = toss();
        player->pieces[pieceIndex].status==BOARD;
        player->piecesInBoard++;
        player->piecesInBase--;

 if(strcmp(player->color,"Red")==0){
         player->pieces[pieceIndex].location=RX;
         }else if (strcmp(player->color,"Yellow")==0){
         player->pieces[pieceIndex].location=YX;
         }else if(strcmp(player->color,"Green")==0){
         player->pieces[pieceIndex].location=GX;
         }else if(strcmp(player->color,"Blue")==0){
         player->pieces[pieceIndex].location=BX;
         }

        player->pieces[pieceIndex].direction = (direction == 1 ? CLOCKWISE : ANTICLOCKWISE);
               
        if (direction == 1) {
                    player->pieces[pieceIndex].xpass++;
                }

       printf("%s's piece %s moves to the board at position %d and will move in %s direction.\n", player->color, player->pieces[pieceIndex].id, player->pieces[pieceIndex].location, player->pieces[pieceIndex].direction == 1 ? "Clockwise" : "Counterclockwise");

 }
*/

//initalising the players
void initializePlayer(Player* player, const char* color) {
    strcpy(player->color, color);
    player->piecesInHome = 0; 
    
    if (strcmp(color, "Red") == 0) {
        player->homeStart = HOME_START_RED;
    } else if (strcmp(color, "Yellow") == 0) {
        player->homeStart = HOME_START_YELLOW;
    } else if (strcmp(color, "Green") == 0) {
        player->homeStart = HOME_START_GREEN;
    } else if (strcmp(color, "Blue") == 0) {
        player->homeStart = HOME_START_BLUE;
    }

    for (int i = 0; i < NUM_PIECES; i++) {
        snprintf(player->pieces[i].id, sizeof(player->pieces[i].id), "%c%d", color[0], i + 1);
        player->pieces[i].location = BASE_LOCATION;  // Start in base
        player->pieces[i].captured = 0;
        player->pieces[i].status = BASE;
        player->pieces[i].direction = CLOCKWISE;
        player->piecesInBoard=0;
        player->piecesInBase=4;
        player->pieces[i].xpass=0;
        player->pieces[i].isBlocked = 0;
        player->pieces[i].isEnergized = 0;
        player->pieces[i].isSick = 0;
        player->pieces[i].skipRounds = 0;
    }
}
//assinging each color pieces to relevant move function
void move(Player* player, Player players[], int numPlayers) {
    if (strcmp(player->color, "Red") == 0) {
        moveRed(player, players, numPlayers);
    } else if (strcmp(player->color, "Yellow") == 0) {
        moveYellow(player, players, numPlayers);
    } else if (strcmp(player->color, "Green") == 0) {
        moveGreen(player, players, numPlayers);
    } else if (strcmp(player->color, "Blue") == 0) {
        moveBlue(player, players, numPlayers);
    }
}
	//dice roll mechanism
int rollDice() {
    return (rand() % 6) + 1;
}
	//capturing mechanism
void capturePiece(Player* player, Player players[], int numPlayers) {
    for (int i = 0; i < numPlayers; i++) {
        if (strcmp(players[i].color, player->color) != 0) {
            for (int j = 0; j < NUM_PIECES; j++) {
                if (players[i].pieces[j].status == BOARD && players[i].pieces[j].location == player->pieces->location) {
                    players[i].pieces[j].status = BASE;
                    players[i].pieces[j].location = BASE_LOCATION;
                    printf("%s piece %s lands on square %d ,captures %s's piece %s and returns it to the base.\n",player->color,player->pieces->id,player->pieces->location, players[i].color, players[i].pieces[j].id);
                    players[i].piecesInBoard--;
                    players[i].piecesInBase++;
                }
            }
        }
    }
}
// Function to handle mystery cell effects
void handleMysteryCell(Player *player, int pieceIndex, int mysteryEffect) {
    printf("%s player lands on a mystery cell and ", player->color);
    switch (mysteryEffect) {
        case 0:
            player->pieces[pieceIndex].location = (player->pieces[pieceIndex].location + 10) % BOARD_SIZE;
            printf("is teleported to location %d.\n", player->pieces[pieceIndex].location);
            break;
        case 1:
            player->pieces[pieceIndex].isEnergized = 1;
            printf("feels energized, and movement speed doubles.\n");
            break;
        case 2:
            player->pieces[pieceIndex].isSick = 1;
            printf("feels sick, and movement speed halves.\n");
            break;
        case 3:
            player->pieces[pieceIndex].skipRounds = 4;
            printf("attends briefing and cannot move for four rounds.\n");
            break;
        case 4:
            player->pieces[pieceIndex].location = -1;
            player->pieces[pieceIndex].status = BASE;
            player->piecesInBoard--;
            player->piecesInBase++;
            printf("is movement-restricted and is teleported to base.\n");
            break;
        case 5:
            player->pieces[pieceIndex].direction *= -1;
            printf("changes to moving in %s direction.\n",
                   player->pieces[pieceIndex].direction == 1 ? "clockwise" : "counterclockwise");
            break;
    }
}
	//calculating the distance from piece to it's home mechanism
int calculateDistancetoHome(Player* opponent, int pieceIndex) {
    int distance;

    if (opponent->pieces[pieceIndex].direction == CLOCKWISE) {
         
            distance = BOARD_SIZE + opponent->homeStart - opponent->pieces[pieceIndex].location;
        
    } else if (opponent->pieces[pieceIndex].direction == ANTICLOCKWISE) {
            distance = opponent->pieces[pieceIndex].location - opponent->homeStart + BOARD_SIZE ;
       
        }
    return distance;
}
	//calculating the to capture for red player
int attemptCapture(Player* player, Player players[], int numPlayers, int dice) {
    int bestPieceIndex = -1;
    int minDistance = BOARD_SIZE;

    for (int i = 0; i < NUM_PIECES; i++) {
        if (player->pieces[i].status == BOARD) {
            int new_location = (player->pieces[i].location + dice * player->pieces[i].direction) % BOARD_SIZE;
            if (new_location < 0) {
                new_location = BOARD_SIZE + new_location;
            }

            for (int j = 0; j < numPlayers; j++) {
                if (&players[j] != player) {
                    for (int k = 0; k < NUM_PIECES; k++) {
                        if (players[j].pieces[k].status == BOARD && players[j].pieces[k].location == new_location) {
                            int distance = calculateDistancetoHome(&players[j], j);
                            if (distance < minDistance) {
                                minDistance = distance;
                                bestPieceIndex = i;
                            }
                        }
                    }
                }
            }
        }
    }

    if (bestPieceIndex != -1) {
        int old_location = player->pieces[bestPieceIndex].location;
        player->pieces[bestPieceIndex].location = (player->pieces[bestPieceIndex].location + dice * player->pieces[bestPieceIndex].direction) % BOARD_SIZE;
        if (player->pieces[bestPieceIndex].location < 0) {
            player->pieces[bestPieceIndex].location = BOARD_SIZE + player->pieces[bestPieceIndex].location;
        }

        printf("%s's piece %s moves to location %d on the board and captures an opponent's piece.\n", player->color, player->pieces[bestPieceIndex].id, player->pieces[bestPieceIndex].location);

        // Call capturePiece() to handle the capture logic
        capturePiece(player, players, numPlayers);
        return 1;
    }

    return 0;
}

void moveRed(Player* player, Player players[], int numPlayers) {
    int roll6count = 0;

    while (1) {
        int dice = rollDice();
        printf("%s player rolls a %d\n", player->color, dice);

        if (dice == 6) {
            roll6count++;
            if (roll6count == 3) {
                printf("%s rolled three consecutive 6s. Turn ends.\n", player->color);
                return;
            }
        } else {
            roll6count = 0;
        }

        if (attemptCapture(player, players, numPlayers, dice)) {
            break;
        }

        for (int i = 0; i < NUM_PIECES; i++) {
            if (player->pieces[i].status == BASE && dice == 6) {

                capturePiece(player, players, numPlayers);
  	        player->pieces[i].status = BOARD;
                player->pieces[i].location = RX; 
                player->piecesInBoard++;
                player->piecesInBase--;
                int direction = toss();
                player->pieces[i].direction = (direction == 1 ? CLOCKWISE:ANTICLOCKWISE);
                if(direction ==1){
                player->pieces[i].xpass++;
                }  
		// Move only one piece to the board
                 printf("%s's piece %s moves to the board at position %d.(Starting position of %s) and will move in %s direction\n", player->color, player->pieces[i].id, player->pieces[i].location,player->color,player->pieces[i].direction==1 ? "Clockwise" : "Counterclockwise");
printf("\n%s player has now %d/4 on pieces on the board and %d/4 pieces on the base\n",player->color,player->piecesInBoard,player->piecesInBase); 
                 break;

            } else if (player->pieces[i].status == BOARD) {
                    if(player->pieces[i].isEnergized){
                    		dice *= 2;
                    }else if(player->pieces[i].isSick){
                    		dice /= 2;
                    }
               		 int old_location = player->pieces[i].location;
                	 int new_location = (player->pieces[i].location + dice * player->pieces[i].direction) % BOARD_SIZE;
                    if (new_location < 0) {
                    	 player->pieces[i].location = BOARD_SIZE + new_location;
               	   }else{
                  	 player->pieces[i].location = new_location;
                }

                	printf("%s moves piece %s moves from location %d to location %d by %d units in %s direction\n", player->color, player->pieces[i].id,old_location, player->pieces[i].location,dice,player->pieces[i].direction==1 ? "Clockwise" : "Counterclockwise");

                if (player->pieces[i].location >= player->homeStart && old_location <= player->homeStart && player->pieces[i].xpass > 0 && player->pieces[i].direction == CLOCKWISE) {
                    player->pieces[i].status = HOME_STRAIGHT;
                    player->pieces[i].location = HOME_PATH_LENGTH - (new_location - player->homeStart);
                    printf("%s's piece %s moves to the home straight.\n", player->color, player->pieces[i].id);
                } else if (player->pieces[i].location < player->homeStart && old_location > player->homeStart && player->pieces[i].xpass > 0 && player->pieces[i].direction == ANTICLOCKWISE) {
                    player->pieces[i].status = HOME_STRAIGHT;
                    player->pieces[i].location = HOME_PATH_LENGTH - (player->homeStart - new_location);
                    printf("%s's piece %s moves to the home straight.\n", player->color, player->pieces[i].id);
                } else if (player->pieces[i].direction == ANTICLOCKWISE && player->pieces[i].location < player->homeStart && player->pieces[i].xpass == 0) {
                    player->pieces[i].xpass++;
                }
		//checking for captures
                capturePiece(player, players, numPlayers);
                break;
            } else if (player->pieces[i].status == HOME_STRAIGHT) {
                player->pieces[i].location += dice;
			//checking if piece reached the home
                if (player->pieces[i].location >= HOME_PATH_LENGTH) {
                    if (player->pieces[i].location == HOME_PATH_LENGTH) {
                        player->pieces[i].status = HOME;
                        player->piecesInHome++;
                        printf("%s's piece %s reaches home!\n", player->color, player->pieces[i].id);
                    } else {
                        printf("%s's piece %s can't move to home until the exact value is rolled.\n", player->color, player->pieces[i].id);
                        player->pieces[i].location -= dice;
                    }
                } else {
                    printf("%s's piece %s moves to position %d on the home straight.\n", player->color, player->pieces[i].id, player->pieces[i].location);
                }
                break;
            }
        }
        if (dice != 6) {
            break;
        }
    }
}

void moveYellow(Player* player, Player players[], int numPlayers) {
    int roll6count = 0;
    int pieceToMove = 0;

    // Continue rolling as long as dice roll is 6
    while (1) {
            
           int dice = rollDice();
        printf("%s player rolls a %d\n", player->color, dice);

        // Check for consecutive 6s
        if (dice == 6) {
            roll6count++;
            if (roll6count == 3) {
                printf("%s rolled three consecutive 6s. Turn ends.\n", player->color);
                return;  // End the function after 3 consecutive 6s
            }
        } else {
            roll6count = 0;  // Reset the count if a number other than 6 is rolled
        }

        // Move the first piece that can move
        for (int i = 0; i < NUM_PIECES; i++) {
            if (player->pieces[i].status == BASE && dice == 6) {
                   player->pieces[i].status = BOARD;
                   player->pieces[i].location = YX; 
                   player->piecesInBoard++;
        	   player->piecesInBase--;
                   int direction = toss();
                   player->pieces[i].direction = (direction == 1 ? CLOCKWISE:ANTICLOCKWISE);
                
		   if(direction ==1){
               		 player->pieces[i].xpass++;
                }  // Move only one piece to the board
                 	printf("%s player moves piece %s to the board at position %d.(Starting position of %s) and will move in %s direction\n", player->color, player->pieces[i].id, player->pieces[i].location,player->color,player->pieces[i].direction==1 ? "Clockwise" : "Counterclockwise");
		 	printf("\n%s player has now %d/4 on pieces on the board and %d/4 pieces on the base\n",player->color,player->piecesInBoard,player->piecesInBase);
                break; 
            } 
            else if (player->pieces[i].status == BOARD) {
			 if(player->pieces[i].isEnergized){
                   		 dice *= 2;
                 	 }else if(player->pieces[i].isSick){
                   		 dice /= 2;
                   	 }
                    		int old_location=player->pieces[i].location;
				player->pieces[i].location = (player->pieces[i].location + dice*player->pieces[i].direction) % BOARD_SIZE;
             		if(player->pieces[i].location<0){
                		player->pieces[i].location=BOARD_SIZE+player->pieces[i].location;
               			player->pieces[i].xpass++;
                }
				 printf("%s moves piece %s moves from location %d to location %d by %d units in %s direction\n", player->color, player->pieces[i].id,old_location, player->pieces[i].location,dice,player->pieces[i].direction==1 ? "Clockwise" : "Counterclockwise");
                // Check if the piece can enter the home straight(logic)
                if (player->pieces[i].location > old_location && player->pieces[i].xpass>0 && player->pieces[i].direction==-1) {
                        player->pieces[i].status = HOME_STRAIGHT;
                        player->pieces[i].location=HOME_PATH_LENGTH-(BOARD_SIZE-player->pieces[i].location);
                    printf("%s's piece %s moves to the home straight.\n", player->color, player->pieces[i].id);
                }else if (player->pieces[i].location < old_location && player->pieces[i].xpass>0&&player->pieces[i].direction==1) {
                        player->pieces[i].status = HOME_STRAIGHT;
                        player->pieces[i].location=HOME_PATH_LENGTH-player->pieces[i].location;

                    printf("%s's piece %s moves to the home straight.\n", player->color, player->pieces[i].id);
                }

                // Check for captures
                capturePiece(player, players, numPlayers);
                break;  // Move only one piece
            } 
            else if (player->pieces[i].status == HOME_STRAIGHT) {
                player->pieces[i].location += dice;

                if (player->pieces[i].location >= HOME_PATH_LENGTH) {
                    if (player->pieces[i].location == HOME_PATH_LENGTH) {
                        player->pieces[i].status = HOME;
                        player->piecesInHome++;
                        printf("%s's piece %s reaches home!\n", player->color, player->pieces[i].id);
                    } else {
                        printf("%s's piece %s can't move to home until the exact value is rolled.\n", player->color, player->pieces[i].id);
                        player->pieces[i].location -= dice;  // Undo the move if it overshoots home
                    }
                } else {
                    printf("%s's piece %s moves to position %d on the home straight.\n", player->color, player->pieces[i].id, player->pieces[i].location);


                }
                break;  // escape
            }
        }

            
            
            /* need a little bit more work , few logical errors are there.
             * this is yellow player's accurate playing behaviour
        int dice = rollDice();
        printf("%s player rolls a %d\n", player->color, dice);

        // Check for consecutive 6s
        if (dice == 6) {
            roll6count++;
            if (roll6count == 3) {
                printf("%s rolled three consecutive 6s. Turn ends.\n", player->color);
                return;  // End the function after 3 consecutive 6s
            }
        } else {
            roll6count = 0;  // Reset the count if a number other than 6 is rolled
        }

        // Move the first piece that can move
        for (int i = 0; i < NUM_PIECES; i++) {
            if (player->pieces[i].status == BASE && dice == 6) {
                 player->pieces[i].status = BOARD;
                player->pieces[i].location = BX; 
                 player->piecesInBoard++;
         player->piecesInBase--;// Start location for Blue
                 int direction = toss();
                player->pieces[i].direction = (direction == 1 ? CLOCKWISE:ANTICLOCKWISE);
                if(direction ==1){
                player->pieces[i].xpass++;
                }  // Move only one piece to the board
                 printf("%s's piece %s moves to the board at position %d.(Starting position of %s) and will move in %s direction\n", player->color, player->pieces[i].id, player->pieces[i].location,player->color,player->pieces[i].direction==1 ? "Clockwise" : "Counterclockwise");

                break; 
            } else if (player->pieces[i].status == HOME_STRAIGHT) {
                player->pieces[i].location += dice;

                if (player->pieces[i].location >= HOME_PATH_LENGTH) {
                    if (player->pieces[i].location == HOME_PATH_LENGTH) {
                        player->pieces[i].status = HOME;
                        player->piecesInHome++;
                        printf("%s's piece %s reaches home!\n", player->color, player->pieces[i].id);
                    } else {
                        printf("%s's piece %s can't move to home until the exact value is rolled.\n", player->color, player->pieces[i].id);
                        player->pieces[i].location -= dice;  // Undo the move if it overshoots home
                    }
                } else {
                    printf("%s's piece %s moves to position %d on the home straight.\n", player->color, player->pieces[i].id, player->pieces[i].location);


                }
               return;  // Move only one piece
            }

            else if (player->pieces[i].status == BOARD) {

                    capturePiece(player, players, numPlayers);

                    return;
                    }
            }

                    int closestToHome = BOARD_SIZE+1;
                    for(int i=0;i<NUM_PIECES;i++){
                        if(player->pieces[i].status==BOARD){
                        int distanceToHome=calculateDistancetoHome(player, i);
                        if(distanceToHome<closestToHome){
                                closestToHome = distanceToHome;
                                pieceToMove = i;
                        }
                }
            }

                    printf("\n\n%d number %d\n\n",closestToHome,pieceToMove);
                    int old_location=player->pieces[pieceToMove].location;
player->pieces[pieceToMove].location = (player->pieces[pieceToMove].location + dice*player->pieces[pieceToMove].direction) % BOARD_SIZE;
                if(player->pieces[pieceToMove].location<0){
                player->pieces[pieceToMove].location=BOARD_SIZE+player->pieces[pieceToMove].location;
                player->pieces[pieceToMove].xpass++;
                }
printf("\n\n%s %d \n\n",player->pieces[pieceToMove].id ,player->pieces[pieceToMove].location);
                printf("%s's piece %s moves to location %d on the board.\n", player->color, player->pieces[pieceToMove].id, player->pieces[pieceToMove].location);

                // Check if the piece can enter the home straight
                if (player->pieces[pieceToMove].location > old_location && player->pieces[pieceToMove].xpass>0 && player->pieces[pieceToMove].direction==-1) {
                        player->pieces[pieceToMove].status = HOME_STRAIGHT;
                        player->pieces[pieceToMove].location=HOME_PATH_LENGTH-(BOARD_SIZE-player->pieces[pieceToMove].location);
                    printf("%s's piece %s moves to the home straight.\n", player->color, player->pieces[pieceToMove].id);
                }else if (player->pieces[pieceToMove].location < old_location && player->pieces[pieceToMove].xpass>0&&player->pieces[pieceToMove].direction==1) {
                        player->pieces[pieceToMove].status = HOME_STRAIGHT;
                        player->pieces[pieceToMove].location=HOME_PATH_LENGTH-player->pieces[pieceToMove].location;

                    printf("%s's piece %s moves to the home straight.\n", player->color, player->pieces[pieceToMove].id);
                }

                // Check for captures
                               break;  // Move only one piece
           */ 
                  


        // If the dice roll is not 6, the turn ends
        	if (dice != 6) {
        	    break;
        	}
	}
}


// Similar logic to moveRed but with specific Green rules (starting at GX)
void moveGreen(Player* player, Player players[], int numPlayers) {
    int roll6count = 0;

    // Continue rolling as long as dice roll is 6
    while (1) {
        int dice = rollDice();
        printf("%s player rolls a %d\n", player->color, dice);

        // Check for consecutive 6s
        if (dice == 6) {
            roll6count++;
            if (roll6count == 3) {
                printf("%s rolled three consecutive 6s. Turn ends.\n", player->color);
                return;  // End the function after 3 consecutive 6s
            }
        }else{
            roll6count = 0;  // Reset the count if a number other than 6 is rolled
        }

        // Move the first piece that can move
        for (int i = 0; i < NUM_PIECES; i++) {
            if (player->pieces[i].status == BASE && dice == 6) {
   		   player->pieces[i].status = BOARD;
                   player->pieces[i].location = GX; 
                   player->piecesInBoard++;
        	   player->piecesInBase--;// Start location for Blue
                   int direction = toss();
                   player->pieces[i].direction = (direction == 1 ? CLOCKWISE:ANTICLOCKWISE);
                if(direction ==1){
                   player->pieces[i].xpass++;
                }  // Move only one piece to the board
                   printf("%s's piece %s moves to the board at position %d.(Starting position of %s) and will move in %s direction\n", player->color, player->pieces[i].id, player->pieces[i].location,player->color,player->pieces[i].direction==1 ? "Clockwise" : "Counterclockwise");
		   printf("\n%s player has now %d/4 on pieces on the board and %d/4 pieces on the base\n",player->color,player->piecesInBoard,player->piecesInBase);
                    break; 
            } 
            else if (player->pieces[i].status == BOARD) {
			if(player->pieces[i].isEnergized){
                    		dice *= 2;
                       }else if(player->pieces[i].isSick){
                      		dice /= 2;
                       }
   				int old_location=player->pieces[i].location;
               			int new_location=(player->pieces[i].location + dice*player->pieces[i].direction) % BOARD_SIZE;
                        if(new_location<0){
                		player->pieces[i].location=BOARD_SIZE+new_location;
                	}else{
                		player->pieces[i].location=new_location;
                }

                 		printf("%s moves piece %s moves from location %d to location %d by %d units in %s direction\n", player->color, player->pieces[i].id,old_location, player->pieces[i].location,dice,player->pieces[i].direction==1 ? "Clockwise" : "Counterclockwise");

                // Check if the piece can enter the home straight
                if (player->pieces[i].location >=player->homeStart && old_location<=player->homeStart && player->pieces[i].xpass>0 && player->pieces[i].direction==1) {
                    player->pieces[i].status = HOME_STRAIGHT;
                    player->pieces[i].location = HOME_PATH_LENGTH - (new_location-player->homeStart);
                printf("%s's piece %s moves to the home straight.\n", player->color, player->pieces[i].id);
        }
                else if(player->pieces[i].location <player->homeStart && old_location>player->homeStart && player->pieces[i].xpass>0 && player->pieces[i].direction==-1) {
                    player->pieces[i].status = HOME_STRAIGHT;
                    player->pieces[i].location = HOME_PATH_LENGTH - (new_location-player->homeStart);
	            printf("%s's piece %s moves to the home straight.\n", player->color, player->pieces[i].id);
                }else if (player->pieces[i].direction==-1 && player->pieces[i].location <player->homeStart && player->pieces[i].xpass==0){
               		 player->pieces[i].xpass++;
                }


                // Check for captures
                capturePiece(player, players, numPlayers);
                break;  //escape 
            } 
            else if (player->pieces[i].status == HOME_STRAIGHT) {
                player->pieces[i].location += dice;

                if (player->pieces[i].location >= HOME_PATH_LENGTH) {
                    if (player->pieces[i].location == HOME_PATH_LENGTH) {
                        player->pieces[i].status = HOME;
                        player->piecesInHome++;
                        printf("%s's piece %s reaches home!\n", player->color, player->pieces[i].id);
                    }else{
                        printf("%s's piece %s can't move to home until the exact value is rolled.\n", player->color, player->pieces[i].id);
                        player->pieces[i].location -= dice;  // Undo the move if it overshoots home
                    }
                }else{
                 	printf("%s's piece %s moves to position %d on the home straight.\n", player->color, player->pieces[i].id, player->pieces[i].location);
                }
                break;  
            }
        }

        // If the dice roll is not 6, the turn ends
        if (dice != 6) {
            break;
        }
    }
}
// Similar logic to moveRed but with specific Blue rules (starting at BX)
void moveBlue(Player* player, Player players[], int numPlayers) {
    int roll6count = 0;
   // static int playerIndex =0; useful when coding blue player's behavious. we can remove for loops and increment playerIndex as (playerIndex +1)%4 which gives circular behaviour of blue.

    // Continue rolling as long as dice roll is 6
    while (1) {
        int dice = rollDice();
        printf("%s player rolls a %d\n", player->color, dice);

        // Check for consecutive 6s
        if (dice == 6) {
            roll6count++;
            if (roll6count == 3) {
                printf("%s rolled three consecutive 6s. Turn ends.\n", player->color);
                return;  // End the function after 3 consecutive 6s
            }
        } else {
            roll6count = 0;  // Reset the count if a number other than 6 is rolled
        }

        // Move the first piece that can move
       for (int i = 0; i < NUM_PIECES; i++) {

            if (player->pieces[i].status == BASE && dice == 6) {
               	 player->pieces[i].status = BOARD;
               	 player->pieces[i].location = BX; 
               	 player->piecesInBoard++;
               	 player->piecesInBase--;// Start location for Blue
                 int direction = toss();
               	 player->pieces[i].direction = (direction == 1 ? CLOCKWISE:ANTICLOCKWISE);
                
		if(direction ==1){
               		 player->pieces[i].xpass++;
               	 }  
		// Move only one piece to the board
                 printf("%s's piece %s moves to the board at position %d.(Starting position of %s) and will move in %s direction\n", player->color, player->pieces[i].id, player->pieces[i].location,player->color,player->pieces[i].direction==1 ? "Clockwise" : "Counterclockwise");

		printf("\n%s player has now %d/4 on pieces on the board and %d/4 pieces on the base\n",player->color,player->piecesInBoard,player->piecesInBase);

                             break; 

                            } 
            else if (player->pieces[i].status == BOARD) {
			if(player->pieces[i].isEnergized){
                   		 dice *= 2;
                    	}else if(player->pieces[i].isSick){
                    		 dice /= 2;
                   	}

int old_location=player->pieces[i].location;
               int new_location=(player->pieces[i].location + dice*player->pieces[i].direction) % BOARD_SIZE;
               
	       if(new_location<0){
               		 player->pieces[i].location=BOARD_SIZE+new_location;
              	  }else{
                	 player->pieces[i].location=new_location;
                }

                //printing move function
 printf("%s moves piece %s moves from location %d to location %d by %d units in %s direction\n", player->color, player->pieces[i].id,old_location, player->pieces[i].location,dice,player->pieces[i].direction==1 ? "Clockwise" : "Counterclockwise");

                // Check if the piece can enter the home straight (logic)
                if (player->pieces[i].location >=player->homeStart && old_location<=player->homeStart && player->pieces[i].xpass>0 && player->pieces[i].direction==1) {
                    player->pieces[i].status = HOME_STRAIGHT;
                    player->pieces[i].location = HOME_PATH_LENGTH - (new_location-player->homeStart);
                    printf("%s's piece %s moves to the home straight.\n", player->color, player->pieces[i].id);
                }else if(player->pieces[i].location < player->homeStart && old_location>player->homeStart && player->pieces[i].xpass>0 && player->pieces[i].direction==-1) {
                    player->pieces[i].status = HOME_STRAIGHT;
                    player->pieces[i].location = HOME_PATH_LENGTH - (player->homeStart-new_location);
                    printf("%s's piece %s moves to the home straight.\n", player->color, player->pieces[i].id);


                }else if (player->pieces[i].direction==-1 && player->pieces[i].location <player->homeStart && player->pieces[i].xpass==0){
                player->pieces[i].xpass++;
                }


                // Check for captures
                capturePiece(player, players, numPlayers);
                break; 
            } 
            else if (player->pieces[i].status == HOME_STRAIGHT) {
                player->pieces[i].location += dice;

                if (player->pieces[i].location >= HOME_PATH_LENGTH) {
                    if (player->pieces[i].location == HOME_PATH_LENGTH) {
                        player->pieces[i].status = HOME;
                        player->piecesInHome++;
                        printf("%s's piece %s reaches home!\n", player->color, player->pieces[i].id);
                    }else{
                        printf("%s's piece %s can't move to home until the exact value is rolled.\n", player->color, player->pieces[i].id);
                        player->pieces[i].location -= dice;  // Undo the move if it overshoots home
                         }
                }else{
                    printf("%s's piece %s moves to position %d on the home straight.\n", player->color, player->pieces[i].id, player->pieces[i].location);
                     }
                break; 
            }
        
    }
        // If the dice roll is not 6, the turn ends
        if (dice != 6) {
            break;
        }
    }
}
//printing status of each player after a round
void displayStatus(Player players[]){
        for(int i=0;i<NUM_PLAYERS;i++){
        printf("\n%s player now has %d/4 pieces on the board and %d/4 pieces are in the base\n",players[i].color,players[i].piecesInBoard,players[i].piecesInBase);

        printf("============================\nLocation of pieces [%s]\n============================\n",players[i].color);
        for(int j=0;j<4;j++)
 	    {
       		 if(players[i].pieces[j].status==BASE){
      			  printf("Piece %s -> Base\n",players[i].pieces[j].id);
        	}else if(players[i].pieces[j].status==HOME){
      			  printf("Piece %s -> Home\n",players[i].pieces[j].id);
         	}else if(players[i].pieces[j].status==HOME_STRAIGHT){
      			  printf("Piece %s -> in Home Straight\n",players[i].pieces[j].id);
        	}else if(players[i].pieces[j].status==BOARD){
        		  printf("Piece %s -> %d\n",players[i].pieces[j].id,players[i].pieces[j].location);
        	}

  	    }
      }
printf("\n");
}

