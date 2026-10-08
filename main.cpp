#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <unistd.h>
#include <thread>
using namespace std;

enum enChoices { Rock=1, Paper=2, Scissors=3 };
enum enWinner { Player1=1, Computer=2, Draw=3 };

struct stRoundResults 
{
    int roundNumber = 0;
    enChoices player1Choice;
    enChoices computerChoice;
    enWinner roundWinner;
    string winnerName;
};

struct stGameResults 
{
    int GameRounds = 0;
    int player1WinTimes = 0;
    int computerWinTimes = 0;
    int drawTimes = 0;

    enWinner finalWinner;
    string winnerName;
};

void ClearScreen()
{
    system("clear");
}

string Tabs(int x)
{
    string tabs = "";

    for (int counter=1; counter <= x; counter++ )
    {
        tabs = tabs + '\t';
    }

    return tabs;
}

short ReadGameRounds()
{
    int userInput;

    do {

        cout << "How many round do you want to play 1 to 10: ";
        cin >> userInput;

    } while (userInput < 1 || userInput > 10);

    return userInput;
}

enChoices ReadPlayer1Choice()
{
    short userChoice;

    do {

        cout << "Your choice? [1] Rock, [2] Paper, [3] scissors: ";
        cin >> userChoice;

    } while ( userChoice < 1 || userChoice > 3);

    return (enChoices) userChoice ;
}

int RandomNumber(int from, int to)
{
    return rand() % (to - from + 1) + from;
}

enChoices GetComputerChoice()
{
    return (enChoices) RandomNumber(1, 3) ;
}

enWinner GetRoundWinner(stRoundResults roundResults)
{
    if (roundResults.player1Choice == roundResults.computerChoice) return enWinner::Draw;

    switch (roundResults.player1Choice)
    {
        case enChoices::Paper:
            if (roundResults.computerChoice == enChoices::Scissors)
                return enWinner::Computer;

        case enChoices::Rock:
            if (roundResults.computerChoice == enChoices::Paper)
                return enWinner::Computer;

        case enChoices::Scissors:
            if (roundResults.computerChoice == enChoices::Rock)
                return enWinner::Computer;
    }

    return enWinner::Player1;
}

string WinnerName(enWinner winner)
{
    string arrWinnerName[3] = {"Player1", "Computer", "No Winner (Draw)"};

    return arrWinnerName[winner - 1];
}

string ChoiceName(enChoices choice)
{
    string arrChoiceName[3] = {"Rock", "Paper", "scissors"};

    return arrChoiceName[choice - 1];
}

void PrintRoundResults(stRoundResults roundResults)
{
    cout << "____________ Round [" << roundResults.roundNumber << "] ____________" << endl;
    cout << "Player1 choice  : " << ChoiceName(roundResults.player1Choice) << '.' << endl;
    cout << "Computer choice : " << ChoiceName(roundResults.computerChoice) << '.' << endl;
    cout << "Round Winner    : " << roundResults.winnerName << endl;
    cout << "_________________________________" << endl;
}

void PrintGameOverBar()
{
    cout << Tabs(2) << "------------------------------------------------" << endl;
    cout << Tabs(2) << "             |++ G A M E  O V E R ++|           " << endl;
    cout << Tabs(2) << "------------------------------------------------" << endl;
}

void LoadingBar()
{
    cout << "\nLoading results [";
    for (int counter=1; counter<=10; counter++)
    {
        cout << "#";
        cout << flush;

        this_thread::sleep_for(chrono::milliseconds(250));
    }
    cout << "] 100%" << endl;

    sleep(2);
}

enWinner FinalWinner(stGameResults gameResults)
{
    if ( gameResults.player1WinTimes > gameResults.computerWinTimes )
        return enWinner::Player1;

    else if ( gameResults.computerWinTimes > gameResults.player1WinTimes )
        return enWinner::Computer;

    else {
        return enWinner::Draw;
    }
}

stGameResults FillGameResults(short gameRounds, short player1WinTimes, short computerWinTimes, short drawTimes)
{
    stGameResults gameResults;

    gameResults.GameRounds = gameRounds;
    gameResults.player1WinTimes = player1WinTimes;
    gameResults.computerWinTimes = computerWinTimes;
    gameResults.drawTimes = drawTimes;

    gameResults.finalWinner = FinalWinner(gameResults);
    gameResults.winnerName = WinnerName(gameResults.finalWinner);

    return gameResults;
}

stGameResults StartGame(short GameRounds)
{
    stRoundResults roundResults;
    short player1WinTimes = 0, computerWinTimes = 0, drawTimes = 0;

    for (int counter=1; counter <= GameRounds; counter++ )
    {
        cout << "\nRound[" << counter << "] begins:" << endl;

        roundResults.roundNumber = counter;
        roundResults.player1Choice = ReadPlayer1Choice();
        roundResults.computerChoice = GetComputerChoice();
        roundResults.roundWinner = GetRoundWinner(roundResults);
        roundResults.winnerName = WinnerName(roundResults.roundWinner);

        PrintRoundResults(roundResults);

        switch (roundResults.roundWinner)
        {
            case enWinner::Player1:
                player1WinTimes++;
                break;

            case enWinner::Computer:
                computerWinTimes ++;
                break;

            case enWinner::Draw: 
                drawTimes++;
                break;
        }
    }

    LoadingBar();

    return FillGameResults(GameRounds, player1WinTimes, computerWinTimes, drawTimes);
}

void PrintGameResults(stGameResults gameResults)
{
    cout << Tabs(2) << "---------------[Game results]------------" << endl;
    cout << Tabs(2) << "Game rounds        : " << gameResults.GameRounds << endl;
    cout << Tabs(2) << "Player1 won times  : " << gameResults.player1WinTimes << endl;
    cout << Tabs(2) << "Computer won times : " << gameResults.computerWinTimes << endl;
    cout << Tabs(2) << "Draw times         : " << gameResults.drawTimes << endl;
    cout << Tabs(2) << "Final winner       : " << gameResults.winnerName << endl;
    cout << Tabs(2) << "-----------------------------------------" << endl;
}

void PlayGame()
{
    char playAgain = 'Y';

    do {

        ClearScreen();

        stGameResults gameResults =  StartGame(ReadGameRounds());

        ClearScreen();

        PrintGameOverBar();
        PrintGameResults(gameResults);
        
        cout << Tabs(2) << "Do you want to play again? (Y/N): ";
        cin >> playAgain;

    } while ( playAgain == 'Y' || playAgain == 'y');
}

int main()
{
    srand( (unsigned) time (NULL) );

    PlayGame();

    return 0;
}
